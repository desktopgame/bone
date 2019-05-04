#ifndef BONE_RUNTIME_CHAR_H
#define BONE_RUNTIME_CHAR_H
#include "object.h"

/**
 * bnChar is char.
 */
typedef struct bnChar {
        bnObject base;
        char value;
} bnChar;

/**
 * return new instance of bnChar.
 * @param bone
 * @param name
 * @return
 */
bnChar* bnNewChar(struct bnInterpreter* bone, char value);

/**
 * free a bnChar.
 * @param name
 * @return
 */
void bnDeleteChar(bnChar* self);
#endif