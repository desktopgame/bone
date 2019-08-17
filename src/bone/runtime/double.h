#ifndef BONE_RUNTIME_DOUBLE_H
#define BONE_RUNTIME_DOUBLE_H
#include "object.h"
struct bnFrame;
struct bnInterpreter;

/**
 * return new instance of bnDouble.
 * @param bone
 * @param name
 * @return
 */
bnReference bnNewDouble(struct bnInterpreter* bone, double value);

/**
 * @param obj
 * @param value
 */
void bnSetDoubleValue(bnObject* obj, double value);

/**
 * @param obj
 * @return
 */
double bnGetDoubleValue(bnObject* obj);
#endif