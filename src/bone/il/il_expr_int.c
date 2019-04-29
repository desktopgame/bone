#include "il_expr_int.h"

bnILExprInt* bnNewILExprInt(int value) {
        bnILExprInt* ret = BN_MALLOC(sizeof(bnILExprInt));
        ret->value = value;
        return ret;
}

void bnDeleteILExprInt(bnILExprInt* self) { BN_FREE(self); }