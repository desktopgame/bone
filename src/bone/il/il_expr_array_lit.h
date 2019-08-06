#ifndef BONE_IL_ILEXPRLIT_H
#define BONE_IL_ILEXPRLIT_H
#include "../bone.h"
#include "il_expression.h"

/**
 * bnILExprArrayLit is {...}.
 */
typedef struct bnILExprArrayLit {
        GPtrArray* arrayLit_expressions;
} bnILExprArrayLit;

/**
 * return new instance of bnILExprArrayLit.
 * @return
 */
bnILExprArrayLit* bnNewILExprArrayLit();

/**
 * print a information of bnILExprArrayLit.
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprArrayLit(FILE* fp, struct bnStringPool* pool,
                          bnILExprArrayLit* self, int depth);

void bnGenerateILExprArrayLit(struct bnInterpreter* bone,
                              bnILExprArrayLit* self, struct bnEnviroment* env,
                              bnCompileCache* ccache);

/**
 * free a bnILExprArrayLit.
 * @param self
 */
void bnDeleteILExprArrayLit(bnILExprArrayLit* self);
#endif