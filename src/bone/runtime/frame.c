#include "frame.h"

bnFrame* bnNewFrame() {
        bnFrame* ret = BN_MALLOC(sizeof(bnFrame));
        ret->stack = NULL;
        ret->variableTable = g_hash_table_new(g_int_hash, g_int_equal);
        ret->pc = 0;
        return ret;
}

void bnDeleteFrame(bnFrame* self) { BN_FREE(self); }