#ifndef BONE_RUNTIME_OBJECT_H
#define BONE_RUNTIME_OBJECT_H
#include "../bone.h"

typedef struct bnObject {
        GHashTable* table;
        bool mark;
} bnObject;

void bnInitObject(bnObject* self);

/**
 * return new instance of bnObject.
 * @param name
 * @return
 */
bnObject* bnNewObject();

/**
 * @param self
 * @param name
 * @param value
 */
void bnDefine(bnObject* self, const char* name, bnObject* value);

/**
 * return new instance of bnObject.
 * @param name
 * @return
 */
void bnDeleteObject(bnObject* self);
#endif