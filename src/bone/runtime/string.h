#ifndef BONE_RUNTIME_STRING_H
#define BONE_RUNTIME_STRING_H
#include "interpreter.h"
#include "object.h"

/**
 * return new instance of bnString.
 * @param bone
 * @param name
 * @return
 */
bnObject* bnNewString(bnInterpreter* bone, bnStringView value);

/**
 * @param bone
 * @param str
 * @return
 */
bnObject* bnNewString2(bnInterpreter* bone, const char* str);

/**
 * @param obj
 * @return
 */
bnStringView bnGetStringValue(bnObject* obj);
#endif