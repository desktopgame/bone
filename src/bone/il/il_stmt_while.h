#ifndef BONE_IL_ILSTMTWHILE_H
#define BONE_IL_ILSTMTWHILE_H
#include "../bone.h"
#include "il_expression.h"

/**
 * bnILStmtWhileはwhile文を表す構造体です。
 */
typedef struct bnILStmtWhile {
        bnILExpression* cond;
        GPtrArray* statements;
} bnILStmtWhile;

/**
 * 新しいbnILStmtWhileインスタンスを生成して返します。
 * @param cond
 * @return
 */
bnILStmtWhile* bnNewILStmtWhile(bnILExpression* cond);

/**
 * while文を再帰的にダンプします。
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILStmtWhile(FILE* fp, struct bnStringPool* pool, bnILStmtWhile* self,
                       int depth);

/**
 * while文のバイトコードを生成します。
 * @param bone
 * @param self
 * @param env
 * @param ccache
 */
void bnGenerateILStmtWhile(struct bnInterpreter* bone, bnILStmtWhile* self,
                           struct bnEnviroment* env, bnCompileCache* ccache);

/**
 * while文を再帰的に解放します。
 * @param self
 */
void bnDeleteILStmtWhile(bnILStmtWhile* self);
#endif