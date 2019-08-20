#ifndef BONE_IL_ILSTMTRETURN_H
#define BONE_IL_ILSTMTRETURN_H
#include "../bone.h"
#include "il_expression.h"

/**
 * bnILStmtReturnはreturn文を表す構造体です。
 */
typedef struct bnILStmtReturn {
        bnILExpression* expr;
} bnILStmtReturn;

/**
 * 新しいbnILStmtReturnインスタンスを生成して返します。
 * @param expr
 * @return
 */
bnILStmtReturn* bnNewILStmtReturn(bnILExpression* expr);

/**
 * return文をダンプします。
 * @param fp
 * @param self
 * @param depth
 */
void bnDumpILStmtReturn(FILE* fp, struct bnStringPool* pool,
                        bnILStmtReturn* self, int depth);

/**
 * return文のバイトコードを生成します。
 * @param bone
 * @param self
 * @param env
 * @param ccache
 */
void bnGenerateILStmtReturn(struct bnInterpreter* bone, bnILStmtReturn* self,
                            struct bnEnviroment* env, bnCompileCache* ccache);

/**
 * return文を解放します。
 * @param self
 */
void bnDeleteILStmtReturn(bnILStmtReturn* self);
#endif
