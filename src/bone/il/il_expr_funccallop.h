#ifndef BONE_IL_ILEXPRFUNCCALLOP_H
#define BONE_IL_ILEXPRFUNCCALLOP_H
#include "../bone.h"
#include "il_expression.h"
struct bnEnviroment;

/**
 * bnILExprFuncCallOp is func call.
 */
typedef struct bnILExprFuncCallOp {
        bnILExpression* expr;
        GList* arguments;
} bnILExprFuncCallOp;

/**
 * return new instance of bnILExprFuncCallOp.
 * @param expr
 * @return
 */
bnILExprFuncCallOp* bnNewILExprFuncCallOp(bnILExpression* expr);

/**
 * print a information of bnILExprFuncCallOp.
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprFuncCallOp(FILE* fp, struct bnStringPool* pool,
                            bnILExprFuncCallOp* self, int depth);

void bnGenerateILExprFuncCallOp(bnILExprFuncCallOp* self,
                                struct bnEnviroment* env);

/**
 * free a bnILExprFuncCallOp.
 * @param self
 */
void bnDeleteILExprFuncCallOp(bnILExprFuncCallOp* self);
#endif
