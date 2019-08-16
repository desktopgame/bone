#ifndef BONE_RUNTIME_OBJECT_H
#define BONE_RUNTIME_OBJECT_H
#include "../bone.h"

struct bnInterpreter;
struct bnFrame;
struct bnHeap;
struct bnObject;
struct bnStorage;
/**
 * bnFreeObjectFunc is for free a object.
 */
typedef void (*bnFreeObjectFunc)(struct bnStorage* storage,
                                 struct bnObject* self);

/**
 * bnObjectType is type of object.
 */
typedef enum bnObjectType {
        BN_OBJECT_PROTO,
        BN_OBJECT_INTEGER,
        BN_OBJECT_DOUBLE,
        BN_OBJECT_CHAR,
        BN_OBJECT_STRING,
        BN_OBJECT_BOOL,
        BN_OBJECT_LAMBDA,
        BN_OBJECT_ARRAY,
        BN_OBJECT_ANY,
} bnObjectType;

/**
 * bnObject is every value in bone.
 */
typedef struct bnObject {
        GHashTable* table;
        bool mark;
        bool freed;
        bnObjectType type;
        bnFreeObjectFunc freeFunc;
} bnObject;

/**
 * initialize a object.
 * @param bone
 * @param self
 * @param type
 */
void bnInitObject(struct bnInterpreter* bone, bnObject* self,
                  bnObjectType type);

/**
 * include a kernel member, into self.
 * @param bonen
 * @param self
 */
void bnIncludeKernel(struct bnInterpreter* bone, bnObject* self);

/**
 * return new instance of bnObject.
 * @param bone
 * @return
 */
bnObject* bnNewObject(struct bnInterpreter* bone);

/**
 * bnDefine is define new member.
 * @param self
 * @param name
 * @param value
 */
void bnDefine(bnObject* self, bnStringView name, bnObject* value);

/**
 * @param self
 * @param pool
 * @param str
 * @param value
 */
void bnDefine2(bnObject* self, struct bnStringPool* pool, const char* str,
               bnObject* value);

/**
 * @param self
 * @param name
 * @return
 */
bnObject* bnLookup(bnObject* self, bnStringView name);

/**
 * @param self
 * @param pool
 * @param str
 * @return
 */
bnObject* bnLookup2(bnObject* self, struct bnStringPool* pool, const char* str);

/**
 * @param self
 * @param name
 * @return
 */
bool bnUndef(bnObject* self, bnStringView name);

/**
 * @param self
 * @param pool
 * @param str
 * @return
 */
bool bnUndef2(bnObject* self, struct bnStringPool* pool, const char* str);

/**
 * @param self
 * @param name
 * @return
 */
bool bnDefined(bnObject* self, bnStringView name);

/**
 * @param self
 * @param pool
 * @param str
 * @return
 */
bool bnDefined2(bnObject* self, struct bnStringPool* pool, const char* str);

/**
 * bnFuncCall is run a `self` as function pointer.
 * it and, return a subframe used for executed function.
 * @param self
 * @param bone
 * @param frame
 * @param argc
 * @return
 */
struct bnFrame* bnFuncCall(bnObject* self, struct bnInterpreter* bone,
                           struct bnFrame* frame, int argc);

/**
 * @param fp
 * @param bone
 * @param self
 */
void bnPrintObject(FILE* fp, struct bnInterpreter* bone, bnObject* self);

/**
 * @param pool
 * @param self
 */
void bnCleanupInjectionBuffer(struct bnStringPool* pool, bnObject* self);

/**
 * @param pool
 * @param view
 * @return
 */
bnStringView bnGetExportVariableName(struct bnStringPool* pool,
                                     bnStringView view);

/**
 * return new instance of bnObject.
 * @param storage
 * @param self
 * @return
 */
void bnDeleteObject(struct bnStorage* storage, bnObject* self);
#endif