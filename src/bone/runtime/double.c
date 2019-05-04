#include "double.h"
#include "interpreter.h"

bnDouble* bnNewDouble(bnInterpreter* bone, double value) {
        bnDouble* ret = BN_MALLOC(sizeof(bnDouble));
        bnInitObject(bone->heap, &ret->base, BN_OBJECT_DOUBLE);
        ret->value = value;
        return ret;
}

void bnDeleteDouble(bnDouble* self) { bnDeleteObject(self); }