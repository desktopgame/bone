#ifndef BONE_IL_ILEXPROBJECTINJECTION_H
#define BONE_IL_ILEXPROBJECTINJECTION_H
#include "../bone.h"
#include "il_expression.h"

/**
 * bnILExprObjectInjection is injection.
 */
typedef struct bnILExprObjectInjection {
        bnILExpression* leftExpr;
        bnILExpression* rightExpr;
} bnILExprObjectInjection;

/**
 * return new instance of bnILExprObjectInjection.
 * @param leftExpr
 * @param rightExpr
 * @return
 */
bnILExprObjectInjection* bnNewILExprObjectInjection(bnILExpression* leftExpr,
                                                    bnILExpression* rightExpr);

/**
 * print a information of bnILExprObjectInjection.
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprObjectInjection(FILE* fp, struct bnStringPool* pool,
                                 bnILExprObjectInjection* self, int depth);

void bnGenerateILExprObjectInjection(struct bnInterpreter* bone,
                                     bnILExprObjectInjection* self,
                                     struct bnEnviroment* env);

/**
 * free a bnILExprObjectInjection.
 * @param self
 */
void bnDeleteILExprObjectInjection(bnILExprObjectInjection* self);
#endif
