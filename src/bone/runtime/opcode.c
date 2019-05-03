#include "opcode.h"

GList* bnPrintOpcode(FILE* fp, struct bnStringPool* pool, GList* list) {
        bnOpcode data = list->data;
        switch (data) {
                case BN_OP_NOP:
                        fprintf(fp, "nop");
                        break;
                case BN_OP_DUP:
                        fprintf(fp, "dup");
                        break;
                case BN_OP_SWAP:
                        fprintf(fp, "swap");
                        break;
                case BN_OP_GEN_INT:
                        list = list->next;
                        int num = list->data;
                        fprintf(fp, "gen int(%d)", num);
                        break;
                case BN_OP_GEN_DOUBLE:
                        fprintf(fp, "gen double");
                        break;
                case BN_OP_GEN_STRING:
                        list = list->next;
                        bnStringView str = list->data;
                        fprintf(fp, "gen string(%s)", bnView2Str(pool, str));
                        break;
                case BN_OP_GEN_LAMBDA_BEGIN:
                        list = list->next;
                        bool isInstanceBase = list->data;
                        fprintf(fp, "gen lambda-begin(%s)",
                                isInstanceBase ? "true" : "false");
                        break;
                case BN_OP_GEN_LAMBDA_END:
                        fprintf(fp, "gen lambda-end");
                        break;
                case BN_OP_SET_REGISTER_0:
                        fprintf(fp, "set register[0]");
                        break;
                case BN_OP_GET_REGISTER_0:
                        fprintf(fp, "get register[0]");
                        break;
                case BN_OP_PUSH_SELF:
                        fprintf(fp, "push");
                        break;
                case BN_OP_POP_SELF:
                        fprintf(fp, "pop");
                        break;
                case BN_OP_STORE: {
                        list = list->next;
                        bnStringView name = list->data;
                        fprintf(fp, "store %s", bnView2Str(pool, name));
                        break;
                }
                case BN_OP_LOAD: {
                        list = list->next;
                        bnStringView name = list->data;
                        fprintf(fp, "load %s", bnView2Str(pool, name));
                        break;
                }
                case BN_OP_PUT:
                        list = list->next;
                        bnStringView name = list->data;
                        fprintf(fp, "put %s", bnView2Str(pool, name));
                        break;
                case BN_OP_GET: {
                        list = list->next;
                        bnStringView name = list->data;
                        fprintf(fp, "get %s", bnView2Str(pool, name));
                        break;
                }
                case BN_OP_FUNCCALL:
                        list = list->next;
                        int argc = list->data;
                        fprintf(fp, "funccall %d", argc);
                        break;
        }
        if (list == NULL) {
                return list;
        }
        return list->next;
}