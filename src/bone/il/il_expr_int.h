#ifndef BONE_IL_ILEXPRINT_H
#define BONE_IL_ILEXPRINT_H
#include "../bone.h"

/**
 * bnILExprInt is literal of integer.
 */
typedef struct bnILExprInt {
        int value;
} bnILExprInt;

/**
 * return new instance of bnILExprInt.
 * @param value
 * @return
 */
bnILExprInt* bnNewILExprInt(int value);

/**
 * free a bnILExprInt.
 * @param self
 */
void bnDeleteILExprInt(bnILExprInt* self);
#endif