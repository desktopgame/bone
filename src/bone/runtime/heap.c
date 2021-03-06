﻿#include "heap.h"
#include "../bone.h"
#include "array.h"
#include "frame.h"
#include "interpreter.h"
#include "lambda.h"
#include "object.h"
#include "snapshot.h"
#include "storage.h"
typedef struct stage {
        GPtrArray* objects;
} stage;

typedef struct bnHeap {
        bnStack* stages;
        bnStorage* storage;
        int all;
} bnHeap;

static void gc_clear(bnHeap* self, bnFrame* frame);
static void gc_mark_stage(bnHeap* self);
static void gc_mark_frame(bnHeap* self, bnFrame* frame);
static void gc_mark_extern(bnInterpreter* bone);
static void gc_mark_rec(bnHeap* self, bnReference ref);
static void gc_mark_array(bnHeap* self, bnObject* array);
static void gc_mark_lambda(bnHeap* self, bnObject* lambda);
static void gc_mark_shared(bnInterpreter* bone, bnHeap* self);
static void gc_sweep(bnHeap* self, bnFrame* frame);
static stage* new_stage();
static void delete_stage(stage* self);

bnHeap* bnNewHeap() {
        bnHeap* ret = BN_MALLOC(sizeof(bnHeap));
        ret->stages = bnNewStack();
        ret->all = 0;
        ret->storage = bnNewStorage(OBJECT_MAXSIZE, 128);
        return ret;
}
int* bnAllocObject(bnHeap* self) { return bnAllocMemory(self->storage); }

void* bnGetObject(struct bnHeap* self, bnReference ref) {
        return bnGetMemory(self->storage, ref);
}

void bnFreeObject(bnHeap* self, bnReference ref) {
        bnFreeMemory(self->storage, ref);
}

void bnPushStage(bnHeap* self) { bnPushStack(self->stages, new_stage()); }

bnReference bnStaging(bnHeap* self, bnReference ref) {
        stage* st = bnPeekStack(self->stages);
        g_ptr_array_add(st->objects, ref);
        return ref;
}

void bnPopStage(bnHeap* self) { delete_stage(bnPopStack(self->stages)); }

void bnGC(bnInterpreter* bone) {
        bnHeap* self = bone->heap;
        bnFrame* frame = bone->frame;
        BN_CHECK_MEM();
        gc_clear(self, frame);
        gc_mark_stage(self);
        gc_mark_frame(self, frame);
        gc_mark_extern(bone);
        gc_mark_shared(bone, self);
        gc_sweep(self, frame);
        BN_CHECK_MEM();
        bnCompact(self->storage);
}

bnStorage* bnGetHeapStorage(bnHeap* self) { return self->storage; }

void bnDeleteHeap(bnHeap* self) {
        bnDeleteStack(self->stages, NULL);
        bnDeleteStorage(self->storage);
        BN_FREE(self);
}

static void gc_clear(bnHeap* self, bnFrame* frame) {
        bnStorage* iter = self->storage;
        while (iter != NULL) {
                for (int i = 0; i < self->storage->objectCount; i++) {
                        bnObject* obj =
                            (bnObject*)(iter->pool +
                                        (self->storage->objectSize * i));
                        bool* bit = iter->bitmap + (sizeof(bool) * i);
                        if (!(*bit)) {
                                obj->mark = false;
                        }
                }
                iter = iter->next;
        }
}

static void gc_mark_stage(bnHeap* self) {
        bnStackElement* stackIter = self->stages->head;
        while (stackIter != NULL) {
                GPtrArray* ary = ((stage*)stackIter->value)->objects;
                for (int i = 0; i < ary->len; i++) {
                        gc_mark_rec(self, g_ptr_array_index(ary, i));
                }
                stackIter = stackIter->next;
        }
}

static void gc_mark_frame(bnHeap* self, bnFrame* frame) {
        if (frame == NULL) {
                return;
        }
        if (frame->next != NULL) {
                gc_mark_frame(self, frame->next);
        }
        //全てのローカル変数をマークする
        GHashTableIter hashIter;
        gpointer k, v;
        g_hash_table_iter_init(&hashIter, frame->variableTable);
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                gc_mark_rec(self, v);
        }
        //遅延実行ブロックを登録した時点での
        //変数をマークする
        GList* defIter = frame->snapshots;
        while (defIter != NULL) {
                bnSnapShot* sn = defIter->data;
                g_hash_table_iter_init(&hashIter, sn->table);
                while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                        gc_mark_rec(self, v);
                }
                defIter = defIter->next;
        }
        //スタック領域をマークする
        bnStackElement* stackE = frame->vStack->head;
        while (stackE != NULL) {
                gc_mark_rec(self, stackE->value);
                stackE = stackE->next;
        }
        if (frame->panic != NULL) {
                gc_mark_rec(self, frame->panic);
        }
}

static void gc_mark_extern(bnInterpreter* bone) {
        GHashTableIter hashIter;
        gpointer k, v;
        g_hash_table_iter_init(&hashIter, bone->externTable);
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                gc_mark_rec(bone->heap, v);
        }
}

static void gc_mark_rec(bnHeap* self, bnReference ref) {
        bnObject* obj = bnGetObject(self, ref);
        if (obj->mark || obj == NULL) {
                return;
        }
        obj->mark = true;
        GHashTableIter hashIter;
        gpointer k, v;
        g_hash_table_iter_init(&hashIter, obj->table);
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                gc_mark_rec(self, v);
        }
        if (obj->type == BN_OBJECT_ARRAY) {
                gc_mark_array(self, obj);
        } else if (obj->type == BN_OBJECT_LAMBDA) {
                gc_mark_lambda(self, obj);
        }
}

static void gc_mark_array(bnHeap* self, bnObject* array) {
        for (int i = 0; i < bnGetArrayLength(array); i++) {
                gc_mark_rec(self, bnGetArrayElementAt(array, i));
        }
}

static void gc_mark_lambda(bnHeap* self, bnObject* lambda) {
        if (bnGetLambdaType(lambda) != BN_LAMBDA_SCRIPT) {
                return;
        }
        GHashTableIter hashIter;
        g_hash_table_iter_init(&hashIter, bnGetCapturedMap(lambda));
        gpointer k, v;
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                gc_mark_rec(self, v);
        }
}

static void gc_mark_shared(bnInterpreter* bone, bnHeap* self) {
        GHashTableIter hashIter;
        gpointer k, v;
        g_hash_table_iter_init(&hashIter, bone->sharedTable);
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                gc_mark_rec(self, (bnReference)v);
        }
}

static void gc_sweep(bnHeap* self, bnFrame* frame) {
        bnStorage* iter = self->storage;
        while (iter != NULL) {
                for (int i = 0; i < self->storage->objectCount; i++) {
                        bnReference ref = iter->table + i;
                        int index = (*ref) - iter->offset;
                        bnObject* obj =
                            (bnObject*)(iter->pool +
                                        (self->storage->objectSize * index));
                        bool* bit = iter->bitmap + (sizeof(bool) * index);
                        if (!obj->mark && !(*bit)) {
                                bnDeleteObject(self->storage, ref, obj);
                        }
                }
                iter = iter->next;
        }
}
static stage* new_stage() {
        stage* ret = BN_MALLOC(sizeof(stage));
        ret->objects = g_ptr_array_new();
        return ret;
}

static void delete_stage(stage* self) {
        g_ptr_array_free(self->objects, FALSE);
        BN_FREE(self);
}
