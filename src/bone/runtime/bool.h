#ifndef BONE_RUNTIME_BOOL_H
#define BONE_RUNTIME_BOOL_H
#include "object.h"

/**
 * bnBool is bool.
 */
typedef struct bnBool {
        bnObject base;
        bool value;
} bnBool;

/**
 * return new instance of bnBool.
 * @param name
 * @return
 */
bnBool* bnNewBool(bool value);

/**
 * return new instance of bnBool.
 * @param name
 * @return
 */
void bnDeleteBool(bnBool* self);
#endif