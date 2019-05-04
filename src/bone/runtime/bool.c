#include "bool.h"
#include "interpreter.h"

bnBool* bnNewBool(bnInterpreter* bone, bool value) {
        bnBool* ret = BN_MALLOC(sizeof(bnBool));
        bnInitObject(bone->heap, &ret->base, BN_OBJECT_BOOL);
        ret->value = value;
        return ret;
}

void bnDeleteBool(bnBool* self) { bnDeleteObject(self); }