#include "string.h"
#include "integer.h"
#include "keyword.h"
#include "lambda.h"
#include "std.h"

bnString* bnNewString(bnInterpreter* bone, bnStringView value) {
        bnString* ret = BN_MALLOC(sizeof(bnString));
        bnInitObject(bone, &ret->base, BN_OBJECT_STRING);
        ret->value = value;
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_EQUAL),
                 bnNewLambdaFromCFunc(bone, bnStdStringEqual, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_NOTEQUAL),
                 bnNewLambdaFromCFunc(bone, bnStdStringNotEqual, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, "length"),
                 bnNewInteger(bone, strlen(bnView2Str(bone->pool, value))));
        bnDefine(&ret->base, bnIntern(bone->pool, "at"),
                 bnNewLambdaFromCFunc(bone, bnStdStringAt, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "index", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        return ret;
}
