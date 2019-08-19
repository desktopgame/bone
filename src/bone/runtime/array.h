#ifndef BONE_RUNTIME_ARRAY_H
#define BONE_RUNTIME_ARRAY_H
#include "object.h"

/**
 * 新しい配列インスタンスを生成し、参照を返します。
 * @param bone
 * @param size
 * @return
 */
bnReference bnNewArray(struct bnInterpreter* bone, int size);

/**
 * 指定の配列をbnCharインスタンスで埋めます。
 * @param bone
 * @param str
 * @param ref
 */
void bnFillString(struct bnInterpreter* bone, const char* str, bnReference ref);

/**
 * 指定の配列の長さを返します。
 * @param obj
 * @return
 */
int bnGetArrayLength(bnObject* obj);

/**
 * 指定の配列の指定位置のインスタンスへの参照を返します。
 * @param obj
 * @param index
 * @return
 */
bnReference bnGetArrayElementAt(bnObject* obj, int index);

/**
 * 指定の配列の指定位置のインスタンスへの参照を上書きします。
 * @param obj
 * @param index
 * @param ref
 */
void bnSetArrayElementAt(bnObject* obj, int index, bnReference ref);
#endif