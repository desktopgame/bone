#ifndef BONE_RUNTIME_ARRAY_H
#define BONE_RUNTIME_ARRAY_H
#include "object.h"

/**
 * return new instance of bnArray.
 * @param bone
 * @param size
 * @return
 */
bnObject* bnNewArray(struct bnInterpreter* bone, int size);

/**
 * @param bone
 * @param str
 * @param obj
 */
void bnFillString(struct bnInterpreter* bone, const char* str, bnObject* obj);

/**
 * @param obj
 * @return
 */
int bnGetArrayLength(bnObject* obj);

/**
 * @param obj
 * @param index
 * @return
 */
bnObject* bnGetArrayElementAt(bnObject* obj, int index);

/**
 * @param obj
 * @param index
 * @param value
 */
void bnSetArrayElementAt(bnObject* obj, int index, bnObject* value);
#endif