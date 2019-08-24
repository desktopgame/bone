#ifndef BONE_IL_ILEXPRCHAR_H
#define BONE_IL_ILEXPRCHAR_H
#include "../bone.h"
#include "compile_cache.h"
struct bnEnviroment;
struct bnInterpreter;

/**
 * bnILExprCharは文字リテラルを表す構造体です。
 */
typedef struct bnILExprChar {
        char value;
} bnILExprChar;

/**
 * 新しいbnILExprCharインスタンスを生成して返します。
 * @param value
 * @return
 */
bnILExprChar* bnNewILExprChar(char value);

/**
 * 文字リテラルをダンプします。
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprChar(FILE* fp, struct bnStringPool* pool, bnILExprChar* self,
                      int depth);
/**
 * 文字リテラルのバイトコードを生成します。
 * @param bone
 * @param self
 * @param env
 * @param ccache
 */
void bnGenerateILExprChar(struct bnInterpreter* bone, bnILExprChar* self,
                          struct bnEnviroment* env, bnCompileCache* ccache);

/**
 * 文字リテラルを解放します。
 * @param self
 */
void bnDeleteILExprChar(bnILExprChar* self);
#endif