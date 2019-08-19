#ifndef BONE_RUNTIME_CHAR_H
#define BONE_RUNTIME_CHAR_H
#include "object.h"

/**
 * 新しい文字インスタンスを生成し、参照を返します。
 * @param bone
 * @param name
 * @return
 */
bnReference bnNewChar(struct bnInterpreter* bone, char value);

/**
 * C言語のcharへ変換します。
 * @param obj
 * @return
 */
char bnGetCharValue(bnObject* obj);

/**
 * 値を上書きします。
 * @param obj
 * @param c
 */
void bnSetCharValue(bnObject* obj, char c);
#endif