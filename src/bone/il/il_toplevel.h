#ifndef BONE_IL_ILTOPLEVEL_H
#define BONE_IL_ILTOPLEVEL_H
#include "../bone.h"
struct bnEnviroment;
struct bnInterpreter;
/**
 * bnILToplevelはプログラムのトップレベルを表す要素です。
 */
typedef struct bnILToplevel {
        GPtrArray* statements;
} bnILToplevel;

/**
 * 新しいbnILToplevelインスタンスを生成します。
 * @param cond
 * @return
 */
bnILToplevel* bnNewILTopLevel();

/**
 * プログラムを構成するあらゆる中間表現を解放します。
 * @param self
 */
void bnDeleteILTopLevel(bnILToplevel* self);

/**
 * 中間表現を全てダンプします。
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILTopLevel(FILE* fp, struct bnStringPool* pool, bnILToplevel* self,
                      int depth);

/**
 * 中間表現を全てバイトコードに変換します。
 * @param bone
 * @param self
 * @param env
 */
void bnGenerateILTopLevel(struct bnInterpreter* bone, bnILToplevel* self,
                          struct bnEnviroment* env);
#endif