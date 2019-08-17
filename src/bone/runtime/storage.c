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
        ret->pool = BN_MALLOC(OBJECT_MAXSIZE * OBJECT_COUNT);
        ret->map = BN_MALLOC(sizeof(int) * OBJECT_COUNT);
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
        int i = *ret - out->offset;
        bnObject* obj = out->pool + (OBJECT_MAXSIZE * i);
        assert(obj->freed);
        obj->freed = false;
        return ret;
}

void bnFreeMemory(bnStorage* self, bnReference index) {
        bnObject* obj = bnGetMemory(self, index);
        self->use--;
        obj->freed = true;
}

void* bnGetMemory(bnStorage* self, bnReference index) {
        int i = *index;
        bnStorage* iter = self;
        while (i >= OBJECT_COUNT) {
                i -= OBJECT_COUNT;
                iter = iter->next;
        }
        bnObject* obj = iter->pool + (OBJECT_MAXSIZE * i);
        return obj;
}

void bnCompact(bnStorage* self) { compact_impl(self); }

void bnDeleteStorage(bnStorage* self) {
        BN_FREE(self->pool);
        BN_FREE(self->map);
        BN_FREE(self);
}
// private
static bnReference find_free_object(bnStorage* self, bnStorage** outStorage) {
        bnStorage* iter = self;
        while (iter != NULL) {
                (*outStorage) = iter;
                for (int i = 0; i < OBJECT_COUNT; i++) {
                        int index = iter->map[i] - iter->offset;
                        bnObject* obj = iter->pool + (OBJECT_MAXSIZE * index);
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
                bnObject* obj = self->pool + (OBJECT_MAXSIZE * tailPos);
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
                bnObject* obj = self->pool + (OBJECT_MAXSIZE * i);
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
                bnObject* obj = self->pool + (OBJECT_MAXSIZE * index);
                if (obj->freed) {
                        continue;
                }
                bnObject* tail = self->pool + (OBJECT_MAXSIZE * tailPos);
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
