#ifndef BONE_IL_ILEXPRVARIABLE_H
#define BONE_IL_ILEXPRVARIABLE_H
#include "../bone.h"
#include "compile_cache.h"
struct bnEnviroment;
struct bnInterpreter;

/**
 * bnILExprVariableは変数を表す構造体です。
 */
typedef struct bnILExprVariable {
        bnStringView name;
} bnILExprVariable;

/**
 * 新しいbnILExprVariableインスタンスを生成して返します。
 * @param name
 * @return
 */
bnILExprVariable* bnNewILExprVariable(bnStringView name);

/**
 * 変数をダンプします。
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprVariable(FILE* fp, struct bnStringPool* pool,
                          bnILExprVariable* self, int depth);

/**
 * 変数のバイトコードを生成します。
 * @param bone
 * @param self
 * @param env
 * @param cache
 */
void bnGenerateILExprVariable(struct bnInterpreter* bone,
                              bnILExprVariable* self, struct bnEnviroment* env,
                              bnCompileCache* ccache);

/**
 * 変数を解放します。
 * @param self
 */
void bnDeleteILExprVariable(bnILExprVariable* self);
#endif
