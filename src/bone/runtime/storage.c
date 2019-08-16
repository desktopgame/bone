#include "storage.h"
#include <stdbool.h>
#include "memory.h"
#include "object.h"

static bnObject* find_free_object(bnStorage* self);

bnStorage* bnNewStorage() {
        bnStorage* ret = BN_MALLOC(sizeof(bnStorage));
        ret->pool = BN_MALLOC(OBJECT_MAXSIZE * OBJECT_COUNT);
        ret->next = NULL;
        for (int i = 0; i < OBJECT_COUNT; i++) {
                bnObject* obj = ret->pool + (OBJECT_MAXSIZE * i);
                obj->freed = true;
        }
        return ret;
}

void* bnAllocMemory(bnStorage* self) {
        bnObject* ret = find_free_object(self);
        if (ret == NULL) {
                bnStorage* storage = bnNewStorage();
                self->next = storage;
                return bnAllocMemory(storage);
        }
        ret->freed = false;
        return ret;
}

void bnFreeMemory(bnStorage* self, void* memory) {
        bnObject* obj = memory;
        obj->freed = true;
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
static bnObject* find_free_object(bnStorage* self) {
        bnStorage* iter = self;
        while (iter != NULL) {
                for (int i = 0; i < OBJECT_COUNT; i++) {
                        bnObject* obj = self->pool + (OBJECT_MAXSIZE * i);
                        if (obj->freed) {
                                return obj;
                        }
                }
                iter = iter->next;
        }
        return NULL;
}