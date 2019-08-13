#ifndef BONE_RUNTIME_BOOL_H
#define BONE_RUNTIME_BOOL_H
#include "object.h"

/**
 * return new instance of bnBool.
 * @param bone
 * @param name
 * @return
 */
bnObject* bnNewBool(struct bnInterpreter* bone, bool value);

/**
 * @param t
 * @param f
 */
void bnSetFlipValue(bnObject* t, bnObject* f);

/**
 * @param obj
 * @return
 */
bnObject* bnGetFlipValue(bnObject* obj);

/**
 * @param obj
 * @return
 */
bool bnGetBoolValue(bnObject* obj);
#endif