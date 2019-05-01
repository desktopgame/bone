#include "double.h"

bnDouble* bnNewDouble(double value) {
        bnDouble* ret = BN_MALLOC(sizeof(bnDouble));
        bnInitObject(&ret->base);
        ret->value = value;
        return ret;
}

void bnDeleteDouble(bnDouble* self) { bnDeleteObject(self); }