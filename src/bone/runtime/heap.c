#include "heap.h"
#include "../bone.h"
#include "array.h"
#include "frame.h"
#include "lambda.h"
#include "object.h"

static GRecMutex gHeapMtx;

typedef struct bnHeap {
        GList* objects;
        int limit;
        int all;
} bnHeap;

static void gc_clear(bnHeap* self, bnFrame* frame);
static void gc_mark(bnHeap* self, bnFrame* frame);
static void gc_mark_rec(bnObject* obj);
static void gc_mark_array(bnArray* array);
static void gc_mark_lambda(bnLambda* lambda);
static void gc_sweep(bnHeap* self, bnFrame* frame);

bnHeap* bnNewHeap() {
        bnHeap* ret = BN_MALLOC(sizeof(bnHeap));
        ret->objects = NULL;
        ret->limit = 100;
        ret->all = 0;
        return ret;
}

void bnAddToHeap(bnHeap* self, bnObject* obj) {
        g_rec_mutex_lock(&gHeapMtx);
        self->objects = g_list_append(self->objects, obj);
        self->all++;
        g_rec_mutex_unlock(&gHeapMtx);
}

void bnGC(bnHeap* self, bnFrame* frame) {
        g_rec_mutex_lock(&gHeapMtx);
        gc_clear(self, frame);
        gc_mark(self, frame);
        gc_sweep(self, frame);
        g_rec_mutex_unlock(&gHeapMtx);
}

void bnDeleteHeap(bnHeap* self) {
        g_rec_mutex_lock(&gHeapMtx);
        g_list_free(self->objects);
        BN_FREE(self);
        g_rec_mutex_unlock(&gHeapMtx);
}

static void gc_clear(bnHeap* self, bnFrame* frame) {
        GList* iter = self->objects;
        while (iter != NULL) {
                bnObject* obj = iter->data;
                obj->mark = false;
                iter = iter->next;
        }
}

static void gc_mark(bnHeap* self, bnFrame* frame) {
        if (frame == NULL) {
                return;
        }
        if (frame->next != NULL) {
                gc_mark(self, frame->next);
        }
        GHashTableIter hashIter;
        gpointer k, v;
        g_hash_table_iter_init(&hashIter, frame->variableTable);
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                gc_mark_rec(v);
        }
        bnStackElement* stackE = frame->vStack->head;
        while (stackE != NULL) {
                gc_mark_rec(stackE->value);
                stackE = stackE->next;
        }
        if (frame->panic) {
                gc_mark_rec(frame->panic);
        }
}

static void gc_mark_rec(bnObject* obj) {
        if (obj->mark || obj == NULL) {
                return;
        }
        obj->mark = true;
        GHashTableIter hashIter;
        gpointer k, v;
        g_hash_table_iter_init(&hashIter, obj->table);
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                gc_mark_rec(v);
        }
        if (obj->type == BN_OBJECT_ARRAY) {
                gc_mark_array(obj);
        } else if (obj->type == BN_OBJECT_LAMBDA) {
                gc_mark_lambda(obj);
        }
}

static void gc_mark_array(bnArray* array) {
        for (int i = 0; i < array->size; i++) {
                gc_mark_rec(g_ptr_array_index(array->arr, i));
        }
}

static void gc_mark_lambda(bnLambda* lambda) {
        if (lambda->type != BN_LAMBDA_SCRIPT) {
                return;
        }
        GHashTableIter hashIter;
        g_hash_table_iter_init(&hashIter, lambda->outer);
        gpointer k, v;
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                gc_mark_rec(v);
        }
}

static void gc_sweep(bnHeap* self, bnFrame* frame) {
        GList* ret = NULL;
        GList* iter = self->objects;
        int sweep = 0;
        while (iter != NULL) {
                bnObject* a = iter->data;
                if (a->mark) {
                        ret = g_list_append(ret, a);
                } else {
                        sweep++;
                        BN_FREE(a);
                }
                iter->data = NULL;
                iter = iter->next;
        }
        g_list_free(self->objects);
        self->objects = ret;
        self->all -= sweep;
#if DEBUG
        printf("sweep %d\n", sweep);
        printf("all %d\n", self->all);
#endif
}