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
        bnWriteCode(env, BN_OP_GEN_DOUBLE);
        bnWriteCode(env, len);
}

void bnDeleteILExprDouble(bnILExprDouble* self) { BN_FREE(self); }