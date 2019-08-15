#include "frame.h"
#include "../util/string_pool.h"
#include "array.h"
#include "interpreter.h"
#include "object.h"
#include "snapshot.h"

static void delete_snapshot(gpointer data);

bnFrame* bnNewFrame() {
        bnFrame* ret = BN_MALLOC(sizeof(bnFrame));
        ret->prev = NULL;
        ret->next = NULL;
        ret->hierarcySelf = bnNewStack();
        ret->vStack = bnNewStack();
        ret->currentCall = NULL;
        ret->depth = 0;
        ret->variableTable =
            g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, NULL);
        ret->pc = 0;
        ret->panic = NULL;
        ret->snapshots = NULL;
        return ret;
}

bnFrame* bnSubFrame(bnFrame* self) {
        assert(self->next == NULL);
        bnFrame* ret = bnNewFrame();
        self->next = ret;
        ret->prev = self;
        ret->depth = self->depth + 1;
        return ret;
}

bnObject* bnReturnValue(bnFrame* self) {
        if (bnGetStackSize(self->vStack) == 0) {
                return NULL;
        }
        return bnPopStack(self->vStack);
}

void bnInjectFrame(GHashTable* src, bnFrame* dst) {
        GHashTableIter iter;
        gpointer k, v;
        g_hash_table_iter_init(&iter, src);
        while (g_hash_table_iter_next(&iter, &k, &v)) {
                g_hash_table_replace(dst->variableTable, k, v);
        }
}

bnObject* bnExportAllVariable(bnInterpreter* bone, bnFrame* self) {
        bnObject* arr =
            bnNewArray(bone, g_hash_table_size(self->variableTable));
        GHashTableIter iter;
        g_hash_table_iter_init(&iter, self->variableTable);
        gpointer k, v;
        int arrI = 0;
        while (g_hash_table_iter_next(&iter, &k, &v)) {
                bnStringView retName = (bnStringView)k;
                const char* retNameStr = bnView2Str(bone->pool, retName);
                if (*retNameStr == '_') {
                        continue;
                }
                bnStringView exportName =
                    bnGetExportVariableName(bone->pool, retName);
                // create private member
                bnDefine(arr, exportName, v);
                bnSetArrayElementAt(arr, arrI, v);
                arrI++;
        }
        return arr;
}

void bnWriteVariable(bnFrame* frame, bnStringView name, bnObject* obj) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
        g_hash_table_replace(frame->variableTable, (gpointer)name, obj);
#pragma clang diagnostic pop
}

void bnWriteVariable2(bnFrame* frame, struct bnStringPool* pool,
                      const char* name, bnObject* obj) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
        g_hash_table_replace(frame->variableTable,
                             (gpointer)bnIntern(pool, name), obj);
#pragma clang diagnostic pop
}

bnObject* bnReadVariable(bnFrame* frame, bnStringView name) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
        return (bnObject*)g_hash_table_lookup(frame->variableTable,
                                              (gpointer)name);
#pragma clang diagnostic pop
}

bnObject* bnReadVariable2(bnFrame* frame, struct bnStringPool* pool,
                          const char* name) {
        return bnReadVariable(frame, bnIntern(pool, name));
}

void bnDeleteFrame(bnFrame* self) {
        assert(self->next == NULL);
        if (self->prev != NULL) {
                self->prev->next = NULL;
        }
        bnDeleteStack(self->vStack, NULL);
        bnDeleteStack(self->hierarcySelf, NULL);
        g_hash_table_destroy(self->variableTable);
        g_list_free_full(self->snapshots, delete_snapshot);
        BN_FREE(self);
}

static void delete_snapshot(gpointer data) {
        bnDeleteSnapShot((bnSnapShot*)data);
}