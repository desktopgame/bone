#ifndef BONE_RUNTIME_BOOL_H
#define BONE_RUNTIME_BOOL_H
#include "object.h"

/**
 * return new instance of bnBool.
 * @param bone
 * @param name
 * @return
 */
bnReference bnNewBool(struct bnInterpreter* bone, bool value);

/**
 * @param heap
 * @param t
 * @param f
 */
void bnSetFlipValue(struct bnHeap* heap, bnReference t, bnReference f);

/**
 * @param obj
 * @return
 */
bnReference bnGetFlipValue(bnObject* obj);

/**
 * @param obj
 * @return
 */
bool bnGetBoolValue(bnObject* obj);
#endif