#include "char.h"
#include "interpreter.h"

bnChar* bnNewChar(bnInterpreter* bone, char value) {
        bnChar* ret = BN_MALLOC(sizeof(bnChar));
        bnInitObject(bone->heap, &ret->base, BN_OBJECT_CHAR);
        ret->value = value;
        return ret;
}
