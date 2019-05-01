#ifndef BONE_RUNTIME_LAMBDA_H
#define BONE_RUNTIME_LAMBDA_H
#include "enviroment.h"
#include "object.h"

typedef struct bnLambda {
        bnObject base;
        bnEnviroment* env;
} bnLambda;

/**
 * return new instance of bnLambda.
 * @param name
 * @return
 */
bnLambda* bnNewLambda();

/**
 * return new instance of bnLambda.
 * @param name
 * @return
 */
void bnDeleteLambda(bnLambda* self);
#endif