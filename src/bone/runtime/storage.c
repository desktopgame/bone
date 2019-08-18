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

bnStorage* bnNewStorage(int offset) {
        bnStorage* ret = BN_MALLOC(sizeof(bnStorage));
        ret->use = 0;
        ret->offset = offset;
        ret->next = NULL;
        ret->nextFree = 0;
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
        // assert(*ret != 517);
        int i = *ret - out->offset;
        bnObject* obj = (bnObject*)(out->pool + (OBJECT_MAXSIZE * i));
        assert(obj->freed);
        obj->freed = false;
        return ret;
}

void bnFreeMemory(bnStorage* self, bnReference index) {
        int i;
        bnStorage* str = bnGetStorage(self, index, &i);
        bnObject* obj = (bnObject*)(str->pool + (OBJECT_MAXSIZE * i));
        memset(obj, 0, OBJECT_MAXSIZE);
        str->use--;
        obj->freed = true;
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
                assert(memcmp(mem, index, OBJECT_MAXSIZE));
        }
#endif
        int i;
        bnStorage* str = bnGetStorage(self, index, &i);
        bnObject* obj = (bnObject*)(str->pool + (OBJECT_MAXSIZE * i));
        return obj;
}

bnStorage* bnGetStorage(bnStorage* self, bnReference index, int* outFixedPos) {
        assert(index != NULL);
        int i = *index;
        bnStorage* iter = self;
        while (i >= OBJECT_COUNT) {
                i -= OBJECT_COUNT;
                iter = iter->next;
        }
        (*outFixedPos) = i;
        return iter;
}

bnReference bnGetReferenceFromGlobalStorageIndex(bnStorage* self, int gindex) {
        bnStorage* iter = self;
        while (gindex >= OBJECT_COUNT) {
                gindex -= OBJECT_COUNT;
                iter = iter->next;
        }
        for (int i = 0; i < OBJECT_COUNT; i++) {
                int index = iter->map[i];
                int refTo = (gindex + iter->offset);
                if (index == refTo) {
                        return iter->map + gindex;
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
                      iter->pool + (OBJECT_MAXSIZE * OBJECT_COUNT))) {
                        iter = iter->next;
                        continue;
                }
                for (int i = 0; i < OBJECT_COUNT; i++) {
                        bnObject* e =
                            (bnObject*)(iter->pool + (OBJECT_MAXSIZE * i));
                        if (!memcmp(e, ptr, OBJECT_MAXSIZE)) {
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
                for (int i = 0; i < OBJECT_COUNT; i++) {
                        bnObject* obj =
                            (bnObject*)(iter->pool + (OBJECT_MAXSIZE * i));
                        if (obj->freed) {
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
        bnShowStorage(self);
#endif
        bnStorage* iter = self;
        while (iter != NULL) {
                compact_impl(iter);
                iter = iter->next;
        }
#if DEBUG
        bnShowStorage(self);
        printf("Compaction Completed\n");
#endif
}

void bnDeleteStorage(bnStorage* self) {
        if (self->next != NULL) {
                bnDeleteStorage(self->next);
        }
        BN_FREE(self);
}
// private
static bnReference find_free_object(bnStorage* self, bnStorage** outStorage) {
        bnStorage* iter = self;
        while (iter != NULL) {
                (*outStorage) = iter;
                for (int i = iter->nextFree; i < OBJECT_COUNT; i++) {
                        int index = iter->map[i] - iter->offset;
                        assert(index >= 0 && index < OBJECT_COUNT);
                        bnObject* obj =
                            (bnObject*)(iter->pool + (OBJECT_MAXSIZE * index));
                        if (obj->freed) {
                                iter->nextFree++;
                                bnReference ref = iter->map + i;
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
                iter->next = bnNewStorage(iter->offset + OBJECT_COUNT);
                break;
        }
        return iter->next;
}

static void clear_storage(bnStorage* self) {
        memset(self->pool, 0, OBJECT_MAXSIZE * OBJECT_COUNT);
        memset(self->map, 0, sizeof(int) * OBJECT_COUNT);
        for (int i = 0; i < OBJECT_COUNT; i++) {
                bnObject* obj = (bnObject*)(self->pool + (OBJECT_MAXSIZE * i));
                self->map[i] = i + self->offset;
                obj->freed = true;
        }
}

static int int_compare(const void* a, const void* b) {
        return (*(int*)a) - (*(int*)b);
}

static void compact_impl(bnStorage* self) {
        int count = 0;
        int lastFree = OBJECT_COUNT - 1;
        // sort all objects
        int mapBuf[OBJECT_COUNT];
        char poolBuf[OBJECT_MAXSIZE * OBJECT_COUNT];
        memcpy(mapBuf, self->map, sizeof(int) * OBJECT_COUNT);
        memset(poolBuf, 0, OBJECT_MAXSIZE * OBJECT_COUNT);
        // sort 0 -> 100
        qsort(self->map, OBJECT_COUNT, sizeof(int), int_compare);
        for (int i = 0; i < OBJECT_COUNT; i++) {
                int oldPos = mapBuf[i] - self->offset;
                int newPos = self->map[i] - self->offset;
                void* oldObj = self->pool + (OBJECT_MAXSIZE * oldPos);
                void* newObj = poolBuf + (OBJECT_MAXSIZE * newPos);
                memcpy(newObj, oldObj, OBJECT_MAXSIZE);
        }
        memcpy(self->pool, poolBuf, OBJECT_MAXSIZE * OBJECT_COUNT);
        // compaction
        for (int i = OBJECT_COUNT - 1; i >= 0; i--) {
                int index = self->map[i];
                int localIndex = index - self->offset;
                bnObject* obj =
                    (bnObject*)(self->pool + (OBJECT_MAXSIZE * localIndex));
                if (obj->freed) {
                        continue;
                }
                count++;
                for (int j = lastFree; j >= i; j--) {
                        int swapIndex = self->map[j];
                        int swapLocalIndex = swapIndex - self->offset;
                        bnObject* swapObj =
                            (bnObject*)(self->pool +
                                        (OBJECT_MAXSIZE * swapLocalIndex));
                        if (!swapObj->freed) {
                                continue;
                        }
                        self->map[i] = swapIndex;
                        self->map[j] = index;
                        memmove(swapObj, obj, OBJECT_MAXSIZE);
                        memset(obj, 0, OBJECT_MAXSIZE);
                        obj->freed = true;
                        lastFree = j - 1;
                        break;
                }
                if (count >= self->use) {
                        //           break;
                }
        }
        self->nextFree = 0;
}
