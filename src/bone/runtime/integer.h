#ifndef BONE_RUNTIME_INTEGER_H
#define BONE_RUNTIME_INTEGER_H
#include "interpreter.h"
#include "object.h"

/**
 * 新しい整数インスタンスを生成し、参照を返します。
 * @param bone
 * @param name
 * @return
 */
bnReference bnNewInteger(bnInterpreter* bone, int value);

/**
 * C言語のintに変換します。
 * @param obj
 * @return
 */
int bnGetIntegerValue(bnObject* obj);

/**
 * 値を上書きします。
 * @param obj
 * @param value
 */
void bnSetIntegerValue(bnObject* obj, int value);
#endif