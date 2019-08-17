#ifndef BONE_RUNTIME_STORAGE_H
#define BONE_RUNTIME_STORAGE_H

#define OBJECT_MAXSIZE (48)

typedef struct bnStorage {
        void* pool;
        int* map;
        int capacity;
        int offset;
        int use;
} bnStorage;

bnStorage* bnNewStorage();

int* bnAllocMemory(bnStorage* self);

void bnFreeMemory(bnStorage* self, int* index);

void* bnGetMemory(bnStorage* self, int* index);

void bnCompact(bnStorage* self);

void bnDeleteStorage(bnStorage* self);
#endif