#ifndef BONE_IL_ILEXPRINT_H
#define BONE_IL_ILEXPRINT_H
#include "../bone.h"
#include "compile_cache.h"
struct bnEnviroment;
struct bnInterpreter;

/**
 * bnILExprIntは整数リテラルを表す構造体です。
 */
typedef struct bnILExprInt {
        int value;
} bnILExprInt;

/**
 * 新しいbnILExprIntインスタンスを生成して返します。
 * @param value
 * @return
 */
bnILExprInt* bnNewILExprInt(int value);

/**
 * 整数リテラルをダンプします。
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprInt(FILE* fp, struct bnStringPool* pool, bnILExprInt* self,
                     int depth);

/**
 * 整数リテラルのバイトコードを生成します。
 * @param bone
 * @param self
 * @param env
 * @param cache
 */
void bnGenerateILExprInt(struct bnInterpreter* bone, bnILExprInt* self,
                         struct bnEnviroment* env, bnCompileCache* ccache);

/**
 * 整数リテラルを解放します。
 * @param self
 */
void bnDeleteILExprInt(bnILExprInt* self);
#endif