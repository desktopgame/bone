#ifndef BONE_IL_ILSTMTSCOPEINJECTION_H
#define BONE_IL_ILSTMTSCOPEINJECTION_H
#include "../bone.h"
#include "il_expression.h"

/**
 * bnILStmtScopeInjection is scope injection.
 */
typedef struct bnILStmtScopeInjection {
        bnILExpression* expr;
} bnILStmtScopeInjection;

/**
 * return new instance of bnILStmtScopeInjection.
 * @param expr
 * @return
 */
bnILStmtScopeInjection* bnNewILStmtScopeInjection(bnILExpression* expr);

/**
 * print a information of bnILStmtScopeInjection.
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILStmtScopeInjection(FILE* fp, struct bnStringPool* pool,
                                bnILStmtScopeInjection* self, int depth);

void bnGenerateILStmtScopeInjection(struct bnInterpreter* bone,
                                    bnILStmtScopeInjection* self,
                                    struct bnEnviroment* env);

/**
 * free a bnILStmtScopeInjection.
 * @param self
 */
void bnDeleteILStmtScopeInjection(bnILStmtScopeInjection* self);
#endif
