#ifndef BONE_RUNTIME_OBJECT_H
#define BONE_RUNTIME_OBJECT_H
#include "../bone.h"

struct bnInterpreter;
struct bnFrame;

typedef enum bnObjectType {
        BN_OBJECT_PROTO,
        BN_OBJECT_INTEGER,
        BN_OBJECT_DOUBLE,
        BN_OBJECT_CHAR,
        BN_OBJECT_STRING,
        BN_OBJECT_BOOL,
        BN_OBJECT_LAMBDA,
} bnObjectType;

typedef struct bnObject {
        GHashTable* table;
        bool mark;
        bnObjectType type;
} bnObject;

void bnInitObject(bnObject* self, bnObjectType type);

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
void bnDefine(bnObject* self, bnStringView name, bnObject* value);

void bnFuncCall(bnObject* self, struct bnInterpreter* bone,
                struct bnFrame* frame, int argc);

void bnPrintObject(FILE* fp, bnObject* self);

/**
 * return new instance of bnObject.
 * @param name
 * @return
 */
void bnDeleteObject(bnObject* self);
#endif