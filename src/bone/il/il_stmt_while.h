#ifndef BONE_IL_ILSTMTWHILE_H
#define BONE_IL_ILSTMTWHILE_H
#include "../bone.h"
#include "il_expression.h"

/**
 * bnILStmtWhile is while statement.
 */
typedef struct bnILStmtWhile {
        bnILExpression* cond;
        GList* statements;
} bnILStmtWhile;

/**
 * return new instance of bnILStmtWhile.
 * @param cond
 * @return
 */
bnILStmtWhile* bnNewILStmtWhile(bnILExpression* cond);

/**
 * print a information of bnILStmtWhile.
 * @param fp
 * @param self
 * @param depth
 */
void bnDumpILStmtWhile(FILE* fp, bnILStmtWhile* self, int depth);

/**
 * free a bnILStmtWhile.
 * @param self
 */
void bnDeleteILStmtWhile(bnILStmtWhile* self);
#endif