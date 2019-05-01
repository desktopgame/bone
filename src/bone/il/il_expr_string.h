#ifndef BONE_IL_ILEXPRSTRING_H
#define BONE_IL_ILEXPRSTRING_H
#include "../bone.h"

/**
 * bnILExprString is literal of string.
 */
typedef struct bnILExprString {
        bnStringView value;
} bnILExprString;

/**
 * return new instance of bnILExprString.
 * @param value
 * @return
 */
bnILExprString* bnNewILExprString(bnStringView value);

/**
 * print a information of bnILExprString.
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprString(FILE* fp, struct bnStringPool* pool,
                        bnILExprString* self, int depth);

/**
 * free a bnILExprString.
 * @param self
 */
void bnDeleteILExprString(bnILExprString* self);
#endif