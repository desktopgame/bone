#ifndef BONE_IL_ILEXPRARRAYSUBSCRIPT_H
#define BONE_IL_ILEXPRARRAYSUBSCRIPT_H
#include "../bone.h"
#include "il_expression.h"

/**
 * bnILExprArraySubscriptは配列参照演算子を表す構造体です。
 */
typedef struct bnILExprArraySubscript {
        bnILExpression* arrayExpr;
        bnILExpression* indexExpr;
} bnILExprArraySubscript;

/**
 * 新しいbnILExprArraySubscriptインスタンスを生成して返します。
 * @param arrayExpr
 * @param indexExpr
 * @return
 */
bnILExprArraySubscript* bnNewILExprArraySubscript(bnILExpression* arrayExpr,
                                                  bnILExpression* indexExpr);

/**
 * 配列参照演算子を再帰的にダンプします。
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprArraySubscript(FILE* fp, struct bnStringPool* pool,
                                bnILExprArraySubscript* self, int depth);

/**
 * 配列参照演算子のバイトコードを生成します。
 * @param bone
 * @param self
 * @param env
 * @param ccache
 */
void bnGenerateILExprArraySubscript(struct bnInterpreter* bone,
                                    bnILExprArraySubscript* self,
                                    struct bnEnviroment* env,
                                    bnCompileCache* ccache);

/**
 * 配列参照演算子を再帰的に解放します。
 * @param self
 */
void bnDeleteILExprArraySubscript(bnILExprArraySubscript* self);
#endif
