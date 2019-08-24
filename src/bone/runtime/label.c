#include "label.h"
#include "../bone.h"

bnLabel* bnNewLabel(int pos) {
        bnLabel* ret = BN_MALLOC(sizeof(bnLabel));
        ret->pos = pos;
        ret->refCount = 1;
        return ret;
}

void bnDeleteLabel(bnLabel* self) {
        self->refCount--;
        if (self->refCount == 0) {
                BN_FREE(self);
        }
}