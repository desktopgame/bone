#include "vm.h"
#include "opcode.h"

int bnExecute(bnEnviroment* env, bnFrame* frame) {
        GList* iter = env->binary;
        int PC = 0;
        while (iter != NULL) {
                bnOpcode code = (bnOpcode)iter->data;
                switch (code) {
                        case BN_OP_NOP:
                                break;
                        case BN_OP_GEN_INT:
                                break;
                        case BN_OP_GEN_DOUBLE:
                                break;
                        case BN_OP_GEN_STRING:
                                break;
                        case BN_OP_STORE:
                                break;
                        case BN_OP_LOAD:
                                break;
                        case BN_OP_PUT:
                                break;
                        case BN_OP_GET:
                                break;
                        case BN_OP_FUNCCALL:
                                break;
                        case BN_OP_POSITIVE:
                                break;
                        case BN_OP_NEGATIVE:
                                break;
                        case BN_OP_CHILDA:
                                break;
                        case BN_OP_NOT:
                                break;
                        case BN_OP_PLUS:
                                break;
                        case BN_OP_MINUS:
                                break;
                        case BN_OP_MULTIPLY:
                                break;
                        case BN_OP_DIVIDE:
                                break;
                        case BN_OP_MODULO:
                                break;
                        case BN_OP_BIT_AND:
                                break;
                        case BN_OP_BIT_OR:
                                break;
                        case BN_OP_LOGIC_AND:
                                break;
                        case BN_OP_LOGIC_OR:
                                break;
                        case BN_OP_EXC_OR:
                                break;
                        case BN_OP_LSHIFT:
                                break;
                        case BN_OP_RSHIFT:
                                break;
                        case BN_OP_GT:
                                break;
                        case BN_OP_GE:
                                break;
                        case BN_OP_LT:
                                break;
                        case BN_OP_EQUAL:
                                break;
                        case BN_OP_NOTEQUAL:
                                break;
                }
                // go to next code
                iter = iter->next;
                PC++;
        }
        return 0;
}