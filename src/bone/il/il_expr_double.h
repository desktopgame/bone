#ifndef BONE_IL_ILEXPRDOUBLE_H
#define BONE_IL_ILEXPRDOUBLE_H
#include "../bone.h"
#include "compile_cache.h"
struct bnEnviroment;
struct bnInterpreter;
/**
 * bnILExprDoubleは実数リテラルを表す構造体です。
 */
typedef struct bnILExprDouble {
        double value;
} bnILExprDouble;

/**
 * 新しいbnILExprDoubleインスタンスを生成して返します。
 * @param value
 * @return
 */
bnILExprDouble* bnNewILExprDouble(double value);

/**
 * 実数リテラルをダンプします。
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprDouble(FILE* fp, struct bnStringPool* pool,
                        bnILExprDouble* self, int depth);

/**
 * 実数リテラルのバイトコードを生成します。
 * @param bone
 * @param self
 * @param env
 * @param ccache
 */
void bnGenerateILExprDouble(struct bnInterpreter* bone, bnILExprDouble* self,
                            struct bnEnviroment* env, bnCompileCache* ccache);

/**
 * 実数リテラルを解放します。
 * @param self
 */
void bnDeleteILExprDouble(bnILExprDouble* self);
#endif