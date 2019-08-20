#ifndef BONE_IL_ILSTMTIF_H
#define BONE_IL_ILSTMTIF_H
#include "../bone.h"
#include "il_expression.h"

/**
 * bnILStmtIfはif文を表す構造体です。
 */
typedef struct bnILStmtIf {
        bnILExpression* cond;
        GPtrArray* statements;
} bnILStmtIf;

/**
 * 新しいbnILStmtIfインスタンスを生成して返します。
 * @param cond
 * @return
 */
bnILStmtIf* bnNewILStmtIf(bnILExpression* cond);

/**
 * if文を再帰的にダンプします。
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILStmtIf(FILE* fp, struct bnStringPool* pool, bnILStmtIf* self,
                    int depth);

/**
 * if文のバイトコードを生成します。
 * @param bone
 * @param self
 * @param env
 * @param ccache
 */
void bnGenerateILStmtIf(struct bnInterpreter* bone, bnILStmtIf* self,
                        struct bnEnviroment* env, bnCompileCache* ccache);

/**
 * if文を再帰的に解放します。
 * @param self
 */
void bnDeleteILStmtIf(bnILStmtIf* self);

/**
 * if-else文を表す構造体です。
 */
typedef struct bnILStmtIfElse {
        bnILStmtIf* trueCase;
        GPtrArray* statements;
} bnILStmtIfElse;

/**
 * 新しいbnILStmtIfElseインスタンスを生成して返します。
 * @param trueCase
 * @return
 */
bnILStmtIfElse* bnNewILStmtIfElse(bnILStmtIf* trueCase);

/**
 * if-else文を再帰的にダンプします。
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILStmtIfElse(FILE* fp, struct bnStringPool* pool,
                        bnILStmtIfElse* self, int depth);

/**
 * if-else文を再帰的に解放します。
 * @param self
 */
void bnDeleteILStmtIfElse(bnILStmtIfElse* self);

/**
 * if-else文のバイトコードを生成します。
 * @param bone
 * @param self
 * @param env
 * @param ccache
 */
void bnGenerateILStmtIfElse(struct bnInterpreter* bone, bnILStmtIfElse* self,
                            struct bnEnviroment* env, bnCompileCache* ccache);
#endif