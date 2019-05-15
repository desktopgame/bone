#include "il_expr_variable.h"
#include "../runtime/enviroment.h"
#include "il_expression.h"

bnILExprVariable* bnNewILExprVariable(bnStringView name) {
        bnILExprVariable* ret = BN_MALLOC(sizeof(bnILExprVariable));
        ret->name = name;
        return ret;
}

void bnDumpILExprVariable(FILE* fp, struct bnStringPool* pool,
                          bnILExprVariable* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "%s\n", bnView2Str(pool, self->name));
}

void bnGenerateILExprVariable(struct bnInterpreter* bone,
                              bnILExprVariable* self, bnEnviroment* env,
                              bnCompileCache* ccache) {
        g_ptr_array_add(env->codeArray, BN_OP_LOAD);
        g_ptr_array_add(env->codeArray, self->name);
}

void bnDeleteILExprVariable(bnILExprVariable* self) { BN_FREE(self); }