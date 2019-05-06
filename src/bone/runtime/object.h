#ifndef BONE_RUNTIME_OBJECT_H
#define BONE_RUNTIME_OBJECT_H
#include "../bone.h"

struct bnInterpreter;
struct bnFrame;
struct bnHeap;

typedef enum bnObjectType {
        BN_OBJECT_PROTO,
        BN_OBJECT_INTEGER,
        BN_OBJECT_DOUBLE,
        BN_OBJECT_CHAR,
        BN_OBJECT_STRING,
        BN_OBJECT_BOOL,
        BN_OBJECT_LAMBDA,
        BN_OBJECT_ARRAY,
} bnObjectType;

typedef struct bnObject {
        GHashTable* table;
        bool mark;
        bnObjectType type;
} bnObject;

void bnInitObject(struct bnHeap* heap, bnObject* self, bnObjectType type);

/**
 * return new instance of bnObject.
 * @param heap
 * @return
 */
bnObject* bnNewObject(struct bnHeap* heap);

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
 * @param pool
 * @param view
 * @return
 */
bnStringView bnGetExportVariableName(struct bnStringPool* pool,
                                     bnStringView view);

/**
 * return new instance of bnObject.
 * @param name
 * @return
 */
void bnDeleteObject(bnObject* self);
#endif