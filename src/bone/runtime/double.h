#ifndef BONE_RUNTIME_DOUBLE_H
#define BONE_RUNTIME_DOUBLE_H
#include "object.h"
struct bnFrame;
struct bnInterpreter;

/**
 * bnDouble is double.
 */
typedef struct bnDouble {
        bnObject base;
        double value;
} bnDouble;

/**
 * return new instance of bnDouble.
 * @param bone
 * @param name
 * @return
 */
bnDouble* bnNewDouble(struct bnInterpreter* bone, double value);

#endif