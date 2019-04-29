#include "il_expr_string.h"

bnILExprString* bnNewILExprString(GString* value) {
        bnILExprString* ret = BN_MALLOC(sizeof(bnILExprString));
        ret->value = value;
        return ret;
}

void bnDeleteILExprString(bnILExprString* self) { BN_FREE(self); }