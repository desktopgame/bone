#include "il_expr_array_lit.h"
#include "../glib.h"
#include "../runtime/enviroment.h"
#include "../runtime/interpreter.h"

bnILExprArrayLit* bnNewILExprArrayLit() {
        bnILExprArrayLit* ret = BN_MALLOC(sizeof(bnILExprArrayLit));
        ret->expressions = g_ptr_array_new_full(2, bnDeleteILExpression);
        return ret;
}

void bnDumpILExprArrayLit(FILE* fp, struct bnStringPool* pool,
                          bnILExprArrayLit* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "array{...}\n");
        for (int i = 0; i < self->expressions->len; i++) {
                bnDumpILExpression(fp, pool,
                                   g_ptr_array_index(self->expressions, i),
                                   depth + 1);
        }
}

void bnGenerateILExprArrayLit(bnInterpreter* bone, bnILExprArrayLit* self,
                              bnEnviroment* env, bnCompileCache* ccache) {
        for (int i = 0; i < self->expressions->len; i++) {
                bnGenerateILExpression(
                    bone, g_ptr_array_index(self->expressions, i), env, ccache);
        }
        bnWriteCode(env, BN_OP_GEN_ARRAY);
        bnWriteCode(env, self->expressions->len);
}

void bnDeleteILExprArrayLit(bnILExprArrayLit* self) {
        g_ptr_array_free(self->expressions, TRUE);
        BN_FREE(self);
}