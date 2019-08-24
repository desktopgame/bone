#ifndef BONE_IL_ILEXPRLIT_H
#define BONE_IL_ILEXPRLIT_H
#include "../bone.h"
#include "il_expression.h"

/**
 * bnILExprArrayLitは配列リテラルを表す構造体です。
 */
typedef struct bnILExprArrayLit {
        GPtrArray* expressions;
} bnILExprArrayLit;

/**
 * 新しいbnILExprArrayLitインスタンスを生成して返します。
 * @return
 */
bnILExprArrayLit* bnNewILExprArrayLit();

/**
 * 配列リテラルを再帰的にダンプします。
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprArrayLit(FILE* fp, struct bnStringPool* pool,
                          bnILExprArrayLit* self, int depth);

/**
 * 配列リテラルのバイトコードを生成します。
 * @param bone
 * @param self
 * @param env
 * @param ccache
 */
void bnGenerateILExprArrayLit(struct bnInterpreter* bone,
                              bnILExprArrayLit* self, struct bnEnviroment* env,
                              bnCompileCache* ccache);

/**
 * 配列リテラルを再帰的に解放します。
 * @param self
 */
void bnDeleteILExprArrayLit(bnILExprArrayLit* self);
#endif