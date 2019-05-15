#ifndef BONE_IL_ILSTMTDEFER_H
#define BONE_IL_ILSTMTDEFER_H
#include "../bone.h"
#include "il_statement.h"

/**
 * bnILStmtDefer is defer statemetn.
 */
typedef struct bnILStmtDefer {
        bnILStatement* stmt;
} bnILStmtDefer;

/**
 * return new instance of bnILStmtDefer.
 * @param stmt
 * @return
 */
bnILStmtDefer* bnNewILStmtDefer(bnILStatement* stmt);

/**
 * print a information of bnILStmtDefer.
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILStmtDefer(FILE* fp, struct bnStringPool* pool, bnILStmtDefer* self,
                       int depth);

void bnGenerateILStmtDefer(struct bnInterpreter* bone, bnILStmtDefer* self,
                           struct bnEnviroment* env, bnCompileCache* ccache);

/**
 * free a bnILStmtDefer.
 * @param self
 */
void bnDeleteILStmtDefer(bnILStmtDefer* self);
#endif
