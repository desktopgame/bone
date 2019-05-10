#ifndef BONE_RUNTIME_BOOL_H
#define BONE_RUNTIME_BOOL_H
#include "object.h"

/**
 * bnBool is bool.
 */
typedef struct bnBool {
        bnObject base;
        bool value;
        struct bnBool* r;
} bnBool;

/**
 * return new instance of bnBool.
 * @param bone
 * @param name
 * @return
 */
bnBool* bnNewBool(struct bnInterpreter* bone, bool value);
#endif