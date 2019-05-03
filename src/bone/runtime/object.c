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
        if (lambda->type == BN_LAMBDA_NATIVE) {
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
        guint len = bnGetStackSize(sub->vStack);
        bnObject* ret = len == 0 ? NULL : bnPopStack(sub->vStack);
        bnPushStack(frame->vStack, ret);
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