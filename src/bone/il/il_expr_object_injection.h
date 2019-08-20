#ifndef BONE_IL_ILEXPROBJECTINJECTION_H
#define BONE_IL_ILEXPROBJECTINJECTION_H
#include "../bone.h"
#include "il_expression.h"

/**
 * bnILExprObjectInjectionはオブジェクトインジェクションを表す構造体です。
 */
typedef struct bnILExprObjectInjection {
        bnILExpression* leftExpr;
        bnILExpression* rightExpr;
} bnILExprObjectInjection;

/**
 * 新しいbnILExprObjectInjectionインスタンスを生成して返します。
 * @param leftExpr
 * @param rightExpr
 * @return
 */
bnILExprObjectInjection* bnNewILExprObjectInjection(bnILExpression* leftExpr,
                                                    bnILExpression* rightExpr);

/**
 * オブジェクトインジェクションを再帰的にダンプします。
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprObjectInjection(FILE* fp, struct bnStringPool* pool,
                                 bnILExprObjectInjection* self, int depth);

/**
 * オブジェクトインジェクションのバイトコードを生成します。
 * @param bone
 * @param self
 * @param env
 * @param ccache
 */
void bnGenerateILExprObjectInjection(struct bnInterpreter* bone,
                                     bnILExprObjectInjection* self,
                                     struct bnEnviroment* env,
                                     bnCompileCache* ccache);

/**
 * オブジェクトインジェクションを再帰的に解放します。
 * @param self
 */
void bnDeleteILExprObjectInjection(bnILExprObjectInjection* self);
#endif
