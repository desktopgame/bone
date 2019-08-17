#include "storage.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "memory.h"
#include "object.h"

static bnReference find_free_object(bnStorage* self);
static int rfind_free_object_pos(bnStorage* self, int cur);
static void realloc_storage(bnStorage* self);
static void clear_storage(bnStorage* self);
static void compact_impl(bnStorage* self);

bnStorage* bnNewStorage() {
        bnStorage* ret = BN_MALLOC(sizeof(bnStorage));
        ret->capacity = 100;
        ret->pool = BN_MALLOC(OBJECT_MAXSIZE * ret->capacity);
        ret->map = BN_MALLOC(sizeof(int) * ret->capacity);
        ret->use = 0;
        clear_storage(ret);
        return ret;
}

bnReference bnAllocMemory(bnStorage* self) {
        bnReference ret = find_free_object(self);
        if (ret == NULL) {
                realloc_storage(self);
                return bnAllocMemory(self);
        }
        self->use++;
        int i = *ret;
        bnObject* obj = self->pool + (OBJECT_MAXSIZE * i);
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
        bnObject* obj = self->pool + (OBJECT_MAXSIZE * i);
        return obj;
}

void bnCompact(bnStorage* self) { compact_impl(self); }

void bnDeleteStorage(bnStorage* self) {
        BN_FREE(self->pool);
        BN_FREE(self->map);
        BN_FREE(self);
}
// private
static bnReference find_free_object(bnStorage* self) {
        for (int i = 0; i < self->capacity; i++) {
                int index = self->map[i];
                bnObject* obj = self->pool + (OBJECT_MAXSIZE * index);
                if (obj->freed) {
                        bnReference ref = self->map + i;
                        int vref = *ref;
                        assert(vref == index);
                        return ref;
                }
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

static void realloc_storage(bnStorage* self) {
        // reallocate pointers
        int oldcapa = self->capacity;
        int newcapa = self->capacity + (self->capacity / 2);
        void* newpool = BN_REALLOC(self->pool, OBJECT_MAXSIZE * newcapa);
        bnReference newmap = BN_REALLOC(self->map, sizeof(int) * newcapa);
        assert(newpool != NULL);
        assert(newmap != NULL);
        self->pool = newpool;
        self->map = newmap;
        self->capacity = newcapa;
        memset(self->pool + (OBJECT_MAXSIZE * oldcapa), 0,
               OBJECT_MAXSIZE * (newcapa - oldcapa));
        for (int i = oldcapa; i < newcapa; i++) {
                self->map[i] = i;
                bnObject* obj = self->pool + (OBJECT_MAXSIZE * i);
                obj->freed = true;
        }
        // bnCompact(self);
}

static void clear_storage(bnStorage* self) {
        memset(self->pool, 0, OBJECT_MAXSIZE * self->capacity);
        memset(self->map, 0, sizeof(int) * self->capacity);
        for (int i = 0; i < self->capacity; i++) {
                bnObject* obj = self->pool + (OBJECT_MAXSIZE * i);
                self->map[i] = i;
                obj->freed = true;
        }
}

static void compact_impl(bnStorage* self) {
        int tailPos = rfind_free_object_pos(self, self->capacity - 1);
        if (!tailPos) {
                return;
        }
        for (int i = 0; i < self->capacity; i++) {
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
