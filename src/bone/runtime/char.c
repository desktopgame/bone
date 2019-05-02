#include "char.h"

bnChar* bnNewChar(char value) {
        bnChar* ret = BN_MALLOC(sizeof(bnChar));
        bnInitObject(&ret->base, BN_OBJECT_CHAR);
        ret->value = value;
        return ret;
}

void bnDeleteChar(bnChar* self) { bnDeleteObject(self); }