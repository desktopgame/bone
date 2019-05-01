#include "bool.h"

bnBool* bnNewBool(bool value) {
        bnBool* ret = BN_MALLOC(sizeof(bnBool));
        bnInitObject(&ret->base);
        ret->value = value;
        return ret;
}

void bnDeleteBool(bnBool* self) { bnDeleteObject(self); }