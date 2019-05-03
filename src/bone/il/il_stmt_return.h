#ifndef BONE_IL_ILSTMTRETURN_H
#define BONE_IL_ILSTMTRETURN_H
#include "../bone.h"
#include "il_expression.h"

/**
 * bnILStmtReturn is return statement.
 */
typedef struct bnILStmtReturn {
        bnILExpression* expr;
} bnILStmtReturn;

/**
 * return new instance of bnILStmtReturn.
 * @param expr
 * @return
 */
bnILStmtReturn* bnNewILStmtReturn(bnILExpression* expr);

/**
 * print a information of bnILStmtReturn.
 * @param fp
 * @param self
 * @param depth
 */
void bnDumpILStmtReturn(FILE* fp, struct bnStringPool* pool,
                        bnILStmtReturn* self, int depth);

void bnGenerateILStmtReturn(struct bnInterpreter* bone, bnILStmtReturn* self,
                            struct bnEnviroment* env);

/**
 * free a bnILStmtReturn.
 * @param self
 */
void bnDeleteILStmtReturn(bnILStmtReturn* self);
#endif
