#ifndef BONE_RUNTIME_LAMBDA_H
#define BONE_RUNTIME_LAMBDA_H
#include <stdarg.h>
#include <stdbool.h>
#include "enviroment.h"
#include "object.h"

struct bnFrame;
struct bnInterpreter;
typedef void (*bnNativeFunc)(struct bnInterpreter* bone, struct bnFrame* frame);

typedef enum bnLambdaType {
        BN_LAMBDA_SCRIPT,
        BN_LAMBDA_NATIVE,
} bnLambdaType;

typedef struct bnLambda {
        bnObject base;
        bnLambdaType type;
        GHashTable* outer;
        GList* returns;
        bool instanceBase;
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
 * @param pool
 * @param ... (named returns)
 * @return
 */
bnLambda* bnNewLambdaFromCFunc(bnNativeFunc func, struct bnStringPool* pool,
                               ...);

/**
 * return new instance of bnLambda.
 * @param name
 * @return
 */
void bnDeleteLambda(bnLambda* self);
#endif