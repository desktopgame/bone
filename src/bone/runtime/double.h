#ifndef BONE_RUNTIME_DOUBLE_H
#define BONE_RUNTIME_DOUBLE_H
#include "object.h"
struct bnFrame;
struct bnInterpreter;

/**
 * 新しい実数インスタンスを生成して、参照を返します。
 * @param bone
 * @param name
 * @return
 */
bnReference bnNewDouble(struct bnInterpreter* bone, double value);

/**
 * 値を上書きします。
 * @param obj
 * @param value
 */
void bnSetDoubleValue(bnObject* obj, double value);

/**
 * C言語のdoubleへ変換します。
 * @param obj
 * @return
 */
double bnGetDoubleValue(bnObject* obj);
#endif