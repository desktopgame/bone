#include "object.h"
#include <stdlib.h>
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
#include "std.h"
#include "string.h"
#include "vm.h"

#define _throw(bone, frame, fmt) (bnFormatThrow(bone, fmt))

static void bnStdObjectEqual(bnInterpreter* bone, bnFrame* frame);
static void bnStdObjectNotEqual(bnInterpreter* bone, bnFrame* frame);
static void bnStdObjectToString(bnInterpreter* bone, bnFrame* frame);

void bnInitObject(bnInterpreter* bone, bnObject* self, bnObjectType type) {
        self->table =
            g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, NULL);
        self->mark = false;
        self->type = type;
        self->freeFunc = NULL;
        bnAddToHeap(bone->heap, self);
}

void bnIncludeKernel(bnInterpreter* bone, bnObject* self) {
        bnDefine(self, bnIntern(bone->pool, BN_KWD_EQUAL),
                 bnNewLambdaFromCFunc(bone, bnStdObjectEqual, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(self, bnIntern(bone->pool, BN_KWD_NOTEQUAL),
                 bnNewLambdaFromCFunc(bone, bnStdObjectNotEqual, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(self, bnIntern(bone->pool, "toString"),
                 bnNewLambdaFromCFunc(bone, bnStdObjectToString, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_RETURN,
                                      "ret", BN_C_ADD_EXIT));
}

bnObject* bnNewObject(bnInterpreter* bone) {
        bnObject* ret = BN_MALLOC(sizeof(bnObject));
        bnInitObject(bone, ret, BN_OBJECT_PROTO);
        bnIncludeKernel(bone, ret);
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
                argc++;
        }
        int paramc = g_list_length(lambda->parameters);
        if (paramc != argc) {
                sub->panic = frame->panic = bnNewString(
                    bone, bnIntern(bone->pool, "illegal arguments"));
                return sub;
        }
        // set default return value
        GList* retIter = lambda->returns;
        while (retIter != NULL) {
                g_hash_table_replace(sub->variableTable, retIter->data,
                                     bnNewObject(bone));
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
                // staging all arguments.
                bnPushStage(bone->heap);
                bnStackElement* stackIter = sub->vStack->head;
                while (stackIter != NULL) {
                        bnStaging(bone->heap, stackIter->value);
                        stackIter = stackIter->next;
                }
                int code = BN_JMP_PUSH(bone->__jstack);
                if (code == 0) {
                        lambda->u.vFunc(bone, sub);
                        if (sub->panic != NULL) {
                                frame->panic = sub->panic;
                        }
                } else {
                        if (code != BN_JMP_CODE_EXCEPTION) {
                                abort();
                        } else {
                                // on exception...
                                frame->panic = sub->panic;
                        }
                }
                BN_JMP_POP(bone->__jstack);
                bnPopStage(bone->heap);
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
                assert(lambda->returns->data != NULL);
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

void bnPrintObject(FILE* fp, bnInterpreter* bone, bnObject* self) {
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
                        fprintf(
                            fp, "\"%s\"",
                            bnView2Str(bone->pool, ((bnString*)self)->value));
                        break;
                case BN_OBJECT_BOOL:
                        fprintf(fp, "bool %s",
                                ((bnBool*)self)->value ? "true" : "false");
                        break;
                case BN_OBJECT_LAMBDA:
                        fprintf(fp, "lambda");
                        break;
                case BN_OBJECT_ARRAY:
                        fprintf(fp, "array[%d]", ((bnArray*)self)->size);
                        break;
        }
}

bool bnObject2CBool(bnObject* self) {
        assert(self != NULL);
        if (self->type == BN_OBJECT_BOOL) {
                return ((bnBool*)self)->value;
        } else if (self->type == BN_OBJECT_INTEGER) {
                return ((bnInteger*)self)->value != 0;
        }
        return true;
}

void bnCleanupInjectionBuffer(struct bnStringPool* pool, bnObject* self) {
        GHashTableIter hashIter;
        gpointer k, v;
        g_hash_table_iter_init(&hashIter, self->table);
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                bnStringView view = k;
                const char* str = bnView2Str(pool, view);
                if (g_str_has_prefix(str, "$$_")) {
                        g_hash_table_iter_remove(&hashIter);
                }
        }
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
                assert(self->freeFunc != NULL);
                self->freeFunc(self);
        } else {
                g_hash_table_destroy(self->table);
                BN_FREE(self);
        }
}
// Object

static void bnStdObjectEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, a == b));
}

static void bnStdObjectNotEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, a != b));
}

static void to_string(bnInterpreter* bone, GString* str, bnObject* root,
                      int depth) {
        GHashTableIter hashIter;
        g_hash_table_iter_init(&hashIter, root->table);
        gpointer k, v;
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                bnStringView name = (bnStringView)k;
                const char* vname = bnView2Str(bone->pool, name);
                if (*vname == '$') {
                        continue;
                }
                bnObject* entry = (bnObject*)v;
                for (int i = 0; i < depth; i++) {
                        g_string_append(str, "    ");
                }
                g_string_append(str, bnView2Str(bone->pool, name));
                g_string_append(str, "\n");
                to_string(bone, str, entry, depth + 1);
        }
        if (root->type == BN_OBJECT_ARRAY) {
                bnArray* ary = root;
                for (int n = 0; n < ary->arr->len; n++) {
                        bnObject* v = g_ptr_array_index(ary->arr, n);

                        for (int i = 0; i < depth; i++) {
                                g_string_append(str, "    ");
                        }
                        g_string_append_printf(str, "[%d]\n", n);
                        to_string(bone, str, v, depth + 1);
                }
        }
}

static void bnStdObjectToString(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        GString* str = g_string_new("ROOT\n");
        to_string(bone, str, a, 1);
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewString2(bone, str->str));
        g_string_free(str, TRUE);
}
