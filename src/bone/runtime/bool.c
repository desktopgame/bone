#include "bool.h"

bnBool* bnNewBool(bool value) {
        bnBool* ret = BN_MALLOC(sizeof(bnBool));
        bnInitObject(&ret->base, BN_OBJECT_BOOL);
        ret->value = value;
        return ret;
}

void bnDeleteBool(bnBool* self) { bnDeleteObject(self); }