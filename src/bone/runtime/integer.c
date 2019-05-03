#include "integer.h"
#include "keyword.h"
#include "lambda.h"
#include "std.h"

bnInteger* bnNewInteger(bnInterpreter* bone, int value) {
        bnInteger* ret = BN_MALLOC(sizeof(bnInteger));
        bnInitObject(&ret->base, BN_OBJECT_INTEGER);
        ret->value = value;
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_PLUS),
                 bnNewLambdaFromCFunc(bnIntegerPlus));
        return ret;
}

void bnDeleteInteger(bnInteger* self) { bnDeleteObject(self); }