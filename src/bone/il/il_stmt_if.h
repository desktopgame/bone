#ifndef BONE_IL_ILSTMTIF_H
#define BONE_IL_ILSTMTIF_H
#include "../bone.h"
#include "il_expression.h"

/**
 * bnILStmtIf is if statement.
 */
typedef struct bnILStmtIf {
        bnILExpression* cond;
        GList* statements;
} bnILStmtIf;

/**
 * return new instance of bnILStmtIf.
 * @param cond
 * @return
 */
bnILStmtIf* bnNewILStmtIf(bnILExpression* cond);

/**
 * free a bnILStmtIf.
 * @param self
 */
void bnDeleteILStmtIf(bnILStmtIf* self);

/**
 * bnILStmtIfElse is if statement with else.
 */
typedef struct bnILStmtIfElse {
        bnILStmtIf* trueCase;
        GList* statements;
} bnILStmtIfElse;

/**
 * return new instance of bnILStmtIf.
 * @param trueCase
 * @return
 */
bnILStmtIfElse* bnNewILStmtIfElse(bnILStmtIf* trueCase);

/**
 * free a bnILStmtIf.
 * @param self
 */
void bnDeleteILStmtIfElse(bnILStmtIfElse* self);
#endif