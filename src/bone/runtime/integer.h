#ifndef BONE_RUNTIME_INTEGER_H
#define BONE_RUNTIME_INTEGER_H
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
 * @param name
 * @return
 */
bnInteger* bnNewInteger(int value);

/**
 * return new instance of bnInteger.
 * @param name
 * @return
 */
void bnDeleteInteger(bnInteger* self);
#endif