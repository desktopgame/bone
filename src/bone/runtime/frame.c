#include "frame.h"
#include "array.h"
#include "interpreter.h"
#include "object.h"
#include "snapshot.h"

bnFrame* bnNewFrame() {
        bnFrame* ret = BN_MALLOC(sizeof(bnFrame));
        ret->prev = NULL;
        ret->next = NULL;
        ret->hierarcySelf = bnNewStack();
        ret->vStack = bnNewStack();
        ret->currentCall = NULL;
        ret->variableTable =
            g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, NULL);
        ret->pc = 0;
        ret->panicName = 0;
        ret->panic = NULL;
        ret->snapshots = NULL;
        return ret;
}

bnFrame* bnSubFrame(bnFrame* self) {
        assert(self->next == NULL);
        bnFrame* ret = bnNewFrame();
        self->next = ret;
        ret->prev = self;
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
        bnArray* arr = bnNewArray(bone, g_hash_table_size(self->variableTable));
        GHashTableIter iter;
        g_hash_table_iter_init(&iter, self->variableTable);
        gpointer k, v;
        int arrI = 0;
        while (g_hash_table_iter_next(&iter, &k, &v)) {
                bnStringView retName = k;
                bnStringView exportName =
                    bnGetExportVariableName(bone->pool, retName);
                // create private member
                g_hash_table_replace(arr->base.table, exportName, v);
                g_ptr_array_index(arr->arr, arrI) = v;
                arrI++;
        }
        return arr;
}

void bnDeleteFrame(bnFrame* self) {
        assert(self->next == NULL);
        if (self->prev != NULL) {
                self->prev->next = NULL;
        }
        bnDeleteStack(self->vStack, NULL);
        bnDeleteStack(self->hierarcySelf, NULL);
        g_hash_table_destroy(self->variableTable);
        g_list_free_full(self->snapshots, bnDeleteSnapShot);
        BN_FREE(self);
}