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
                                g_trash_stack_push(
                                    &frame->stack,
                                    g_trash_stack_peek(&frame->stack));
                                break;
                        }
                        case BN_OP_SWAP: {
                                gpointer a = g_trash_stack_pop(&frame->stack);
                                gpointer b = g_trash_stack_pop(&frame->stack);
                                g_trash_stack_push(&frame->stack, a);
                                g_trash_stack_push(&frame->stack, b);
                        }
                        case BN_OP_GEN_INT: {
                                iter = iter->next;
                                PC++;
                                int data = iter->data;
                                g_trash_stack_push(&frame->stack,
                                                   bnNewInteger(data));
                                break;
                        }
                        case BN_OP_GEN_DOUBLE:
                                break;
                        case BN_OP_GEN_STRING:
                                break;
                        case BN_OP_STORE: {
                                break;
                        }
                        case BN_OP_LOAD: {
                                break;
                        }
                        case BN_OP_PUT: {
                                break;
                        }
                        case BN_OP_GET: {
                                bnObject* container =
                                    g_trash_stack_pop(&frame->stack);
                                iter = iter->next;
                                PC++;
                                bnStringView name = iter->data;
                                gpointer data =
                                    g_hash_table_lookup(container->table, name);
                                g_trash_stack_push(&frame->stack, data);
                                break;
                        }
                        case BN_OP_FUNCCALL: {
                                bnObject* lambda =
                                    g_trash_stack_pop(&frame->stack);
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