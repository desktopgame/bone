#ifndef BONE_RUNTIME_INTEGER_H
#define BONE_RUNTIME_INTEGER_H
#include "interpreter.h"
#include "object.h"

/**
 * return new instance of bnInteger.
 * @param bone
 * @param name
 * @return
 */
bnReference bnNewInteger(bnInterpreter* bone, int value);

/**
 * @param obj
 * @return
 */
int bnGetIntegerValue(bnObject* obj);

/**
 * @param obj
 * @param value
 */
void bnSetIntegerValue(bnObject* obj, int value);
#endif