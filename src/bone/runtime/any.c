#include "any.h"
#include "interpreter.h"

void bnInitAny(bnInterpreter* bone, bnAny* self, const char* type) {
        bnInitObject(bone, &self->base, BN_OBJECT_ANY);
        self->type = bnIntern(bone->pool, type);
}

bool bnMatchType(bnInterpreter* bone, bnAny* self, const char* type) {
        return self->type == bnIntern(bone->pool, type);
}