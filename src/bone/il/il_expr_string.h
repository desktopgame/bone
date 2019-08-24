#ifndef BONE_IL_ILEXPRSTRING_H
#define BONE_IL_ILEXPRSTRING_H
#include "../bone.h"
#include "compile_cache.h"
struct bnEnviroment;
struct bnInterpreter;

/**
 * bnILExprStringは文字リテラルを表す構造体です。
 */
typedef struct bnILExprString {
        bnStringView value;
} bnILExprString;

/**
 * 新しいbnILExprStringインスタンスを生成して返します。
 * @param value
 * @return
 */
bnILExprString* bnNewILExprString(bnStringView value);

/**
 * 文字リテラルをダンプします。
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprString(FILE* fp, struct bnStringPool* pool,
                        bnILExprString* self, int depth);

/**
 * 文字リテラルのバイトコードを生成します。
 * @param bone
 * @param self
 * @param env
 * @param ccache
 */
void bnGenerateILExprString(struct bnInterpreter* bone, bnILExprString* self,
                            struct bnEnviroment* env, bnCompileCache* ccache);

/**
 * 文字リテラルを解放します。
 * @param self
 */
void bnDeleteILExprString(bnILExprString* self);
#endif