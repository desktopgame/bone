#ifndef BONE_IL_ILEXPRDOUBLE_H
#define BONE_IL_ILEXPRDOUBLE_H
#include "../bone.h"
/**
 * bnILExprDouble is literal of double precision floating point type.
 */
typedef struct bnILExprDouble {
        double value;
} bnILExprDouble;

/**
 * return new instance of bnILExprDouble.
 * @param value
 * @return
 */
bnILExprDouble* bnNewILExprDouble(double value);

/**
 * free a bnILExprDouble.
 * @param self
 */
void bnDeleteILExprDouble(bnILExprDouble* self);
#endif