#include "frame.h"
#include "../glib.h"
#include "../util/string_pool.h"
#include "array.h"
#include "heap.h"
#include "interpreter.h"
#include "object.h"
#include "snapshot.h"
#include "storage.h"

static void delete_snapshot(gpointer data);

bnFrame* bnNewFrame() {
        bnFrame* ret = BN_MALLOC(sizeof(bnFrame));
        ret->prev = NULL;
        ret->next = NULL;
        ret->hierarcySelf = bnNewStack();
        ret->vStack = bnNewStack();
        ret->currentCall = NULL;
        ret->depth = 0;
        ret->variableListExcludeOuter = g_ptr_array_new();
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

bnReference bnReturnValue(bnFrame* self) {
        if (bnGetStackSize(self->vStack) == 0) {
                return NULL;
        }
        return (bnReference)bnPopStack(self->vStack);
}

void bnInjectFrame(GHashTable* src, bnFrame* dst) {
        GHashTableIter iter;
        gpointer k, v;
        g_hash_table_iter_init(&iter, src);
        while (g_hash_table_iter_next(&iter, &k, &v)) {
                assert(v != NULL);
                g_hash_table_replace(dst->variableTable, k, v);
        }
}

bnReference bnExportAllVariable(bnInterpreter* bone, bnFrame* self) {
        bnReference arrRef =
            bnNewArray(bone, self->variableListExcludeOuter->len);
        bnObject* arr = bnGetObject(bone->heap, arrRef);
        int arrI = 0;
        for (int i = 0; i < self->variableListExcludeOuter->len; i++) {
                bnStringView name = (bnStringView)g_ptr_array_index(
                    self->variableListExcludeOuter, i);
                const char* nameStr = bnView2Str(bone->pool, name);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
                void* v = g_hash_table_lookup(self->variableTable,
                                              (gconstpointer)name);
#pragma clang diagnostic pop
                // _から始まる変数は公開されない仕様
                if (*nameStr == '_') {
                        continue;
                }
                bnStringView exportName =
                    bnGetExportVariableName(bone->pool, name);
                bnDefine(arr, exportName, v);
                bnSetArrayElementAt(arr, arrI, v);
                arrI++;
        }
        return arrRef;
}

void bnWriteVariable(bnFrame* frame, bnStringView name, bnReference ref) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
        assert(ref != NULL);
        g_hash_table_replace(frame->variableTable, (gpointer)name, ref);
#pragma clang diagnostic pop
}

void bnWriteVariable2(bnFrame* frame, struct bnStringPool* pool,
                      const char* name, bnReference ref) {
        assert(ref != NULL);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
        g_hash_table_replace(frame->variableTable,
                             (gpointer)bnIntern(pool, name), ref);
#pragma clang diagnostic pop
}

bnReference bnReadVariable(bnFrame* frame, bnStringView name) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
        return (bnReference)g_hash_table_lookup(frame->variableTable,
                                                (gpointer)name);
#pragma clang diagnostic pop
}

bnReference bnReadVariable2(bnFrame* frame, struct bnStringPool* pool,
                            const char* name) {
        return bnReadVariable(frame, bnIntern(pool, name));
}

void bnDeclareVariable(bnFrame* frame, bnStringView name) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
        for (int i = 0; i < frame->variableListExcludeOuter->len; i++) {
                bnStringView e = (bnStringView)g_ptr_array_index(
                    frame->variableListExcludeOuter, i);
                if (e == name) {
                        return;
                }
        }
        g_ptr_array_add(frame->variableListExcludeOuter,
                        GINT_TO_POINTER((int)name));
#pragma clang diagnostic pop
}

void bnDeleteFrame(bnFrame* self) {
        if (self == NULL) {
                return;
        }
        assert(self->next == NULL);
        if (self->prev != NULL) {
                self->prev->next = NULL;
        }
        bnDeleteStack(self->vStack, NULL);
        bnDeleteStack(self->hierarcySelf, NULL);
        g_ptr_array_free(self->variableListExcludeOuter, FALSE);
        g_hash_table_destroy(self->variableTable);
        g_list_free_full(self->snapshots, delete_snapshot);
        BN_FREE(self);
}

static void delete_snapshot(gpointer data) {
        bnDeleteSnapShot((bnSnapShot*)data);
}