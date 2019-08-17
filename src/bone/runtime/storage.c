#include "storage.h"
#include <stdbool.h>
#include <string.h>
#include "memory.h"
#include "object.h"

#define SEARCH_L2R (0)
#define SEARCH_R2L (1)

static int* use_free_object(bnStorage* self);
static bnStorage* append_storage(bnStorage* self);
static void compact_impl(bnStorage* self);
static int get_free_from_last(bnStorage* self, int cur);

bnStorage* bnNewStorage(int offset) {
        bnStorage* ret = BN_MALLOC(sizeof(bnStorage));
        ret->pool = BN_MALLOC(OBJECT_MAXSIZE * OBJECT_COUNT);
        ret->prev = NULL;
        ret->next = NULL;
        ret->offset = offset;
        ret->use = 0;
        ret->searchRule = SEARCH_L2R;
        for (int i = 0; i < OBJECT_COUNT; i++) {
                bnObject* obj = ret->pool + (OBJECT_MAXSIZE * i);
                obj->freed = true;
                ret->map[i] = offset + i;
        }
        return ret;
}

int* bnAllocMemory(bnStorage* self) {
        int* ret = use_free_object(self);
        if (ret == NULL) {
                return bnAllocMemory(append_storage(self));
        }
        return ret;
}

void bnFreeMemory(bnStorage* self, int* index) {
        int i;
        bnStorage* storage = bnGetStorage(self, index, &i);
        bnObject* obj = bnGetMemory(self, index);
        storage->use--;
        obj->freed = true;
}

void* bnGetMemory(bnStorage* self, int* index) {
        int i;
        bnStorage* storage = bnGetStorage(self, index, &i);
        return storage->pool + (OBJECT_MAXSIZE * storage->map[i]);
}

bnStorage* bnGetStorage(bnStorage* head, int* index, int* fixedPos) {
        int i = *index;
        bnStorage* iter = head;
        while (i >= OBJECT_COUNT) {
                i -= OBJECT_COUNT;
                iter = iter->next;
        }
        return iter;
}

void bnCompact(bnStorage* self) {
        bnStorage* iter = self;
        while (iter != NULL) {
                compact_impl(iter);
                iter = iter->next;
        }
}

void bnDeleteStorage(bnStorage* self) {
        if (self->next != NULL) {
                bnDeleteStorage(self->next);
                self->next = NULL;
        }
        BN_FREE(self->pool);
        BN_FREE(self);
}
// private
static int* use_free_object(bnStorage* self) {
        bnStorage* iter = self;
        while (iter != NULL) {
                for (int i = 0; i < OBJECT_COUNT; i++) {
                        int index = self->map[i] - self->offset;
                        bnObject* obj = self->pool + (OBJECT_MAXSIZE * index);
                        if (obj->freed) {
                                iter->use++;
                                obj->freed = false;
                                return self->map + (sizeof(int) * index);
                        }
                }
                iter = iter->next;
        }
        return NULL;
}

static bnStorage* append_storage(bnStorage* self) {
        if (self->next == NULL) {
                bnStorage* storage = bnNewStorage(self->offset + OBJECT_COUNT);
                self->next = storage;
                return storage;
        } else {
                return append_storage(self->next);
        }
}

static void compact_impl(bnStorage* self) {
        int tailPos = get_free_from_last(self, OBJECT_COUNT - 1);
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
                tailPos = get_free_from_last(self, tailPos);
                if (!tailPos) {
                        break;
                }
        }
}

static int get_free_from_last(bnStorage* self, int cur) {
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