#ifndef BONE_RUNTIME_LAMBDA_H
#define BONE_RUNTIME_LAMBDA_H
#include "enviroment.h"
#include "object.h"

struct bnFrame;
typedef void (*bnNativeFunc)(struct bnFrame* frame);

typedef enum bnLambdaType {
        BN_LAMBDA_SCRIPT,
        BN_LAMBDA_NATIVE,
} bnLambdaType;

typedef struct bnLambda {
        bnObject base;
        bnLambdaType type;
        union {
                bnEnviroment* vEnv;
                bnNativeFunc vFunc;
        } u;
} bnLambda;

/**
 * return new instance of bnLambda.
 * @param type
 * @return
 */
bnLambda* bnNewLambda(bnLambdaType type);

/**
 * return new instance of bnLambda, from C function.
 * @param func
 * @return
 */
bnLambda* bnNewLambdaFromCFunc(bnNativeFunc func);

/**
 * return new instance of bnLambda.
 * @param name
 * @return
 */
void bnDeleteLambda(bnLambda* self);
#endif