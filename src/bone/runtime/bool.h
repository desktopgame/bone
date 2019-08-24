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
 * C言語のboolへ変換します。
 * @param obj
 * @return
 */
bool bnGetBoolValue(bnObject* obj);
#endif