#ifndef BONE_RUNTIME_INTEGER_H
#define BONE_RUNTIME_INTEGER_H
#include "interpreter.h"
#include "object.h"

/**
 * bnInteger is bone integer.
 */
typedef struct bnInteger {
        bnObject base;
        int value;
} bnInteger;

/**
 * return new instance of bnInteger.
 * @param bone
 * @param name
 * @return
 */
bnInteger* bnNewInteger(bnInterpreter* bone, int value);

#endif