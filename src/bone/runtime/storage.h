#ifndef BONE_RUNTIME_STORAGE_H
#define BONE_RUNTIME_STORAGE_H
#include <stdbool.h>

typedef int* bnReference;

typedef struct bnStorage {
        char* pool;
        int* table;
        bool* bitmap;
        int objectSize;
        int objectCount;
        int use;
        int offset;
        int nextFree;
        struct bnStorage* next;
} bnStorage;

bnStorage* bnNewStorage(int objectSize, int objectCount);

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