#ifndef BONE_IL_ILEXPRFUNCCALLOP_H
#define BONE_IL_ILEXPRFUNCCALLOP_H
#include "../bone.h"
#include "il_expression.h"

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
 * @param self
 * @param depth
 */
void bnDumpILExprFuncCallOp(FILE* fp, bnILExprFuncCallOp* self, int depth);

/**
 * free a bnILExprFuncCallOp.
 * @param self
 */
void bnDeleteILExprFuncCallOp(bnILExprFuncCallOp* self);
#endif
