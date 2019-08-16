#ifndef BONE_RUNTIME_STORAGE_H
#define BONE_RUNTIME_STORAGE_H

#define OBJECT_MAXSIZE (48)
#define OBJECT_COUNT (256)

typedef struct bnStorage {
        void* pool;
        int map[OBJECT_COUNT];
        int offset;
        int use;
        char searchRule;
        struct bnStorage* prev;
        struct bnStorage* next;
} bnStorage;

bnStorage* bnNewStorage(int offset);

int* bnAllocMemory(bnStorage* self);

void bnFreeMemory(bnStorage* self, int* index);

void* bnGetMemory(bnStorage* self, int* index);

bnStorage* bnGetStorage(bnStorage* head, int* index, int* fixedPos);

void bnCompact(bnStorage* self);

void bnDeleteStorage(bnStorage* self);
#endif