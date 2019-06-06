#include "il_expr_double.h"
#include "../runtime/enviroment.h"

bnILExprDouble* bnNewILExprDouble(double value) {
        bnILExprDouble* ret = BN_MALLOC(sizeof(bnILExprDouble));
        ret->value = value;
        return ret;
}

void bnDumpILExprDouble(FILE* fp, struct bnStringPool* pool,
                        bnILExprDouble* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "%lf\n", self->value);
}

void bnGenerateILExprDouble(struct bnInterpreter* bone, bnILExprDouble* self,
                            bnEnviroment* env, bnCompileCache* ccache) {
        int len = env->doubleConstants->len;
        g_array_append_val(env->doubleConstants, self->value);
        g_ptr_array_add(env->codeArray, BN_OP_GEN_DOUBLE);
        g_ptr_array_add(env->codeArray, len);
}

void bnDeleteILExprDouble(bnILExprDouble* self) { BN_FREE(self); }