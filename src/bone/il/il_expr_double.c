#include "il_expr_double.h"

bnILExprDouble* bnNewILExprDouble(double value) {
        bnILExprDouble* ret = BN_MALLOC(sizeof(bnILExprDouble));
        ret->value = value;
        return ret;
}

void bnDeleteILExprDouble(bnILExprDouble* self) { BN_FREE(self); }