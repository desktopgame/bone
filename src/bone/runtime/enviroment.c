#include "enviroment.h"

bnEnviroment* bnNewEnviroment() {
        bnEnviroment* ret = BN_MALLOC(sizeof(bnEnviroment));
        ret->codeArray = g_ptr_array_new();
        ret->labels = g_ptr_array_new();
        g_ptr_array_set_free_func(ret->labels, free);
        return ret;
}

bnLabel* bnGenerateLabel(bnEnviroment* env, int pos) {
        bnLabel* lb = bnNewLabel(pos);
        g_ptr_array_add(env->codeArray, lb);
        return lb;
}

int bnGenerateNOP(bnEnviroment* env) {
        int ret = env->codeArray->len;
        g_ptr_array_add(env->codeArray, BN_OP_NOP);
        return ret;
}

void bnDeleteEnviroment(bnEnviroment* self) {
        g_ptr_array_unref(self->codeArray);
        g_ptr_array_unref(self->labels);
        BN_FREE(self);
}