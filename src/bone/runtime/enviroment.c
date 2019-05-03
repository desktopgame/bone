#include "enviroment.h"

bnEnviroment* bnNewEnviroment() {
        bnEnviroment* ret = BN_MALLOC(sizeof(bnEnviroment));
        ret->codeArray = g_ptr_array_new();
        return ret;
}

void bnDeleteEnviroment(bnEnviroment* self) {
        g_ptr_array_unref(self->codeArray);
        BN_FREE(self);
}