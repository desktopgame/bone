#ifndef BONE_RUNTIME_CHAR_H
#define BONE_RUNTIME_CHAR_H
#include "object.h"

/**
 * return new instance of bnChar.
 * @param bone
 * @param name
 * @return
 */
bnObject* bnNewChar(struct bnInterpreter* bone, char value);

/**
 * @param obj
 * @return
 */
char bnGetCharValue(bnObject* obj);

/**
 * @param obj
 * @param c
 */
void bnSetCharValue(bnObject* obj, char c);
#endif