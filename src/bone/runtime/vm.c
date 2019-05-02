#include "vm.h"
#include "integer.h"
#include "opcode.h"

int bnExecute(bnInterpreter* bone, bnEnviroment* env, bnFrame* frame) {
        GList* iter = env->binary;
        int PC = 0;
        while (iter != NULL) {
                bnOpcode code = (bnOpcode)iter->data;
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
                                void* a = bnPopStack(frame->vStack);
                                void* b = bnPopStack(frame->vStack);
                                bnPushStack(frame->vStack, a);
                                bnPushStack(frame->vStack, b);
                                break;
                        }
                        case BN_OP_GEN_INT: {
                                iter = iter->next;
                                PC++;
                                int data = iter->data;
                                bnPushStack(frame->vStack,
                                            bnNewInteger(bone, data));
                                break;
                        }
                        case BN_OP_GEN_DOUBLE:
                                break;
                        case BN_OP_GEN_STRING:
                                break;
                        case BN_OP_STORE: {
                                iter = iter->next;
                                PC++;
                                bnStringView name = iter->data;
                                bnObject* value = bnPopStack(frame->vStack);
                                g_hash_table_insert(frame->variableTable,
                                                    GINT_TO_POINTER((int)name),
                                                    value);
                                break;
                        }
                        case BN_OP_LOAD: {
                                iter = iter->next;
                                PC++;
                                bnStringView name = iter->data;
                                bnObject* value = g_hash_table_lookup(
                                    frame->variableTable,
                                    GINT_TO_POINTER((int)name));
                                bnPushStack(frame->vStack, value);
                                break;
                        }
                        case BN_OP_PUT: {
                                break;
                        }
                        case BN_OP_GET: {
                                bnObject* container = bnPopStack(frame->vStack);
                                iter = iter->next;
                                PC++;
                                bnStringView name = iter->data;
                                gpointer data =
                                    g_hash_table_lookup(container->table, name);
                                assert(data != NULL);
                                bnPushStack(frame->vStack, data);
                                break;
                        }
                        case BN_OP_FUNCCALL: {
                                bnObject* lambda = bnPopStack(frame->vStack);
                                iter = iter->next;
                                PC++;
                                int argc = iter->data;
                                bnFuncCall(lambda, bone, frame, argc);
                                break;
                        }
                }
                // go to next code
                iter = iter->next;
                PC++;
        }
        return 0;
}