#ifndef BONE_RUNTIME_STORAGE_H
#define BONE_RUNTIME_STORAGE_H

#define OBJECT_MAXSIZE (92)
#define OBJECT_COUNT (100)

typedef int* bnReference;

typedef struct bnStorage {
        void* pool;
        int* map;
        int use;
        int offset;
        struct bnStorage* next;
} bnStorage;

bnStorage* bnNewStorage(int offset);

bnReference bnAllocMemory(bnStorage* self);

void bnFreeMemory(bnStorage* self, bnReference index);

void* bnGetMemory(bnStorage* self, bnReference index);

void bnCompact(bnStorage* self);

void bnDeleteStorage(bnStorage* self);
#endif