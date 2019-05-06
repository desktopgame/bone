#include "enviroment.h"

bnEnviroment* bnNewEnviroment() {
        bnEnviroment* ret = BN_MALLOC(sizeof(bnEnviroment));
        ret->codeArray = g_ptr_array_new();
        ret->labels = g_ptr_array_new();
        ret->labelFixStack = bnNewStack();
        g_ptr_array_set_free_func(ret->labels, free);
        return ret;
}

bnLabel* bnGenerateLabel(bnEnviroment* self, int pos) {
        if (self->labelFixStack->head != NULL) {
                pos -= ((int)bnPeekStack(self->labelFixStack));
        }
        bnLabel* lb = bnNewLabel(pos);
        g_ptr_array_add(self->codeArray, lb);
        return lb;
}

int bnGenerateNOP(bnEnviroment* self) {
        int ret = self->codeArray->len;
        g_ptr_array_add(self->codeArray, BN_OP_NOP);
        return ret;
}

void bnGenerateEnterLambda(bnEnviroment* self) {
        bnPushStack(self->labelFixStack, self->codeArray->len);
}

void bnGenerateExitLambda(bnEnviroment* self) {
        bnPopStack(self->labelFixStack);
}

void bnGenerateFillNOP(bnEnviroment* self, int count) {
        while (count-- > 0) {
                bnGenerateNOP(self);
        }
}

void bnDeleteEnviroment(bnEnviroment* self) {
        g_ptr_array_unref(self->codeArray);
        g_ptr_array_unref(self->labels);
        bnDeleteStack(self->labelFixStack, NULL);
        BN_FREE(self);
}