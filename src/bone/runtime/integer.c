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
            bnNewLambdaFromCFunc(bnStdIntegerPlus, bone->pool, BN_C_ADD_PARAM,
                                 "self", BN_C_ADD_PARAM, "other",
                                 BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        bnDefine(
            &ret->base, bnIntern(bone->pool, BN_KWD_MINUS),
            bnNewLambdaFromCFunc(bnStdIntegerMinus, bone->pool, BN_C_ADD_PARAM,
                                 "self", BN_C_ADD_PARAM, "other",
                                 BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_MULTIPLY),
                 bnNewLambdaFromCFunc(bnStdIntegerMultiply, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(
            &ret->base, bnIntern(bone->pool, BN_KWD_DIVIDE),
            bnNewLambdaFromCFunc(bnStdIntegerDivide, bone->pool, BN_C_ADD_PARAM,
                                 "self", BN_C_ADD_PARAM, "other",
                                 BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        bnDefine(
            &ret->base, bnIntern(bone->pool, BN_KWD_MODULO),
            bnNewLambdaFromCFunc(bnStdIntegerModulo, bone->pool, BN_C_ADD_PARAM,
                                 "self", BN_C_ADD_PARAM, "other",
                                 BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        bnDefine(
            &ret->base, bnIntern(bone->pool, BN_KWD_LT),
            bnNewLambdaFromCFunc(bnStdIntegerLT, bone->pool, BN_C_ADD_PARAM,
                                 "self", BN_C_ADD_PARAM, "other",
                                 BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        bnDefine(
            &ret->base, bnIntern(bone->pool, BN_KWD_LE),
            bnNewLambdaFromCFunc(bnStdIntegerLE, bone->pool, BN_C_ADD_PARAM,
                                 "self", BN_C_ADD_PARAM, "other",
                                 BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        bnDefine(
            &ret->base, bnIntern(bone->pool, BN_KWD_GT),
            bnNewLambdaFromCFunc(bnStdIntegerGT, bone->pool, BN_C_ADD_PARAM,
                                 "self", BN_C_ADD_PARAM, "other",
                                 BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        bnDefine(
            &ret->base, bnIntern(bone->pool, BN_KWD_GE),
            bnNewLambdaFromCFunc(bnStdIntegerGE, bone->pool, BN_C_ADD_PARAM,
                                 "self", BN_C_ADD_PARAM, "other",
                                 BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        bnDefine(
            &ret->base, bnIntern(bone->pool, BN_KWD_EQUAL),
            bnNewLambdaFromCFunc(bnStdIntegerEqual, bone->pool, BN_C_ADD_PARAM,
                                 "self", BN_C_ADD_PARAM, "other",
                                 BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_NOTEQUAL),
                 bnNewLambdaFromCFunc(bnStdIntegerNotEqual, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, "toString"),
                 bnNewLambdaFromCFunc(bnStdIntegerToString, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        return ret;
}

void bnDeleteInteger(bnInteger* self) { bnDeleteObject(self); }