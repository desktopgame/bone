#ifndef BONE_IL_ILEXPRESSION_H
#define BONE_IL_ILEXPRESSION_H
#include <stdio.h>
#include "../util/string_pool.h"
#include "compile_cache.h"
struct bnInterpreter;

/**
 * 計算可能な要素の種類を表す列挙です。
 */
typedef enum bnILExpressionType {
        BN_IL_EXPR_NONE,
        BN_IL_EXPR_INT,
        BN_IL_EXPR_DOUBLE,
        BN_IL_EXPR_CHAR,
        BN_IL_EXPR_STRING,
        BN_IL_EXPR_BINOP,
        BN_IL_EXPR_UOP,
        BN_IL_EXPR_MEMBEROP,
        BN_IL_EXPR_VARIABLE,
        BN_IL_EXPR_FUNCCALLOP,
        BN_IL_EXPR_LAMBDA,
        BN_IL_EXPR_OBJECT_INJECTION,
        BN_IL_EXPR_ARRAY_SUBSCRIPT,
        BN_IL_EXPR_ARRAY_LIT,
} bnILExpressionType;

struct bnEnviroment;
struct bnILExprInt;
struct bnILExprDouble;
struct bnILExprChar;
struct bnILExprString;
struct bnILExprBinOp;
struct bnILExprUOp;
struct bnILExprMemberOp;
struct bnILExprVariable;
struct bnILExprFuncCallOp;
struct bnILExprLambda;
struct bnILExprObjectInjection;
struct bnILExprArraySubscript;
struct bnILExprArrayLit;

/**
 * 計算可能な要素を表す構造体です。
 */
typedef struct bnILExpression {
        bnILExpressionType type;
        union {
                struct bnILExprInt* vInt;
                struct bnILExprDouble* vDouble;
                struct bnILExprChar* vChar;
                struct bnILExprString* vString;
                struct bnILExprBinOp* vBinOp;
                struct bnILExprUOp* vUOp;
                struct bnILExprMemberOp* vMemberOp;
                struct bnILExprVariable* vVariable;
                struct bnILExprFuncCallOp* vFuncCallOp;
                struct bnILExprLambda* vLambda;
                struct bnILExprObjectInjection* vObjInj;
                struct bnILExprArraySubscript* vArraySub;
                struct bnILExprArrayLit* vArrayLit;
        } u;
        int line;
} bnILExpression;

/**
 * 新しいbnILExpressionインスタンスを生成して返します。
 * @param type
 * @return
 */
bnILExpression* bnNewILExpression(bnILExpressionType type);

/**
 * 計算可能な要素を再帰的にダンプします。
 * @param fp
 * @parram pool
 * @param self
 * @param depth
 */
void bnDumpILExpression(FILE* fp, struct bnStringPool* pool,
                        bnILExpression* self, int depth);

/**
 * 計算可能な要素のバイトコードを生成します。
 * @param bone
 * @param self
 * @param env
 * @param ccache
 */
void bnGenerateILExpression(struct bnInterpreter* bone, bnILExpression* self,
                            struct bnEnviroment* env, bnCompileCache* ccache);

/**
 * 計算可能な要素を再帰的に解放します。
 * @param self
 */
void bnDeleteILExpression(bnILExpression* self);
#endif