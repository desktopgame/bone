#include "string.h"

bnString* bnNewString(bnInterpreter* bone, bnStringView value) {
        bnString* ret = BN_MALLOC(sizeof(bnString));
        bnInitObject(&ret->base, BN_OBJECT_STRING);
        ret->value = value;
        return ret;
}

void bnDeleteString(bnString* self) {}