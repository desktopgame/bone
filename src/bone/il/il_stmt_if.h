#ifndef BONE_IL_ILSTMTIF_H
#define BONE_IL_ILSTMTIF_H
#include "../bone.h"
#include "il_expression.h"

/**
 * bnILStmtIf is if statement.
 */
typedef struct bnILStmtIf {
        bnILExpression* cond;
        GPtrArray* statements;
} bnILStmtIf;

/**
 * return new instance of bnILStmtIf.
 * @param cond
 * @return
 */
bnILStmtIf* bnNewILStmtIf(bnILExpression* cond);

/**
 * print a information of bnILStmtIf.
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILStmtIf(FILE* fp, struct bnStringPool* pool, bnILStmtIf* self,
                    int depth);

void bnGenerateILStmtIf(struct bnInterpreter* bone, bnILStmtIf* self,
                        struct bnEnviroment* env, bnCompileCache* ccache);

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
        GPtrArray* statements;
} bnILStmtIfElse;

/**
 * return new instance of bnILStmtIf.
 * @param trueCase
 * @return
 */
bnILStmtIfElse* bnNewILStmtIfElse(bnILStmtIf* trueCase);

/**
 * print a information of bnILStmtIfElse.
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILStmtIfElse(FILE* fp, struct bnStringPool* pool,
                        bnILStmtIfElse* self, int depth);

/**
 * free a bnILStmtIf.
 * @param self
 */
void bnDeleteILStmtIfElse(bnILStmtIfElse* self);

void bnGenerateILStmtIfElse(struct bnInterpreter* bone, bnILStmtIfElse* self,
                            struct bnEnviroment* env, bnCompileCache* ccache);
#endif