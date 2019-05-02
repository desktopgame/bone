#include "il_expr_int.h"
#include "../runtime/enviroment.h"

bnILExprInt* bnNewILExprInt(int value) {
        bnILExprInt* ret = BN_MALLOC(sizeof(bnILExprInt));
        ret->value = value;
        return ret;
}

void bnDumpILExprInt(FILE* fp, struct bnStringPool* pool, bnILExprInt* self,
                     int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "%d\n", self->value);
}

void bnGenerateILExprInt(struct bnInterpreter* bone, bnILExprInt* self,
                         bnEnviroment* env) {
        env->binary = g_list_append(env->binary, BN_OP_GEN_INT);
        env->binary = g_list_append(env->binary, GINT_TO_POINTER(self->value));
}

void bnDeleteILExprInt(bnILExprInt* self) { BN_FREE(self); }