#include "il_expr_char.h"

bnILExprChar* bnNewILExprChar(char value) {
        bnILExprChar* ret = BN_MALLOC(sizeof(bnILExprChar));
        ret->value = value;
        return ret;
}

void bnDumpILExprChar(FILE* fp, struct bnStringPool* pool, bnILExprChar* self,
                      int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "char %c\n", self->value);
}

void bnDeleteILExprChar(bnILExprChar* self) { BN_FREE(self); }