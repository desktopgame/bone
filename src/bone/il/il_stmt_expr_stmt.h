#ifndef BONE_IL_ILSTMTEXPR_H
#define BONE_IL_ILSTMTEXPR_H
#include "../bone.h"
#include "il_expression.h"

/**
 * bnILStmtExpr is statement a structured by only expression.
 */
typedef struct bnILStmtExpr {
        bnILExpression* expr;
} bnILStmtExpr;

/**
 * return new instance of bnILStmtExpr.
 * @param expr
 * @return
 */
bnILStmtExpr* bnNewILStmtExpr(bnILExpression* expr);

/**
 * print a information of bnILStmtExpr.
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILStmtExpr(FILE* fp, struct bnStringPool* pool, bnILStmtExpr* self,
                      int depth);

void bnGenerateILStmtExpr(struct bnInterpreter* bone, bnILStmtExpr* self,
                          struct bnEnviroment* env);

/**
 * free a bnILStmtExpr.
 * @param self
 */
void bnDeleteILStmtExpr(bnILStmtExpr* self);
#endif