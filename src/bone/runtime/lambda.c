#include "lambda.h"

bnLambda* bnNewLambda(bnLambdaType type) {
        bnLambda* ret = BN_MALLOC(sizeof(bnLambda));
        bnInitObject(&ret->base, BN_OBJECT_LAMBDA);
        ret->type = type;
        ret->outer =
            g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, NULL);
        return ret;
}

bnLambda* bnNewLambdaFromCFunc(bnNativeFunc func) {
        bnLambda* ret = bnNewLambda(BN_LAMBDA_NATIVE);
        ret->u.vFunc = func;
        return ret;
}

void bnDeleteLambda(bnLambda* self) {}