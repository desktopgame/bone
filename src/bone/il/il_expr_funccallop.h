#ifndef BONE_IL_ILEXPRFUNCCALLOP_H
#define BONE_IL_ILEXPRFUNCCALLOP_H
#include "../bone.h"
#include "il_expression.h"
struct bnEnviroment;
struct bnInterpreter;

/**
 * bnILExprFuncCallOpは関数呼び出しを表す構造体です。
 */
typedef struct bnILExprFuncCallOp {
        bnILExpression* expr;
        GPtrArray* arguments;
} bnILExprFuncCallOp;

/**
 * 新しいbnILExprFuncCallOpインスタンスを生成して返します。
 * @param expr
 * @return
 */
bnILExprFuncCallOp* bnNewILExprFuncCallOp(bnILExpression* expr);

/**
 * 関数呼び出し演算子を再帰的にダンプします。
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprFuncCallOp(FILE* fp, struct bnStringPool* pool,
                            bnILExprFuncCallOp* self, int depth);

/**
 * 関数呼び出し演算子のバイトコードを生成します。
 * @param bone
 * @param self
 * @param env
 * @param ccache
 */
void bnGenerateILExprFuncCallOp(struct bnInterpreter* bone,
                                bnILExprFuncCallOp* self,
                                struct bnEnviroment* env,
                                bnCompileCache* ccache);

/**
 * 関数呼び出し演算子を再帰的に解放します。
 * @param self
 */
void bnDeleteILExprFuncCallOp(bnILExprFuncCallOp* self);
#endif
