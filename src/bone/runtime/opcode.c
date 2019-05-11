#include "opcode.h"
#include "label.h"

int bnPrintOpcode(FILE* fp, struct bnStringPool* pool, GPtrArray* ary,
                  int pos) {
        bnOpcode data = (bnOpcode)g_ptr_array_index(ary, pos);
        fprintf(fp, "<%d>", pos);
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
                case BN_OP_GEN_INT: {
                        int num = g_ptr_array_index(ary, ++pos);
                        fprintf(fp, "gen int(%d)", num);
                        break;
                }
                case BN_OP_GEN_DOUBLE:
                        fprintf(fp, "gen double");
                        break;
                case BN_OP_GEN_CHAR: {
                        char c = g_ptr_array_index(ary, ++pos);
                        fprintf(fp, "gen char(%c)", c);
                        break;
                }
                case BN_OP_GEN_STRING: {
                        bnStringView str = g_ptr_array_index(ary, ++pos);
                        fprintf(fp, "gen string(%s)", bnView2Str(pool, str));
                        break;
                }
                case BN_OP_GEN_ARRAY: {
                        int size = g_ptr_array_index(ary, ++pos);
                        fprintf(fp, "gen array(%d)", size);
                        break;
                }
                case BN_OP_GEN_LAMBDA_BEGIN: {
                        int parameterLen = g_ptr_array_index(ary, ++pos);
                        pos += parameterLen;
                        int returnLen = g_ptr_array_index(ary, ++pos);
                        fprintf(fp, "gen lambda-begin((%d) <-(%d))",
                                parameterLen, returnLen);
                        pos += returnLen;
                        break;
                }
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
                case BN_OP_SCOPE_INJECTION:
                        fprintf(fp, "scope injection");
                        break;
                case BN_OP_OBJECT_INJECTION:
                        fprintf(fp, "object injection");
                        break;
                case BN_OP_STORE: {
                        bnStringView name = g_ptr_array_index(ary, ++pos);
                        fprintf(fp, "store %s", bnView2Str(pool, name));
                        break;
                }
                case BN_OP_LOAD: {
                        bnStringView name = g_ptr_array_index(ary, ++pos);
                        fprintf(fp, "load %s", bnView2Str(pool, name));
                        break;
                }
                case BN_OP_PUT: {
                        bnStringView name = g_ptr_array_index(ary, ++pos);
                        fprintf(fp, "put %s", bnView2Str(pool, name));
                        break;
                }
                case BN_OP_GET: {
                        bnStringView name = g_ptr_array_index(ary, ++pos);
                        fprintf(fp, "get %s", bnView2Str(pool, name));
                        break;
                }
                case BN_OP_GOTO: {
                        bnLabel* jmp = g_ptr_array_index(ary, ++pos);
                        fprintf(fp, "goto %d", jmp->pos);
                        break;
                }
                case BN_OP_GOTO_IF: {
                        bnLabel* jmp = g_ptr_array_index(ary, ++pos);
                        fprintf(fp, "goto if %d", jmp->pos);
                        break;
                }
                case BN_OP_GOTO_ELSE: {
                        bnLabel* jmp = g_ptr_array_index(ary, ++pos);
                        fprintf(fp, "goto else %d", jmp->pos);
                        break;
                }
                case BN_OP_RETURN: {
                        fprintf(fp, "return");
                        break;
                }
                case BN_OP_FUNCCALL: {
                        int argc = (int)g_ptr_array_index(ary, ++pos);
                        fprintf(fp, "funccall %d", argc);
                        break;
                }
                case BN_OP_DEFER_PUSH: {
                        bnLabel* jmp = g_ptr_array_index(ary, ++pos);
                        fprintf(fp, "defer push %d", jmp->pos);
                        break;
                }
                case BN_OP_DEFER_BEGIN: {
                        fprintf(fp, "defer begin");
                        break;
                }
                case BN_OP_DEFER_NEXT: {
                        fprintf(fp, "defer next");
                        break;
                }
                case BN_OP_DEFER_END: {
                        fprintf(fp, "defer end");
                        break;
                }
        }
        return pos + 1;
}

int bnOperands(bnOpcode data) {
        if (data == BN_OP_STORE || data == BN_OP_LOAD || data == BN_OP_PUT ||
            data == BN_OP_GET || data == BN_OP_GEN_INT ||
            data == BN_OP_GEN_STRING || data == BN_OP_GOTO ||
            data == BN_OP_GOTO_IF || data == BN_OP_GOTO_ELSE ||
            data == BN_OP_FUNCCALL || data == BN_OP_GEN_CHAR ||
            data == BN_OP_DEFER_PUSH) {
                return 1;
        }
        return 0;
}