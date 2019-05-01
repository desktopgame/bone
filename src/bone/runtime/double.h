#ifndef BONE_RUNTIME_DOUBLE_H
#define BONE_RUNTIME_DOUBLE_H
#include "object.h"

/**
 * bnDouble is double.
 */
typedef struct bnDouble {
        bnObject base;
        double value;
} bnDouble;

/**
 * return new instance of bnDouble.
 * @param name
 * @return
 */
bnDouble* bnNewDouble(double value);

/**
 * return new instance of bnDouble.
 * @param name
 * @return
 */
void bnDeleteDouble(bnDouble* self);
#endif