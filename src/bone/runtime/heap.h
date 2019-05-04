#ifndef BONE_RUNTIME_HEAP_H
#define BONE_RUNTIME_HEAP_H
#include "../glib.h"

struct bnHeap;
struct bnObject;
struct bnFrame;

struct bnHeap* bnNewHeap();

void bnAddToHeap(struct bnHeap* self, struct bnObject* obj);

void bnGC(struct bnHeap* self, struct bnFrame* frame);

void bnDeleteHeap(struct bnHeap* self);
#endif