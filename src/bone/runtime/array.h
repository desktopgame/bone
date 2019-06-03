#ifndef BONE_RUNTIME_ARRAY_H
#define BONE_RUNTIME_ARRAY_H
#include "object.h"

/**
 * bnArray is array.
 */
typedef struct bnArray {
        bnObject base;
        GPtrArray* arr;
        int size;
} bnArray;

/**
 * return new instance of bnArray.
 * @param bone
 * @param size
 * @return
 */
bnArray* bnNewArray(struct bnInterpreter* bone, int size);

/**
 * @param bone
 * @param str
 * @param ary
 */
void bnFillString(struct bnInterpreter* bone, const char* str, bnArray* ary);
#endif