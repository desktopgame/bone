#ifndef BONE_RUNTIME_ARRAY_H
#define BONE_RUNTIME_ARRAY_H
#include "object.h"

/**
 * bnArray is bool.
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
 * free a bnArray.
 * @param name
 * @return
 */
void bnDeleteArray(bnArray* self);
#endif