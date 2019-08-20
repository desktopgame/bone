#include "storage.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "memory.h"
#include "object.h"

static bnReference find_free_object(bnStorage* self, bnStorage** outStorage);
static bnStorage* append_storage(bnStorage* self);
static void clear_storage(bnStorage* self);
static int int_compare(const void* a, const void* b);
static void compact_impl(bnStorage* self);

bnStorage* bnNewStorage(int objectSize, int objectCount) {
        bnStorage* ret = BN_MALLOC(sizeof(bnStorage));
        ret->use = 0;
        ret->offset = 0;
        ret->objectSize = objectSize;
        ret->objectCount = objectCount;
        ret->next = NULL;
        ret->nextFree = 0;
        ret->table = BN_MALLOC(sizeof(int) * objectCount);
        ret->pool = BN_MALLOC(objectSize * objectCount);
        ret->bitmap = BN_MALLOC(sizeof(bool) * objectCount);
        clear_storage(ret);
        return ret;
}

bnReference bnAllocMemory(bnStorage* self) {
        bnStorage* out;
        bnReference ret = find_free_object(self, &out);
        if (ret == NULL) {
                return bnAllocMemory(append_storage(self));
        }
        out->use++;
        int i = *ret - out->offset;
        bool* bit = out->bitmap + (sizeof(bool) * i);
        (*bit) = false;
        return ret;
}

void bnFreeMemory(bnStorage* self, bnReference index) {
        int i;
        bnStorage* str = bnGetStorage(self, index, &i);
        char* obj = (char*)(str->pool + (self->objectSize * i));
        bool* bit = str->bitmap + (sizeof(bool) * i);
        memset(obj, 0, self->objectSize);
        str->use--;
        (*bit) = true;
}

void* bnGetMemory(bnStorage* self, bnReference index) {
        if (index == NULL) {
                return NULL;
        }
#if DEBUG
        int gd = bnGetGlobalStorageIndexFromPointer(self, index);
        if (gd > 0) {
                bnReference vref =
                    bnGetReferenceFromGlobalStorageIndex(self, gd);
                void* mem = bnGetMemory(self, vref);
                assert(memcmp(mem, index, self->objectSize));
        }
#endif
        int i;
        bnStorage* str = bnGetStorage(self, index, &i);
        char* obj = (char*)(str->pool + (self->objectSize * i));
        return obj;
}

bnStorage* bnGetStorage(bnStorage* self, bnReference index, int* outFixedPos) {
        assert(index != NULL);
        int i = *index;
        bnStorage* iter = self;
        while (i >= self->objectCount) {
                i -= self->objectCount;
                iter = iter->next;
        }
        (*outFixedPos) = i;
        return iter;
}

bnReference bnGetReferenceFromGlobalStorageIndex(bnStorage* self, int gindex) {
        bnStorage* iter = self;
        while (gindex >= self->objectCount) {
                gindex -= self->objectCount;
                iter = iter->next;
        }
        for (int i = 0; i < self->objectCount; i++) {
                int index = iter->table[i];
                int refTo = (gindex + iter->offset);
                if (index == refTo) {
                        return iter->table + gindex;
                }
        }
        return NULL;
}

int bnGetGlobalStorageIndexFromPointer(bnStorage* self, void* ptr) {
        bnStorage* iter = self;
        int ret = -1;
        while (iter != NULL && ret == -1) {
                if (!((char*)ptr >= iter->pool) ||
                    !((char*)ptr <
                      iter->pool + (self->objectSize * self->objectCount))) {
                        iter = iter->next;
                        continue;
                }
                for (int i = 0; i < self->objectCount; i++) {
                        char* e = (char*)(iter->pool + (self->objectSize * i));
                        if (!memcmp(e, ptr, self->objectSize)) {
                                ret = i + iter->offset;
                                break;
                        }
                }
                iter = iter->next;
        }
        return ret;
}

void bnShowStorage(bnStorage* self) {
        bnStorage* iter = self;
        int count = 0;
        while (iter != NULL) {
                printf("%03d. [", count);
                for (int i = 0; i < self->objectCount; i++) {
                        bool* bit = iter->bitmap + (sizeof(bool) * i);
                        if (*bit) {
                                printf("-");
                        } else {
                                printf("+");
                        }
                }
                printf("]\n");
                count++;
                iter = iter->next;
        }
}

void bnCompact(bnStorage* self) {
#if DEBUG
        // bnShowStorage(self);
#endif
        bnStorage* iter = self;
        while (iter != NULL) {
                compact_impl(iter);
                iter = iter->next;
        }
#if DEBUG
        // bnShowStorage(self);
        // printf("Compaction Completed\n");
#endif
}

