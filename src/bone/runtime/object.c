#include "object.h"
#include "../util/stack.h"
#include "bool.h"
#include "char.h"
#include "double.h"
#include "frame.h"
#include "integer.h"
#include "interpreter.h"
#include "keyword.h"
#include "lambda.h"
#include "object.h"
#include "vm.h"

void bnInitObject(bnObject* self, bnObjectType type) {
        self->table =
            g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, NULL);
        self->mark = false;
        self->type = type;
}

bnObject* bnNewObject() {
        bnObject* ret = BN_MALLOC(sizeof(bnObject));
        bnInitObject(ret, BN_OBJECT_PROTO);
        return ret;
}

void bnDefine(bnObject* self, bnStringView name, bnObject* value) {
        int v = (int)name;
        g_hash_table_insert(self->table, GINT_TO_POINTER(v), value);
}

void bnFuncCall(bnObject* self, bnInterpreter* bone, bnFrame* frame, int argc) {
        assert(self != NULL && self->type == BN_OBJECT_LAMBDA);
        // create new frame
        bnFrame* sub = bnSubFrame(frame);
        for (int i = 0; i < argc; i++) {
                bnPushStack(sub->vStack, bnPopStack(frame->vStack));
        }
        bnLambda* lambda = self;
        if (lambda->instanceBase && bnGetStackSize(frame->hierarcySelf) > 0) {
                bnPushStack(sub->vStack, bnPeekStack(frame->hierarcySelf));
        }
        if (lambda->type == BN_LAMBDA_NATIVE) {
                // write captured vatiable
                GHashTableIter iter;
                gpointer k, v;
                g_hash_table_iter_init(&iter, frame->variableTable);
                while (g_hash_table_iter_next(&iter, &k, &v)) {
                        g_hash_table_insert(sub->variableTable, k, v);
                }

                int code = setjmp(bone->__jmp);
                if (code == 0) {
                        lambda->u.vFunc(bone, sub);
                } else {
                        assert(code == BN_JMP_CODE_EXCEPTION);
                }
        } else {
                // write captured vatiable
                GHashTableIter iter;
                gpointer k, v;
                g_hash_table_iter_init(&iter, lambda->outer);
                while (g_hash_table_iter_next(&iter, &k, &v)) {
                        g_hash_table_insert(sub->variableTable, k, v);
                }
                bnExecute(bone, lambda->u.vEnv, sub);
        }
        if (g_list_length(lambda->returns) > 0) {
                bnObject* body = g_hash_table_lookup(sub->variableTable,
                                                     lambda->returns->data);
                assert(body != NULL);
                GList* iter = lambda->returns;
                while (iter != NULL) {
                        bnStringView retName = iter->data;
                        // create private member
                        char buf[100] = {0};
                        const char* retStr = bnView2Str(bone->pool, retName);
                        sprintf(buf, "$$_%s", retStr);
                        g_hash_table_insert(
                            body->table, bnIntern(bone->pool, buf),
                            g_hash_table_lookup(sub->variableTable,
                                                iter->data));
                        iter = iter->next;
                }
                bnPushStack(frame->vStack, body);
        }
        bnDeleteFrame(sub);
}

void bnPrintObject(FILE* fp, bnObject* self) {
        switch (self->type) {
                case BN_OBJECT_PROTO:
                        fprintf(fp, "%p", (void*)self);
                        break;
                case BN_OBJECT_INTEGER:
                        fprintf(fp, "%d", ((bnInteger*)self)->value);
                        break;
                case BN_OBJECT_DOUBLE:
                        fprintf(fp, "%lf", ((bnDouble*)self)->value);
                        break;
                case BN_OBJECT_CHAR:
                        fprintf(fp, "%c", ((bnChar*)self)->value);
                        break;
                case BN_OBJECT_STRING:
                        break;
                case BN_OBJECT_BOOL:
                        break;
                case BN_OBJECT_LAMBDA:
                        fprintf(fp, "lambda");
                        break;
        }
}

void bnDeleteObject(bnObject* self) {}