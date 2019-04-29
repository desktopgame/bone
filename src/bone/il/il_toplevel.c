#include "il_toplevel.h"

bnILToplevel* bnNewILTopLevel() {
        bnILToplevel* ret = BN_MALLOC(sizeof(bnILToplevel));
        ret->statementList = NULL;
        return ret;
}

void bnDeleteILTopLevel(bnILToplevel* self) { BN_FREE(self); }