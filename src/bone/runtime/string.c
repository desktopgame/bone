#include "string.h"
#include "keyword.h"
#include "lambda.h"
#include "std.h"

bnString* bnNewString(bnInterpreter* bone, bnStringView value) {
        bnString* ret = BN_MALLOC(sizeof(bnString));
        bnInitObject(&ret->base, BN_OBJECT_STRING);
        ret->value = value;
        bnDefine(
            &ret->base, bnIntern(bone->pool, BN_KWD_EQUAL),
            bnNewLambdaFromCFunc(bnStdStringEqual, bone->pool, BN_C_ADD_PARAM,
                                 "self", BN_C_ADD_PARAM, "other",
                                 BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        return ret;
}

void bnDeleteString(bnString* self) {}