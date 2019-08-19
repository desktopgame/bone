#ifndef BONE_RUNTIME_BOOL_H
#define BONE_RUNTIME_BOOL_H
#include "object.h"

/**
 * 新しい真偽値インスタンスを生成し、参照を返します。
 * @param bone
 * @param name
 * @return
 */
bnReference bnNewBool(struct bnInterpreter* bone, bool value);

/**
 * 二つの真偽値に反転する値への参照をもたせます。
 * @param heap
 * @param t
 * @param f
 */
void bnSetFlipValue(struct bnHeap* heap, bnReference t, bnReference f);

/**
 * 真偽値の反転した値を返します。
 * @param obj
 * @return
 */
bnReference bnGetFlipValue(bnObject* obj);

/**
 * C言語のboolへ変換します。
 * @param obj
 * @return
 */
bool bnGetBoolValue(bnObject* obj);
#endif