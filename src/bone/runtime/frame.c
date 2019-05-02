#include "frame.h"

bnFrame* bnNewFrame() {
        bnFrame* ret = BN_MALLOC(sizeof(bnFrame));
        ret->prev = NULL;
        ret->next = NULL;
        ret->stack = NULL;
        ret->variableTable = g_hash_table_new(g_int_hash, g_int_equal);
        ret->pc = 0;
        return ret;
}

bnFrame* bnSubFrame(bnFrame* self) {
        assert(self->next == NULL);
        bnFrame* ret = bnNewFrame();
        self->next = ret;
        ret->prev = self;
        return ret;
}

void bnDeleteFrame(bnFrame* self) {
        assert(self->next == NULL);
        if (self->prev != NULL) {
                self->prev->next = NULL;
        }
        BN_FREE(self);
}