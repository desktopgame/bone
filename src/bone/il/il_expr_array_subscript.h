#ifndef BONE_IL_ILEXPRARRAYSUBSCRIPT_H
#define BONE_IL_ILEXPRARRAYSUBSCRIPT_H
#include "../bone.h"
#include "il_expression.h"

/**
 * bnILExprArraySubscript is array[index].
 */
typedef struct bnILExprArraySubscript {
        bnILExpression* arrayExpr;
        bnILExpression* indexExpr;
} bnILExprArraySubscript;

/**
 * return new instance of bnILExprArraySubscript.
 * @param arrayExpr
 * @param indexExpr
 * @return
 */
bnILExprArraySubscript* bnNewILExprArraySubscript(bnILExpression* arrayExpr,
                                                  bnILExpression* indexExpr);

/**
 * print a information of bnILExprArraySubscript.
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprArraySubscript(FILE* fp, struct bnStringPool* pool,
                                bnILExprArraySubscript* self, int depth);

void bnGenerateILExprArraySubscript(struct bnInterpreter* bone,
                                    bnILExprArraySubscript* self,
                                    struct bnEnviroment* env,
                                    bnCompileCache* ccache);

/**
 * free a bnILExprArraySubscript.
 * @param self
 */
void bnDeleteILExprArraySubscript(bnILExprArraySubscript* self);
#endif
