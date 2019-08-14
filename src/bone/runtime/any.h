#ifndef BONE_RUNTIME_ANY_H
#define BONE_RUNTIME_ANY_H
#include "object.h"
struct bnInterpreter;

typedef struct bnAny {
        bnObject base;
        bnStringView type;
} bnAny;

/**
 * @param bone
 * @param self
 * @param type
 */
void bnInitAny(struct bnInterpreter* bone, bnAny* self, const char* type);

/**
 * @param bone
 * @param self
 * @parram type
 * @return
 */
bool bnMatchType(struct bnInterpreter* bone, bnObject* self, const char* type);
#endif