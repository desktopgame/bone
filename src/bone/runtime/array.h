#ifndef BONE_RUNTIME_ARRAY_H
#define BONE_RUNTIME_ARRAY_H
#include "object.h"

/**
 * return new instance of bnArray.
 * @param bone
 * @param size
 * @return
 */
bnReference bnNewArray(struct bnInterpreter* bone, int size);

/**
 * @param bone
 * @param str
 * @param ref
 */
void bnFillString(struct bnInterpreter* bone, const char* str, bnReference ref);

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
bnReference bnGetArrayElementAt(bnObject* obj, int index);

/**
 * @param obj
 * @param index
 * @param ref
 */
void bnSetArrayElementAt(bnObject* obj, int index, bnReference ref);
#endif