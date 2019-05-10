#ifndef BONE_RUNTIME_STRING_H
#define BONE_RUNTIME_STRING_H
#include "interpreter.h"
#include "object.h"

/**
 * bnString is bone string.
 */
typedef struct bnString {
        bnObject base;
        bnStringView value;
} bnString;

/**
 * return new instance of bnString.
 * @param bone
 * @param name
 * @return
 */
bnString* bnNewString(bnInterpreter* bone, bnStringView value);

#endif