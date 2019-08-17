#ifndef BONE_RUNTIME_HEAP_H
#define BONE_RUNTIME_HEAP_H
#include "../glib.h"

struct bnHeap;
struct bnObject;
struct bnFrame;
struct bnInterpreter;

/**
 * @return
 */
struct bnHeap* bnNewHeap();

/**
 * @param self
 * @return
 */
bnReference bnAllocObject(struct bnHeap* self);

/**
 * @param self
 * @param ref
 * @return
 */
void* bnGetObject(struct bnHeap* self, bnReference ref);

/**
 * @parram self
 * @param ref
 */
void bnFreeObject(struct bnHeap* self, bnReference ref);

/**
 * bnPushStage is push new stage.
 * bnStaging is will record objects to created last stage by this function.
 * @param self
 */
void bnPushStage(struct bnHeap* self);

/**
 * bnStaging is will register object to last stage.
 * was registered object is not collect by GC until it stage poped.
 * @param self
 * @param obj
 * @return
 */
struct bnObject* bnStaging(struct bnHeap* self, struct bnObject* obj);

/**
 * bnPopStage is pop last stage.
 * should be call at finished all staging process.
 * @param self
 */
void bnPopStage(struct bnHeap* self);

/**
 * @param bone
 */
void bnGC(struct bnInterpreter* bone);

/**
 * @param self
 */
void bnDeleteHeap(struct bnHeap* self);
#endif