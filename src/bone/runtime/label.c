#include "label.h"
#include "../bone.h"

bnLabel* bnNewLabel(int pos) {
        bnLabel* ret = BN_MALLOC(sizeof(bnLabel));
        ret->pos = pos;
        return ret;
}

void bnDeleteLabel(bnLabel* self) { BN_FREE(self); }