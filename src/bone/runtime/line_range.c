#include "line_range.h"
#include "../util/memory.h"

bnLineRange* bnNewLineRange() {
        bnLineRange* ret = BN_MALLOC(sizeof(bnLineRange));
        ret->start = -1;
        ret->end = -1;
        ret->line = -1;
        return ret;
}

void bnDeleteLineRange(bnLineRange* self) { BN_FREE(self); }