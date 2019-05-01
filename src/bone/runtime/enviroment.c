#include "enviroment.h"

bnEnviroment* bnNewEnviroment() {
        bnEnviroment* ret = BN_MALLOC(sizeof(bnEnviroment));
        ret->binary = NULL;
        return ret;
}

void bnDeleteEnviroment(bnEnviroment* self) {
        g_list_free(self->binary);
        BN_FREE(self);
}