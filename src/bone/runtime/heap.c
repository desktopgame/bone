#include "heap.h"
#include "../bone.h"
#include "array.h"
#include "frame.h"
#include "interpreter.h"
#include "lambda.h"
#include "object.h"
#include "snapshot.h"

static GRecMutex gHeapMtx;

typedef struct bnHeap {
        GList* objects;
        GList* protected;
        int all;
} bnHeap;

static void gc_clear(bnHeap* self, bnFrame* frame);
static void gc_mark(bnHeap* self, bnFrame* frame);
static void gc_mark_extern(bnInterpreter* bone);
static void gc_mark_native(bnInterpreter* bone);
static void gc_mark_rec(bnObject* obj);
static void gc_mark_array(bnArray* array);
static void gc_mark_lambda(bnLambda* lambda);
static void gc_sweep(bnHeap* self, bnFrame* frame);

bnHeap* bnNewHeap() {
        bnHeap* ret = BN_MALLOC(sizeof(bnHeap));
        ret->objects = NULL;
        ret->protected = NULL;
        ret->all = 0;
        return ret;
}

void bnAddToHeap(bnHeap* self, bnObject* obj) {
        g_rec_mutex_lock(&gHeapMtx);
        self->objects = g_list_append(self->objects, obj);
        self->all++;
        g_rec_mutex_unlock(&gHeapMtx);
}

void bnGC(bnInterpreter* bone) {
        bnHeap* self = bone->heap;
        bnFrame* frame = bone->frame;
        g_rec_mutex_lock(&gHeapMtx);
        gc_clear(self, frame);
        gc_mark(self, frame);
        gc_mark_extern(bone);
        gc_mark_native(bone);
        gc_sweep(self, frame);
        g_rec_mutex_unlock(&gHeapMtx);
}

void bnDrop(bnHeap* self, bnObject* obj) { g_list_remove(self->objects, obj); }

bnObject* bnProtect(bnHeap* self, bnObject* obj) {
        self->protected = g_list_append(self->protected, obj);
        return obj;
}

void bnRelease(bnHeap* self) {
        g_list_free(self->protected);
        self->protected = NULL;
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
        // mark local variable
        GHashTableIter hashIter;
        gpointer k, v;
        g_hash_table_iter_init(&hashIter, frame->variableTable);
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                gc_mark_rec(v);
        }
        // mark defer
        GList* defIter = frame->snapshots;
        while (defIter != NULL) {
                bnSnapShot* sn = defIter->data;
                g_hash_table_iter_init(&hashIter, sn->table);
                while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                        gc_mark_rec(v);
                }
                defIter = defIter->next;
        }
        // mark stack
        bnStackElement* stackE = frame->vStack->head;
        while (stackE != NULL) {
                gc_mark_rec(stackE->value);
                stackE = stackE->next;
        }
        if (frame->panic != NULL) {
                gc_mark_rec(frame->panic);
        }
        // mark protected
        GList* proIter = self->protected;
        while (proIter != NULL) {
                bnObject* obj = proIter->data;
                gc_mark_rec(obj);
                proIter = proIter->next;
        }
}

static void gc_mark_extern(bnInterpreter* bone) {
        GHashTableIter hashIter;
        gpointer k, v;
        g_hash_table_iter_init(&hashIter, bone->externTable);
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                const char* str = bnView2Str(bone->pool, k);
                bnObject* obj = v;
                gc_mark_rec(v);
        }
}

static void gc_mark_native(bnInterpreter* bone) {
        GList* iter = bone->nativeAlloc;
        while (iter != NULL) {
                gc_mark_rec(iter->data);
                iter = iter->next;
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
                        bnDeleteObject(a);
                }
                iter->data = NULL;
                iter = iter->next;
        }
        g_list_free(self->objects);
        self->objects = ret;
        self->all -= sweep;
}