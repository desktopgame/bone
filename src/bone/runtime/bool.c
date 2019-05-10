#include "bool.h"
#include "interpreter.h"
#include "keyword.h"
#include "lambda.h"
#include "std.h"

bnBool* bnNewBool(bnInterpreter* bone, bool value) {
        bnBool* ret = BN_MALLOC(sizeof(bnBool));
        bnInitObject(bone->heap, &ret->base, BN_OBJECT_BOOL);
        ret->value = value;
        ret->r = NULL;
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_NOT),
                 bnNewLambdaFromCFunc(bone, bnStdBoolNot, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_RETURN,
                                      "ret", BN_C_ADD_EXIT));
        return ret;
}
