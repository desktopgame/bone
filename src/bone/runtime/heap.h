#ifndef BONE_RUNTIME_HEAP_H
#define BONE_RUNTIME_HEAP_H
#include "../glib.h"

struct bnHeap;
struct bnObject;

struct bnHeap* bnNewHeap();

void bnAddToHeap(struct bnHeap* self, struct bnObject* obj);

void bnDeleteHeap(struct bnHeap* self);
#endif