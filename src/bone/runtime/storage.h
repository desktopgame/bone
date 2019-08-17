#ifndef BONE_RUNTIME_STORAGE_H
#define BONE_RUNTIME_STORAGE_H

#define OBJECT_MAXSIZE (92)

typedef int* bnReference;

typedef struct bnStorage {
        void* pool;
        int* map;
        int capacity;
        int offset;
        int use;
} bnStorage;

bnStorage* bnNewStorage();

bnReference bnAllocMemory(bnStorage* self);

void bnFreeMemory(bnStorage* self, bnReference index);

void* bnGetMemory(bnStorage* self, bnReference index);

void bnCompact(bnStorage* self);

void bnDeleteStorage(bnStorage* self);
#endif