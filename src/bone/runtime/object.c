#include "object.h"
#include "../util/stack.h"
#include "array.h"
#include "bool.h"
#include "char.h"
#include "double.h"
#include "frame.h"
#include "heap.h"
#include "integer.h"
#include "interpreter.h"
#include "keyword.h"
#include "lambda.h"
#include "object.h"
#include "vm.h"

void bnInitObject(struct bnHeap* heap, bnObject* self, bnObjectType type) {
        self->table =
            g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, NULL);
        self->mark = false;
        self->type = type;
        self->freeFunc = NULL;
        bnAddToHeap(heap, self);
}

bnObject* bnNewObject(struct bnHeap* heap) {
        bnObject* ret = BN_MALLOC(sizeof(bnObject));
        bnInitObject(heap, ret, BN_OBJECT_PROTO);
        return ret;
}

void bnDefine(bnObject* self, bnStringView name, bnObject* value) {
        int v = (int)name;
        g_hash_table_replace(self->table, GINT_TO_POINTER(v), value);
}

bnFrame* bnFuncCall(bnObject* self, bnInterpreter* bone, bnFrame* frame,
                    int argc) {
        assert(self != NULL && self->type == BN_OBJECT_LAMBDA);
        bnLambda* lambda = self;
        // int paramLen = g_list_length(lambda->parameters);
        // assert(paramLen == argc);
        // create new frame
        bnFrame* sub = bnSubFrame(frame);
        sub->currentCall = lambda;
        for (int i = 0; i < argc; i++) {
                bnPushStack(sub->vStack, bnPopStack(frame->vStack));
        }
        if (bnIsInstanceBaseLambda(bone->pool, lambda) &&
            bnGetStackSize(frame->hierarcySelf) > 0) {
                bnPushStack(sub->vStack, bnPeekStack(frame->hierarcySelf));
        }
        // set default return value
        GList* retIter = lambda->returns;
        while (retIter != NULL) {
                g_hash_table_replace(sub->variableTable, retIter->data,
                                     bnNewObject(bone->heap));
                retIter = retIter->next;
        }
        if (lambda->type == BN_LAMBDA_NATIVE) {
                // write captured vatiable
                GHashTableIter iter;
                gpointer k, v;
                g_hash_table_iter_init(&iter, frame->variableTable);
                while (g_hash_table_iter_next(&iter, &k, &v)) {
                        g_hash_table_replace(sub->variableTable, k, v);
                }

                int code = BN_JMP_PUSH(bone->__jstack);
                if (code == 0) {
                        lambda->u.vFunc(bone, sub);
                } else {
                        if (code != BN_JMP_CODE_EXCEPTION) {
                                abort();
                        } else {
                                // on exception...
                        }
                }
                BN_JMP_POP(bone->__jstack);
        } else {
                // write captured vatiable
                GHashTableIter iter;
                gpointer k, v;
                g_hash_table_iter_init(&iter, lambda->outer);
                while (g_hash_table_iter_next(&iter, &k, &v)) {
                        g_hash_table_replace(sub->variableTable, k, v);
                }
                bnExecute(bone, lambda->u.vEnv, sub);
        }
        if (bnIsVariadicReturn(bone->pool, lambda)) {
                bnObject* arr = bnExportAllVariable(bone, sub);
                bnPushStack(frame->vStack, arr);
        } else if (g_list_length(lambda->returns) > 0) {
                bnObject* body = g_hash_table_lookup(sub->variableTable,
                                                     lambda->returns->data);
                assert(body != NULL);
                GList* iter = lambda->returns;
                while (iter != NULL) {
                        bnStringView retName = iter->data;
                        // create private member
                        bnStringView exportName =
                            bnGetExportVariableName(bone->pool, retName);
                        const char* expstr = bnView2Str(bone->pool, exportName);
                        g_hash_table_replace(
                            body->table, exportName,
                            g_hash_table_lookup(sub->variableTable,
                                                iter->data));
                        iter = iter->next;
                }
                bnPushStack(frame->vStack, body);
        }
        return sub;
}

void bnPrintObject(FILE* fp, bnObject* self) {
        switch (self->type) {
                case BN_OBJECT_PROTO:
                        fprintf(fp, "proto %p", (void*)self);
                        break;
                case BN_OBJECT_INTEGER:
                        fprintf(fp, "int %d", ((bnInteger*)self)->value);
                        break;
                case BN_OBJECT_DOUBLE:
                        fprintf(fp, "double %lf", ((bnDouble*)self)->value);
                        break;
                case BN_OBJECT_CHAR:
                        fprintf(fp, "char %c", ((bnChar*)self)->value);
                        break;
                case BN_OBJECT_STRING:
                        fprintf(fp, "string");
                        break;
                case BN_OBJECT_BOOL:
                        fprintf(fp, "bool");
                        break;
                case BN_OBJECT_LAMBDA:
                        fprintf(fp, "lambda");
                        break;
        }
}

bool bnObject2CBool(bnObject* self) {
        if (self->type == BN_OBJECT_BOOL) {
                return ((bnBool*)self)->value;
        } else if (self->type == BN_OBJECT_INTEGER) {
                return ((bnInteger*)self)->value != 0;
        }
        return true;
}

bnStringView bnGetExportVariableName(struct bnStringPool* pool,
                                     bnStringView view) {
        const char* str = bnView2Str(pool, view);
        char buf[100];
        sprintf(buf, "$$_%s", str);
        return bnIntern(pool, buf);
}

void bnDeleteObject(bnObject* self) {
        if (self->freeFunc != NULL) {
                self->freeFunc(self);
        } else {
                g_hash_table_destroy(self->table);
                BN_FREE(self);
        }
}