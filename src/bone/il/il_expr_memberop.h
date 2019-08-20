#ifndef BONE_IL_ILEXPRMEMBEROP_H
#define BONE_IL_ILEXPRMEMBEROP_H
#include "../bone.h"
#include "il_expression.h"
struct bnEnviroment;
struct bnInterpreter;

/**
 * bnILExprMemberOpはメンバアクセスを表す構造体です。
 */
typedef struct bnILExprMemberOp {
        bnILExpression* expr;
        bnStringView name;
} bnILExprMemberOp;

/**
 * 新しいbnILExprMemberOpインスタンスを生成して返します。
 * @param name
 * @return
 */
bnILExprMemberOp* bnNewILExprMemberOp(bnStringView name);

/**
 * メンバアクセスを再帰的にダンプします。
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprMemberOp(FILE* fp, struct bnStringPool* pool,
                          bnILExprMemberOp* self, int depth);

/**
 * メンバアクセスのバイトコードを生成します。
 * @param bone
 * @param self
 * @param env
 * @param ccache
 */
void bnGenerateILExprMemberOp(struct bnInterpreter* bone,
                              bnILExprMemberOp* self, struct bnEnviroment* env,
                              bnCompileCache* ccache);

/**
 * メンバアクセスを再帰的に解放します。
 * @param self
 */
void bnDeleteILExprMemberOp(bnILExprMemberOp* self);
#endif
