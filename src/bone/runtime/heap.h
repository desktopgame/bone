#ifndef BONE_RUNTIME_HEAP_H
#define BONE_RUNTIME_HEAP_H
#include "../glib.h"

struct bnHeap;
struct bnObject;
struct bnFrame;
struct bnInterpreter;

struct bnHeap* bnNewHeap();

void bnAddToHeap(struct bnHeap* self, struct bnObject* obj);

void bnGC(struct bnInterpreter* bone);

void bnDeleteHeap(struct bnHeap* self);
#endif