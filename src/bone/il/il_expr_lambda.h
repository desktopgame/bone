#ifndef $INCLUDE
#define $INCLUDE
#include "../bone.h"

/**
 * bnILExprLambda is lambda.
 */
typedef struct bnILExprLambda {
        GList* parameters;
        GList* returns;
        GList* statements;
} bnILExprLambda;

/**
 * return new instance of bnILExprLambda.
 * @return
 */
bnILExprLambda* bnNewILExprLambda();

/**
 * print a information of bnILExprLambda.
 * @param fp
 * @param self
 * @param depth
 */
void bnDumpILExprLambda(FILE* fp, bnILExprLambda* self, int depth);

/**
 * free a bnILExprLambda.
 * @param self
 */
void bnDeleteILExprLambda(bnILExprLambda* self);
#endif
