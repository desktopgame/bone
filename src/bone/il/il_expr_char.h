#ifndef BONE_IL_ILEXPRCHAR_H
#define BONE_IL_ILEXPRCHAR_H
#include "../bone.h"

/**
 * bnILExprChar is literal of character.
 */
typedef struct bnILExprChar {
        char value;
} bnILExprChar;

/**
 * return new instance of bnILExprChar.
 * @param value
 * @return
 */
bnILExprChar* bnNewILExprChar(char value);

/**
 * free a bnILExprChar.
 * @param self
 */
void bnDeleteILExprChar(bnILExprChar* self);
#endif