#include "compile_cache.h"
#include "../util/memory.h"

bnCompileCache* bnNewCompileCache() {
        bnCompileCache* ret = BN_MALLOC(sizeof(bnCompileCache));
        ret->whileStartStack = bnNewStack();
        ret->whileEndStack = bnNewStack();
        return ret;
}

void bnDeleteCompileCache(bnCompileCache* self) {
        bnDeleteStack(self->whileStartStack, NULL);
        bnDeleteStack(self->whileEndStack, NULL);
        BN_FREE(self);
}