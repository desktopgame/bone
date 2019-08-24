#ifndef BONE_IL_ILSTMTDEFER_H
#define BONE_IL_ILSTMTDEFER_H
#include "../bone.h"
#include "il_statement.h"

/**
 * bnILStmtDeferは遅延実行ブロックを表す構造体です。
 */
typedef struct bnILStmtDefer {
        bnILStatement* stmt;
} bnILStmtDefer;

/**
 * 新しいbnILStmtDeferインスタンスを生成して返します。
 * @param stmt
 * @return
 */
bnILStmtDefer* bnNewILStmtDefer(bnILStatement* stmt);

/**
 * 遅延実行ブロックを再帰的にダンプします。
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILStmtDefer(FILE* fp, struct bnStringPool* pool, bnILStmtDefer* self,
                       int depth);

/**
 * 遅延実行ブロックのバイトコードを生成します。
 * @param bone
 * @param self
 * @param env
 * @param ccache
 */
void bnGenerateILStmtDefer(struct bnInterpreter* bone, bnILStmtDefer* self,
                           struct bnEnviroment* env, bnCompileCache* ccache);

/**
 * 遅延実行ブロックを再帰的に解放します。
 * @param self
 */
void bnDeleteILStmtDefer(bnILStmtDefer* self);
#endif
