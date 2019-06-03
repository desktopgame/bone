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
 * @param obj
 */
void bnAddToHeap(struct bnHeap* self, struct bnObject* obj);

/**
 * @param bone
 */
void bnGC(struct bnInterpreter* bone);

/**
 * @param self
 * @param obj
 */
void bnDrop(struct bnHeap* self, struct bnObject* obj);

/**
 * protect `obj`.
 * @param self
 * @param obj
 * @return
 */
struct bnObject* bnProtect(struct bnHeap* self, struct bnObject* obj);

/**
 * release protected all object.
 * @param self
 */
void bnRelease(struct bnHeap* self);

/**
 * @param self
 */
void bnDeleteHeap(struct bnHeap* self);
#endif