#ifndef BONE_RUNTIME_LAMBDA_H
#define BONE_RUNTIME_LAMBDA_H
#include <stdarg.h>
#include <stdbool.h>
#include "enviroment.h"
#include "object.h"

struct bnFrame;
struct bnInterpreter;
typedef void (*bnNativeFunc)(struct bnInterpreter* bone, struct bnFrame* frame);

#define BN_C_ADD_PARAM (0)
#define BN_C_ADD_RETURN (1)
#define BN_C_ADD_EXIT (2)

/**
 * bnLambdaType is type of bnLambda.
 */
typedef enum bnLambdaType {
        BN_LAMBDA_SCRIPT,
        BN_LAMBDA_NATIVE,
} bnLambdaType;

#define bnNewLambda(bone, type) \
        (bnNewLambdaFunc(bone, type, __FILE__, __LINE__))
/**
 * return new instance of bnLambda.
 * @param bone
 * @param type
 * @param filename
 * @param lineno
 * @return
 */
bnReference bnNewLambdaFunc(struct bnInterpreter* bone, bnLambdaType type,
                            const char* filename, int lineno);

#define bnNewLambdaFromCFunc(bone, func, pool, ...)                     \
        (bnNewLambdaFromCFuncFunc(bone, func, pool, __FILE__, __LINE__, \
                                  __VA_ARGS__))
/**
 * return new instance of bnLambda, from C function.
 * @param bone
 * @param func
 * @param pool
 * @param filename
 * @param lineno
 * @param ... (parameter, named returns)
 * @return
 */
bnReference bnNewLambdaFromCFuncFunc(struct bnInterpreter* bone,
                                     bnNativeFunc func,
                                     struct bnStringPool* pool,
                                     const char* filename, int lineno, ...);

/**
 * return true, if first parameter name is self.
 * @param pool
 * @param self
 * @return
 */
bool bnIsInstanceBaseLambda(struct bnStringPool* pool, bnObject* self);

/**
 * return true, if variadic return value.
 * @param pool
 * @param self
 * @return
 */
bool bnIsVariadicReturn(struct bnStringPool* pool, bnObject* self);

/**
 * @param obj
 * @param env
 */
void bnSetEnviroment(bnObject* obj, bnEnviroment* env);

/**
 * @param obj
 * @return
 */
bnEnviroment* bnGetEnviroment(bnObject* obj);

/**
 * @param obj
 * @return
 */
bnNativeFunc bnGetNativeFunc(bnObject* obj);

/**
 * @param obj
 * @param param
 */
void bnAddParameter(bnObject* obj, gpointer param);

/**
 * @param obj
 * @return
 */
GList* bnGetParameterList(bnObject* obj);

/**
 * @param obj
 * @param retval
 */
void bnAddReturnValue(bnObject* obj, gpointer retval);

/**
 * @param obj
 * @return
 */
GList* bnGetReturnValueList(bnObject* obj);

/**
 * @param obj
 * @return
 */
GHashTable* bnGetCapturedMap(bnObject* obj);

/**
 * @param obj
 * @param filename
 */
void bnSetLambdaFileName(bnObject* obj, bnStringView filename);

/**
 * @param obj
 * @return
 */
bnStringView bnGetLambdaFileName(bnObject* obj);

/**
 * @param obj
 * @param line
 */
void bnSetLambdaLineNumber(bnObject* obj, int line);

/**
 * @param obj
 * @return
 */
int bnGetLambdaLineNumber(bnObject* obj);

/**
 * @param obj
 * @return
 */
bnLambdaType bnGetLambdaType(bnObject* obj);
#endif