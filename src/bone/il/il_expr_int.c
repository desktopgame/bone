#include "il_expr_int.h"

bnILExprInt* bnNewILExprInt(int value) {
        bnILExprInt* ret = BN_MALLOC(sizeof(bnILExprInt));
        ret->value = value;
        return ret;
}

void bnDumpILExprInt(FILE* fp, struct bnStringPool* pool, bnILExprInt* self,
                     int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "%d\n", self->value);
}

void bnDeleteILExprInt(bnILExprInt* self) { BN_FREE(self); }