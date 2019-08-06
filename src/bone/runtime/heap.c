#include "heap.h"
#include "../bone.h"
#include "array.h"
#include "frame.h"
#include "interpreter.h"
#include "lambda.h"
#include "object.h"
#include "snapshot.h"

typedef struct bnHeap {
        GPtrArray* objects;
        GPtrArray* protected;
        int all;
        GRecMutex mutex;
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
        ret->objects = g_ptr_array_new_full(2, NULL);
        ret->protected = g_ptr_array_new_full(2, NULL);
        ret->all = 0;
        g_rec_mutex_init(&ret->mutex);
        return ret;
}

void bnAddToHeap(bnHeap* self, bnObject* obj) {
        g_rec_mutex_lock(&self->mutex);
        g_ptr_array_add(self->objects, obj);
        self->all++;
        g_rec_mutex_unlock(&self->mutex);
}

void bnGC(bnInterpreter* bone) {
        bnHeap* self = bone->heap;
        bnFrame* frame = bone->frame;
        g_rec_mutex_lock(&self->mutex);
        BN_CHECK_MEM();
        gc_clear(self, frame);
        gc_mark(self, frame);
        gc_mark_extern(bone);
        gc_mark_native(bone);
        gc_sweep(self, frame);
        BN_CHECK_MEM();
        g_rec_mutex_unlock(&self->mutex);
}

void bnDrop(bnHeap* self, bnObject* obj) {
        g_ptr_array_remove(self->objects, obj);
}

bnObject* bnProtect(bnHeap* self, bnObject* obj) {
        if (self->protected == NULL) {
                self->protected = g_ptr_array_new_full(2, NULL);
        }
        g_ptr_array_add(self->protected, obj);
        return obj;
}

void bnRelease(bnHeap* self) {
        g_ptr_array_free(self->protected, TRUE);
        self->protected = NULL;
}

void bnDeleteHeap(bnHeap* self) {
        g_rec_mutex_lock(&self->mutex);
        g_ptr_array_free(self->objects, TRUE);
        if (self->protected != NULL) {
                g_ptr_array_free(self->protected, TRUE);
                self->protected = NULL;
        }
        BN_FREE(self);
        g_rec_mutex_unlock(&self->mutex);
}

static void gc_clear(bnHeap* self, bnFrame* frame) {
        for (int i = 0; i < self->objects->len; i++) {
                bnObject* obj = g_ptr_array_index(self->objects, i);
                obj->mark = false;
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
        if (self->protected != NULL) {
                for (int i = 0; i < self->protected->len; i++) {
                        bnObject* obj = g_ptr_array_index(self->protected, i);
                        gc_mark_rec(obj);
                }
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
        GPtrArray* ret = g_ptr_array_new_full(2, NULL);
        int sweep = 0;
        for (int i = 0; i < self->objects->len; i++) {
                bnObject* a = g_ptr_array_index(self->objects, i);
                if (a->mark) {
                        g_ptr_array_add(ret, a);
                } else {
                        sweep++;
                        bnDeleteObject(a);
                }
        }
        g_ptr_array_free(self->objects, TRUE);
        self->objects = ret;
        self->all -= sweep;
}