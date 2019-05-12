#include "enviroment.h"
#include "lambda.h"

static void delete_label(gpointer data);

bnEnviroment* bnNewEnviroment() {
        bnEnviroment* ret = BN_MALLOC(sizeof(bnEnviroment));
        ret->codeArray = g_ptr_array_new();
        ret->labels = g_ptr_array_new();
        ret->labelFixStack = bnNewStack();
        g_ptr_array_set_free_func(ret->codeArray, NULL);
        g_ptr_array_set_free_func(ret->labels, delete_label);
        return ret;
}

bnLabel* bnAutoNewLabel(bnEnviroment* self, int pos) {
        bnLabel* lb = bnNewLabel(pos);
        g_ptr_array_add(self->labels, lb);
        return lb;
}

bnLabel* bnGenerateLabel(bnEnviroment* self, int pos) {
        if (self->labelFixStack->head != NULL) {
                pos -= bnGetLambdaOffset(self);
        }
        bnLabel* lb = bnNewLabel(pos);
        g_ptr_array_add(self->codeArray, lb);
        g_ptr_array_add(self->labels, lb);
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

int bnGetLambdaOffset(bnEnviroment* self) {
        if (bnGetStackSize(self->labelFixStack) == 0) {
                return 0;
        }
        return ((int)bnPeekStack(self->labelFixStack));
}

int bnGetPrependPos(bnEnviroment* self) {
        if (bnGetStackSize(self->labelFixStack) == 0) {
                return 0;
        }
        return bnPeekStack(self->labelFixStack);
}

void bnGenerateFillNOP(bnEnviroment* self, int count) {
        while (count-- > 0) {
                bnGenerateNOP(self);
        }
}

void bnDeleteEnviroment(bnEnviroment* self) {
        g_ptr_array_free(self->codeArray, TRUE);
        g_ptr_array_free(self->labels, TRUE);
        bnDeleteStack(self->labelFixStack, NULL);
        BN_FREE(self);
}

static void delete_label(gpointer data) {
        bnLabel* l = data;
        BN_FREE(data);
}