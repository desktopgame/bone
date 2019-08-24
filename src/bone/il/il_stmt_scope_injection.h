#ifndef BONE_IL_ILSTMTSCOPEINJECTION_H
#define BONE_IL_ILSTMTSCOPEINJECTION_H
#include "../bone.h"
#include "il_expression.h"

/**
 * bnILStmtScopeInjectionはスコープインジェクションを表す構造体です。
 */
typedef struct bnILStmtScopeInjection {
        bnILExpression* expr;
} bnILStmtScopeInjection;

/**
 * 新しいbnILStmtScopeInjectionインスタンスを生成して返します。
 * @param expr
 * @return
 */
bnILStmtScopeInjection* bnNewILStmtScopeInjection(bnILExpression* expr);

/**
 * スコープインジェクションを再帰的にダンプします。
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILStmtScopeInjection(FILE* fp, struct bnStringPool* pool,
                                bnILStmtScopeInjection* self, int depth);

/**
 * スコープインジェクションのバイトコードを生成します。
 * @param bone
 * @param self
 * @param env
 * @param ccache
 */
void bnGenerateILStmtScopeInjection(struct bnInterpreter* bone,
                                    bnILStmtScopeInjection* self,
                                    struct bnEnviroment* env,
                                    bnCompileCache* ccache);

/**
 * スコープインジェクションを再帰的に解放します。
 * @param self
 */
void bnDeleteILStmtScopeInjection(bnILStmtScopeInjection* self);
#endif
