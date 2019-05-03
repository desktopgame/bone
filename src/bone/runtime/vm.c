#include "vm.h"
#include "../glib.h"
#include "integer.h"
#include "lambda.h"
#include "opcode.h"
#include "string.h"

void bnDebugStack(FILE* fp, bnStack* stack, const char* name) {
        fprintf(fp, "--- %s ---\n", name == NULL ? "" : name);
        bnStackElement* iter = stack->head;
        while (iter != NULL) {
                bnPrintObject(fp, (bnObject*)iter->value);
                fprintf(fp, "\n");
                iter = iter->next;
        }
}

int bnExecute(bnInterpreter* bone, bnEnviroment* env, bnFrame* frame) {
        for (int PC = 0; PC < env->codeArray->len; PC++) {
                bnOpcode code = g_ptr_array_index(env->codeArray, PC);
                switch (code) {
                        case BN_OP_NOP:
                                break;
                        case BN_OP_DUP: {
                                gpointer data = bnPopStack(frame->vStack);
                                bnPushStack(frame->vStack, data);
                                bnPushStack(frame->vStack, data);
                                break;
                        }
                        case BN_OP_SWAP: {
                                bnDebugStack(stdout, frame->vStack, "preSwap");
                                void* a = bnPopStack(frame->vStack);
                                void* b = bnPopStack(frame->vStack);
                                bnPushStack(frame->vStack, a);
                                bnPushStack(frame->vStack, b);
                                bnDebugStack(stdout, frame->vStack, "postSwap");
                                break;
                        }
                        case BN_OP_GEN_INT: {
                                int data =
                                    g_ptr_array_index(env->codeArray, ++PC);
                                bnPushStack(frame->vStack,
                                            bnNewInteger(bone, data));
                                break;
                        }
                        case BN_OP_GEN_DOUBLE:
                                break;
                        case BN_OP_GEN_STRING: {
                                bnStringView name =
                                    g_ptr_array_index(env->codeArray, ++PC);
                                bnPushStack(frame->vStack,
                                            bnNewString(bone, name));
                                break;
                        }
                        case BN_OP_GEN_LAMBDA_BEGIN: {
                                bnLambda* lmb = bnNewLambda(BN_LAMBDA_SCRIPT);
                                lmb->u.vEnv = bnNewEnviroment();
                                // is instance base?
                                bool isInstanceBase =
                                    g_ptr_array_index(env->codeArray, ++PC);
                                lmb->instanceBase = isInstanceBase;
                                // length of named return
                                int namedReturnLen =
                                    g_ptr_array_index(env->codeArray, ++PC);
                                for (int i = 0; i < namedReturnLen; i++) {
                                        lmb->returns = g_list_append(
                                            lmb->returns,
                                            g_ptr_array_index(env->codeArray,
                                                              ++PC));
                                }
                                // collect all variables
                                GHashTableIter hashIter;
                                g_hash_table_iter_init(&hashIter,
                                                       frame->variableTable);
                                gpointer k, v;
                                while (
                                    g_hash_table_iter_next(&hashIter, &k, &v)) {
                                        g_hash_table_insert(lmb->outer, k, v);
                                }
                                // generate code
                                while (1) {
                                        gpointer data = g_ptr_array_index(
                                            env->codeArray, ++PC);
                                        if (data == BN_OP_GEN_LAMBDA_END) {
                                                break;
                                        }
                                        g_ptr_array_add(lmb->u.vEnv->codeArray,
                                                        data);
                                }
                                bnPushStack(frame->vStack, lmb);
                                break;
                        }
                        case BN_OP_GEN_LAMBDA_END: {
                                break;
                        }
                        case BN_OP_SET_REGISTER_0:
                                frame->register0 = bnPopStack(frame->vStack);
                                break;
                        case BN_OP_GET_REGISTER_0:
                                bnPushStack(frame->vStack, frame->register0);
                                frame->register0 = NULL;
                                break;
                        case BN_OP_PUSH_SELF: {
                                bnPushStack(frame->hierarcySelf,
                                            bnPopStack(frame->vStack));
                                break;
                        }
                        case BN_OP_POP_SELF: {
                                bnPopStack(frame->hierarcySelf);
                                break;
                        }
                        case BN_OP_SCOPE_INJECTION: {
                                bnObject* obj = bnPopStack(frame->vStack);
                                // collect all hidden variables
                                GHashTableIter hashIter;
                                g_hash_table_iter_init(&hashIter, obj->table);
                                gpointer k, v;
                                while (
                                    g_hash_table_iter_next(&hashIter, &k, &v)) {
                                        bnStringView kView = k;
                                        const char* kStr =
                                            bnView2Str(bone->pool, kView);
                                        if (g_str_has_prefix(kStr, "$$_")) {
                                                const char* origName = kStr + 3;
                                                bnStringView origView =
                                                    bnIntern(bone->pool,
                                                             origName);
                                                g_hash_table_replace(
                                                    frame->variableTable,
                                                    origView, v);
                                        }
                                        g_hash_table_iter_remove(&hashIter);
                                }
                                break;
                        }
                        case BN_OP_OBJECT_INJECTION: {
                                bnObject* src = bnPopStack(frame->vStack);
                                bnObject* dst = bnPopStack(frame->vStack);
                                // collect all hidden variables
                                GHashTableIter hashIter;
                                g_hash_table_iter_init(&hashIter, src->table);
                                gpointer k, v;
                                while (
                                    g_hash_table_iter_next(&hashIter, &k, &v)) {
                                        bnStringView kView = k;
                                        const char* kStr =
                                            bnView2Str(bone->pool, kView);
                                        if (g_str_has_prefix(kStr, "$$_")) {
                                                const char* origName = kStr + 3;
                                                bnStringView origView =
                                                    bnIntern(bone->pool,
                                                             origName);
                                                g_hash_table_replace(
                                                    dst->table, origView, v);
                                        }
                                        g_hash_table_iter_remove(&hashIter);
                                }
                                bnPushStack(frame->vStack, dst);
                                break;
                        }
                        case BN_OP_STORE: {
                                bnStringView name =
                                    g_ptr_array_index(env->codeArray, ++PC);
                                bnObject* value = bnPopStack(frame->vStack);
                                g_hash_table_insert(frame->variableTable,
                                                    GINT_TO_POINTER((int)name),
                                                    value);
                                assert(value != NULL);
                                break;
                        }
                        case BN_OP_LOAD: {
                                bnStringView name =
                                    g_ptr_array_index(env->codeArray, ++PC);
                                bnObject* value = g_hash_table_lookup(
                                    frame->variableTable,
                                    GINT_TO_POINTER((int)name));
                                assert(value != NULL);
                                bnPushStack(frame->vStack, value);
                                break;
                        }
                        case BN_OP_PUT: {
                                bnObject* container = bnPopStack(frame->vStack);
                                bnObject* value = bnPopStack(frame->vStack);
                                bnStringView name =
                                    g_ptr_array_index(env->codeArray, ++PC);
                                g_hash_table_insert(container->table, name,
                                                    value);
                                break;
                        }
                        case BN_OP_GET: {
                                bnObject* container = bnPopStack(frame->vStack);
                                bnStringView name =
                                    g_ptr_array_index(env->codeArray, ++PC);
                                gpointer data =
                                    g_hash_table_lookup(container->table, name);
                                bnObject* obj = data;
                                assert(data != NULL);
                                bnPushStack(frame->vStack, data);
                                break;
                        }
                        case BN_OP_FUNCCALL: {
                                bnObject* lambda = bnPopStack(frame->vStack);
                                int argc =
                                    g_ptr_array_index(env->codeArray, ++PC);
                                bnFuncCall(lambda, bone, frame, argc);
                                break;
                        }
                }
        }
        return 0;
}