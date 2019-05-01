#include "lambda.h"

bnLambda* bnNewLambda() {
        bnLambda* ret = BN_MALLOC(sizeof(bnLambda));
        bnInitObject(&ret->base);
        ret->env = bnNewEnviroment();
        return ret;
}

void bnDeleteLambda(bnLambda* self) {}