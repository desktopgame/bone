#ifndef BONE_IL_ILEXPRLAMBDA_H
#define BONE_IL_ILEXPRLAMBDA_H
#include "../bone.h"
#include "compile_cache.h"
struct bnEnviroment;
struct bnInterpreter;

/**
 * bnILExprLambdaはラムダ式を表す構造体です。
 */
typedef struct bnILExprLambda {
        GPtrArray* parameters;
        GPtrArray* returns;
        GPtrArray* statements;
        bnStringView filename;
        int lineno;
} bnILExprLambda;

/**
 * 新しいbnILExprLambdaインスタンスを生成して返します。
 * @param filename
 * @param lineno
 * @return
 */
bnILExprLambda* bnNewILExprLambda(bnStringView filename, int lineno);

/**
 * ラムダ式を再帰的にダンプします。
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprLambda(FILE* fp, struct bnStringPool* pool,
                        bnILExprLambda* self, int depth);

/**
 * ラムダ式のバイトコードを生成します。
 * @param bone
 * @param self
 * @param env
 * @param ccache
 */
void bnGenerateILExprLambda(struct bnInterpreter* bone, bnILExprLambda* self,
                            struct bnEnviroment* env, bnCompileCache* ccache);

/**
 * ラムダ式を再帰的に解放します。
 * @param self
 */
void bnDeleteILExprLambda(bnILExprLambda* self);
#endif
