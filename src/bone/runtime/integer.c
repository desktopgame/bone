#include "integer.h"
#include "keyword.h"
#include "lambda.h"
#include "std.h"

bnInteger* bnNewInteger(bnInterpreter* bone, int value) {
        bnInteger* ret = BN_MALLOC(sizeof(bnInteger));
        bnInitObject(&ret->base, BN_OBJECT_INTEGER);
        ret->value = value;
        bnDefine(
            &ret->base, bnIntern(bone->pool, BN_KWD_PLUS),
            bnNewLambdaFromCFunc(bnStdIntegerPlus, bone->pool, "ret", NULL));
        bnDefine(
            &ret->base, bnIntern(bone->pool, BN_KWD_MINUS),
            bnNewLambdaFromCFunc(bnStdIntegerMinus, bone->pool, "ret", NULL));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_MULTIPLY),
                 bnNewLambdaFromCFunc(bnStdIntegerMultiply, bone->pool, "ret",
                                      NULL));
        bnDefine(
            &ret->base, bnIntern(bone->pool, BN_KWD_DIVIDE),
            bnNewLambdaFromCFunc(bnStdIntegerDivide, bone->pool, "ret", NULL));
        bnDefine(
            &ret->base, bnIntern(bone->pool, BN_KWD_MODULO),
            bnNewLambdaFromCFunc(bnStdIntegerModulo, bone->pool, "ret", NULL));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_LT),
                 bnNewLambdaFromCFunc(bnStdIntegerLT, bone->pool, "ret", NULL));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_LE),
                 bnNewLambdaFromCFunc(bnStdIntegerLE, bone->pool, "ret", NULL));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_GT),
                 bnNewLambdaFromCFunc(bnStdIntegerGT, bone->pool, "ret", NULL));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_GE),
                 bnNewLambdaFromCFunc(bnStdIntegerGE, bone->pool, "ret", NULL));
        bnDefine(
            &ret->base, bnIntern(bone->pool, BN_KWD_EQUAL),
            bnNewLambdaFromCFunc(bnStdIntegerEqual, bone->pool, "ret", NULL));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_NOTEQUAL),
                 bnNewLambdaFromCFunc(bnStdIntegerNotEqual, bone->pool, "ret",
                                      NULL));
        bnDefine(&ret->base, bnIntern(bone->pool, "toString"),
                 bnNewLambdaFromCFunc(bnStdIntegerToString, bone->pool, "ret",
                                      NULL));
        return ret;
}

void bnDeleteInteger(bnInteger* self) { bnDeleteObject(self); }