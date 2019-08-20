#ifndef BONE_IL_ILEXPRUOP_H
#define BONE_IL_ILEXPRUOP_H
#include "../bone.h"
#include "il_expression.h"
struct bnInterpreter;

/**
 * bnILUOpTypeは単項演算子の種類を表す列挙です。
 */
typedef enum bnILUOpType {
        BN_IL_UNOP_NOT,
        BN_IL_UNOP_NEGATIVE,
        BN_IL_UNOP_CHILDA,
} bnILUOpType;

/**
 * bnILExprUOpは単項演算子の種類を表す構造体です。
 */
typedef struct bnILExprUOp {
        bnILExpression* a;
        bnILUOpType type;
} bnILExprUOp;

/**
 * 新しいbnILExprUOpインスタンスを生成して返します。
 * @param type
 * @return
 */
bnILExprUOp* bnNewILExprUOp(bnILUOpType type);

/**
 * 単項演算子を再帰的にダンプします。
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprUOp(FILE* fp, struct bnStringPool* pool, bnILExprUOp* self,
                     int depth);

/**
 * 単項演算子のバイトコードを生成します。
 * @param bone
 * @param self
 * @param env
 * @param ccache
 */
void bnGenerateILExprUOp(struct bnInterpreter* bone, bnILExprUOp* self,
                         struct bnEnviroment* env, bnCompileCache* ccache);

/**
 * 単項演算子を再帰的に解放します。
 * @param self
 */
void bnDeleteILExprUOp(bnILExprUOp* self);
#endif
