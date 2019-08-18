#ifndef BONE_RUNTIME_STORAGE_H
#define BONE_RUNTIME_STORAGE_H

#define OBJECT_MAXSIZE (92)
#define OBJECT_COUNT (100)

typedef int* bnReference;

typedef struct bnStorage {
        char pool[OBJECT_MAXSIZE * OBJECT_COUNT];
        int map[OBJECT_COUNT];
        int use;
        int offset;
        int nextFree;
        struct bnStorage* next;
} bnStorage;

bnStorage* bnNewStorage(int offset);

bnReference bnAllocMemory(bnStorage* self);

void bnFreeMemory(bnStorage* self, bnReference index);

void* bnGetMemory(bnStorage* self, bnReference index);

bnStorage* bnGetStorage(bnStorage* self, bnReference index, int* outFixedPos);

bnReference bnGetReferenceFromGlobalStorageIndex(bnStorage* self, int gindex);

int bnGetGlobalStorageIndexFromPointer(bnStorage* self, void* ptr);

void bnShowStorage(bnStorage* self);

void bnCompact(bnStorage* self);

void bnDeleteStorage(bnStorage* self);
#endif