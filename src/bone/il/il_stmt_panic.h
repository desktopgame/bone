#ifndef BONE_IL_ILSTMTPANIC_H
#define BONE_IL_ILSTMTPANIC_H
#include "../bone.h"
#include "il_expression.h"

/**
 * bnILStmtPanic is panic statement.
 */
typedef struct bnILStmtPanic {
        bnStringView name;
        bnILExpression* expr;
} bnILStmtPanic;

/**
 * return new instance of bnILStmtPanic.
 * @param name
 * @param expr
 * @return
 */
bnILStmtPanic* bnNewILStmtPanic(bnStringView name, bnILExpression* expr);

/**
 * print a information of bnILStmtPanic.
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILStmtPanic(FILE* fp, struct bnStringPool* pool, bnILStmtPanic* self,
                       int depth);

void bnGenerateILStmtPanic(struct bnInterpreter* bone, bnILStmtPanic* self,
                           struct bnEnviroment* env);

/**
 * free a bnILStmtPanic.
 * @param self
 */
void bnDeleteILStmtPanic(bnILStmtPanic* self);

#endif