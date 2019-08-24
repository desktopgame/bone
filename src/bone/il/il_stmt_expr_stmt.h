#ifndef BONE_IL_ILSTMTEXPR_H
#define BONE_IL_ILSTMTEXPR_H
#include "../bone.h"
#include "il_expression.h"

/**
 * bnILStmtExprは計算可能な要素だけで構成された文を表す構造体です。
 */
typedef struct bnILStmtExpr {
        bnILExpression* expr;
} bnILStmtExpr;

/**
 * 新しいbnILStmtExprインスタンスを生成して返します。
 * @param expr
 * @return
 */
bnILStmtExpr* bnNewILStmtExpr(bnILExpression* expr);

/**
 * 計算可能な文を再帰的にダンプします。
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILStmtExpr(FILE* fp, struct bnStringPool* pool, bnILStmtExpr* self,
                      int depth);

/**
 * 計算可能な文のバイトコードを生成します。
 * @param bone
 * @param self
 * @param env
 * @param ccache
 */
void bnGenerateILStmtExpr(struct bnInterpreter* bone, bnILStmtExpr* self,
                          struct bnEnviroment* env, bnCompileCache* ccache);

/**
 * 計算可能な文を再帰的に解放します。
 * @param self
 */
void bnDeleteILStmtExpr(bnILStmtExpr* self);
#endif