#include "storage.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "memory.h"
#include "object.h"

static bnReference find_free_object(bnStorage* self, bnStorage** outStorage);
static int rfind_free_object_pos(bnStorage* self, int cur);
static bnStorage* append_storage(bnStorage* self);
static void clear_storage(bnStorage* self);
static void compact_impl(bnStorage* self);

bnStorage* bnNewStorage(int offset) {
        bnStorage* ret = BN_MALLOC(sizeof(bnStorage));
        ret->use = 0;
        ret->offset = offset;
        ret->next = NULL;
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

void bnCompact(bnStorage* self) { compact_impl(self); }

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
                for (int i = 0; i < OBJECT_COUNT; i++) {
                        int index = iter->map[i] - iter->offset;
                        bnObject* obj =
                            (bnObject*)(iter->pool + (OBJECT_MAXSIZE * index));
                        if (obj->freed) {
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

static int rfind_free_object_pos(bnStorage* self, int cur) {
        int tailPos = cur;
        // find tail position
        while (tailPos) {
                bnObject* obj =
                    (bnObject*)(self->pool + (OBJECT_MAXSIZE * tailPos));
                if (!obj->freed) {
                        tailPos--;
                } else {
                        break;
                }
        }
        return tailPos;
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

static void compact_impl(bnStorage* self) {
        int tailPos = rfind_free_object_pos(self, OBJECT_COUNT - 1);
        if (!tailPos) {
                return;
        }
        for (int i = 0; i < OBJECT_COUNT; i++) {
                int index = self->map[i];
                bnObject* obj =
                    (bnObject*)(self->pool + (OBJECT_MAXSIZE * index));
                if (obj->freed) {
                        continue;
                }
                bnObject* tail =
                    (bnObject*)(self->pool + (OBJECT_MAXSIZE * tailPos));
                if (tail->freed) {
                        self->map[i] = tailPos;
                        self->map[tailPos] = i;
                        memcpy(tail, obj, OBJECT_MAXSIZE);
                        memset(obj, 0, OBJECT_MAXSIZE);
                        obj->freed = true;
                } else {
                        break;
                }
                tailPos = rfind_free_object_pos(self, tailPos);
                if (!tailPos) {
                        break;
                }
        }
}