void bnDeleteStorage(bnStorage* self) {
        if (self->next != NULL) {
                bnDeleteStorage(self->next);
        }
        BN_FREE(self->bitmap);
        BN_FREE(self->table);
        BN_FREE(self->pool);
        BN_FREE(self);
}
// private
static bnReference find_free_object(bnStorage* self, bnStorage** outStorage) {
        bnStorage* iter = self;
        while (iter != NULL) {
                (*outStorage) = iter;
                for (int i = iter->nextFree; i < self->objectCount; i++) {
                        int index = iter->table[i] - iter->offset;
                        assert(index >= 0 && index < self->objectCount);
                        bool* bit = iter->bitmap + (sizeof(bool) * index);
                        if (*bit) {
                                iter->nextFree++;
                                bnReference ref = iter->table + i;
                                int vref = *ref - iter->offset;
                                assert(vref == index);
                                return ref;
                        }
                }
                iter = iter->next;
        }
        return NULL;
}

static bnStorage* append_storage(bnStorage* self) {
        bnStorage* iter = self;
        while (1) {
                if (iter->next != NULL) {
                        iter = iter->next;
                        continue;
                }
                iter->next = bnNewStorage(self->objectSize, self->objectCount);
                iter->next->offset = iter->offset + iter->objectCount;
                clear_storage(iter->next);
                break;
        }
        return iter->next;
}

static void clear_storage(bnStorage* self) {
        memset(self->pool, 0, self->objectSize * self->objectCount);
        memset(self->table, 0, sizeof(int) * self->objectCount);
        memset(self->bitmap, true, sizeof(bool) * self->objectCount);
        for (int i = 0; i < self->objectCount; i++) {
                self->table[i] = i + self->offset;
        }
}

static int int_compare(const void* a, const void* b) {
        return (*(int*)a) - (*(int*)b);
}

static void compact_impl(bnStorage* self) {
        if (self->use >= self->objectCount) {
                // can't compaction
                return;
        }
        int count = 0;
        int lastFree = self->objectCount - 1;
        // sort all objects
        int mapBuf[self->objectCount];
        char poolBuf[self->objectSize * self->objectCount];
        bool bitmapBuf[sizeof(bool) * self->objectCount];
        memcpy(mapBuf, self->table, sizeof(int) * self->objectCount);
        memset(poolBuf, 0, self->objectSize * self->objectCount);
        memset(bitmapBuf, true, sizeof(bool) * self->objectCount);
        // sort 0 -> 100
        qsort(self->table, self->objectCount, sizeof(int), int_compare);
        if (memcmp(mapBuf, self->table, sizeof(int) * self->objectCount)) {
                for (int i = 0; i < self->objectCount; i++) {
                        int oldPos = mapBuf[i] - self->offset;
                        int newPos = self->table[i] - self->offset;
                        void* oldObj = self->pool + (self->objectSize * oldPos);
                        void* newObj = poolBuf + (self->objectSize * newPos);
                        bool* oldBit = self->bitmap + (sizeof(bool) * oldPos);
                        bool* newBit = bitmapBuf + (sizeof(bool) * newPos);
                        memcpy(newObj, oldObj, self->objectSize);
                        memcpy(newBit, oldBit, sizeof(bool));
                }
                memcpy(self->pool, poolBuf,
                       self->objectSize * self->objectCount);
                memcpy(self->bitmap, bitmapBuf,
                       sizeof(bool) * self->objectCount);
        }
        // compaction
        for (int i = 0; i < self->objectCount; i++) {
                int index = self->table[i];
                int localIndex = index - self->offset;
                char* obj =
                    (char*)(self->pool + (self->objectSize * localIndex));
                bool* bit = self->bitmap + (sizeof(bool) * localIndex);
                if (*bit) {
                        continue;
                }
                count++;
                for (int j = lastFree; j >= i; j--) {
                        int swapIndex = self->table[j];
                        int swapLocalIndex = swapIndex - self->offset;
                        char* swapObj = (char*)(self->pool + (self->objectSize *
                                                              swapLocalIndex));
                        bool* swapBit =
                            self->bitmap + (sizeof(bool) * swapLocalIndex);
                        if (!(*swapBit)) {
                                continue;
                        }
                        self->table[i] = swapIndex;
                        self->table[j] = index;
                        memmove(swapObj, obj, self->objectSize);
                        memset(obj, 0, self->objectSize);
                        (*swapBit) = false;
                        (*bit) = true;
                        lastFree = j - 1;
                        break;
                }
                if (count >= self->use) {
                        break;
                }
        }
        self->nextFree = 0;
}
