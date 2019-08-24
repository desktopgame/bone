#include "any.h"
#include "interpreter.h"

void bnInitAny(bnInterpreter* bone, bnAny* self, const char* type) {
        bnInitObject(bone, &self->base, BN_OBJECT_ANY);
        self->type = bnIntern(bone->pool, type);
}

bool bnMatchType(bnInterpreter* bone, bnObject* self, const char* type) {
        if (self->type != BN_OBJECT_ANY) {
                return false;
        }
        bnAny* a = (bnAny*)self;
        return a->type == bnIntern(bone->pool, type);
}