#include "lambda.h"

bnLambda* bnNewLambda(bnLambdaType type) {
        bnLambda* ret = BN_MALLOC(sizeof(bnLambda));
        bnInitObject(&ret->base, BN_OBJECT_LAMBDA);
        ret->type = type;
        return ret;
}

bnLambda* bnNewLambdaFromCFunc(bnNativeFunc func) {
        bnLambda* ret = bnNewLambda(BN_LAMBDA_NATIVE);
        ret->u.vFunc = func;
        return ret;
}

void bnDeleteLambda(bnLambda* self) {}