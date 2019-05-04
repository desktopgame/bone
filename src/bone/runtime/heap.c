#include "heap.h"
#include "../bone.h"
#include "array.h"
#include "frame.h"
#include "lambda.h"
#include "object.h"

typedef struct bnHeap {
        GList* objects;
        int limit;
} bnHeap;

static void gc_clear(bnHeap* self, bnFrame* frame);
static void gc_mark(bnHeap* self, bnFrame* frame);
static void gc_mark_rec(bnObject* obj);
static void gc_mark_array(bnArray* array);
static void gc_mark_lambda(bnLambda* lambda);

bnHeap* bnNewHeap() {
        bnHeap* ret = BN_MALLOC(sizeof(bnHeap));
        ret->objects = NULL;
        ret->limit = 100;
        return ret;
}

void bnAddToHeap(bnHeap* self, bnObject* obj) {
        self->objects = g_list_append(self->objects, obj);
}

void bnGC(bnHeap* self, bnFrame* frame) {
        gc_clear(self, frame);
        gc_mark(self, frame);
}

void bnDeleteHeap(bnHeap* self) {
        g_list_free(self->objects);
        BN_FREE(self);
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