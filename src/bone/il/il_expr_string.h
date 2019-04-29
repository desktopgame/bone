#ifndef BONE_IL_ILEXPRSTRING_H
#define BONE_IL_ILEXPRSTRING_H
#include "../bone.h"

/**
 * bnILExprString is literal of string.
 */
typedef struct bnILExprString {
        GString* value;
} bnILExprString;

/**
 * return new instance of bnILExprString.
 * @param value
 * @return
 */
bnILExprString* bnNewILExprString(GString* value);

/**
 * free a bnILExprString.
 * @param self
 */
void bnDeleteILExprString(bnILExprString* self);
#endif