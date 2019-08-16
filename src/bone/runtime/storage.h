#ifndef BONE_RUNTIME_STORAGE_H
#define BONE_RUNTIME_STORAGE_H

typedef struct bnStorage {
        void* pool;
        struct bnStorage* next;
} bnStorage;

#define OBJECT_MAXSIZE (48)
#define OBJECT_COUNT (256)

bnStorage* bnNewStorage();

void* bnAllocMemory(bnStorage* self);

void bnFreeMemory(bnStorage* self, void* memory);

void bnDeleteStorage(bnStorage* self);
#endif