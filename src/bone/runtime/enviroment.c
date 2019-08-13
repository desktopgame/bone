#include "enviroment.h"
#include "lambda.h"
#include "line_range.h"

static void delete_label(gpointer data);

bnEnviroment* bnNewEnviroment(bnStringView filename) {
        bnEnviroment* ret = BN_MALLOC(sizeof(bnEnviroment));
        ret->codeArray = g_ptr_array_new();
        ret->labels = g_ptr_array_new();
        ret->ranges = g_ptr_array_new();
        ret->labelFixStack = bnNewStack();
        ret->filename = filename;
        ret->lineOffset = 0;
        ret->doubleConstants =
            g_array_sized_new(FALSE, TRUE, sizeof(double), 10);
        g_ptr_array_set_free_func(ret->codeArray, NULL);
        g_ptr_array_set_free_func(ret->labels, delete_label);
        g_ptr_array_set_free_func(ret->ranges, bnDeleteLineRange);
        return ret;
}

void bnWriteCode(bnEnviroment* self, int code) {
        g_ptr_array_add(self->codeArray, (gpointer)code);
}

int bnReadCode(bnEnviroment* self, int pc) {
        return (int)g_ptr_array_index(self->codeArray, pc);
}

void bnWriteLabel(bnEnviroment* self, bnLabel* label) {
        g_ptr_array_add(self->codeArray, (gpointer)label);
}

void bnAddLineRange(bnEnviroment* self, int lineno) {
        if (self->ranges->len == 0) {
                bnLineRange* lr = bnNewLineRange();
                lr->start = 0;
                lr->end = 0;
                lr->line = lineno;
                g_ptr_array_add(self->ranges, lr);
                return;
        }
        bnLineRange* lr =
            g_ptr_array_index(self->ranges, self->ranges->len - 1);
        if (lr->line == lineno) {
                lr->end = self->codeArray->len;
        } else {
                bnLineRange* lr = bnNewLineRange();
                lr->start = self->codeArray->len;
                lr->end = self->codeArray->len;
                lr->line = lineno;
                g_ptr_array_add(self->ranges, lr);
        }
}

int bnFindLineRange(bnEnviroment* self, int pc) {
        for (int i = 0; i < self->ranges->len; i++) {
                bnLineRange* lr = g_ptr_array_index(self->ranges, i);
                if (pc >= lr->start && pc <= lr->end) {
                        return lr->line;
                }
        }
        return 0;
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
        g_ptr_array_free(self->ranges, TRUE);
        g_array_free(self->doubleConstants, FALSE);
        bnDeleteStack(self->labelFixStack, NULL);
        BN_FREE(self);
}

static void delete_label(gpointer data) {
        bnLabel* l = data;
        bnDeleteLabel(l);
}