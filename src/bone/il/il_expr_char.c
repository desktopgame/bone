#include "il_expr_char.h"

bnILExprChar* bnNewILExprDouble(char value) {
        bnILExprChar* ret = BN_MALLOC(sizeof(bnILExprChar));
        ret->value = value;
        return ret;
}

void bnDumpILExprChar(FILE* fp, bnILExprChar* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "%c\n");
}

void bnDeleteILExprDouble(bnILExprChar* self) { BN_FREE(self); }