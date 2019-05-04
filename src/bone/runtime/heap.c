#include "heap.h"
#include "../bone.h"
#include "object.h"

typedef struct bnHeap {
        GList* objects;
        int limit;
} bnHeap;

bnHeap* bnNewHeap() {
        bnHeap* ret = BN_MALLOC(sizeof(bnHeap));
        ret->objects = NULL;
        ret->limit = 100;
        return ret;
}

void bnAddToHeap(bnHeap* self, bnObject* obj) {
        self->objects = g_list_append(self->objects, obj);
}

void bnDeleteHeap(bnHeap* self) {
        g_list_free(self->objects);
        BN_FREE(self);
}