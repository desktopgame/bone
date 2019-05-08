#include "il_expr_array_lit.h"
#include "../runtime/enviroment.h"
#include "../runtime/interpreter.h"

bnILExprArrayLit* bnNewILExprArrayLit() {
        bnILExprArrayLit* ret = BN_MALLOC(sizeof(bnILExprArrayLit));
        ret->expressions = NULL;
        return ret;
}

void bnDumpILExprArrayLit(FILE* fp, struct bnStringPool* pool,
                          bnILExprArrayLit* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "array{...}\n");
        GList* iter = self->expressions;
        while (iter != NULL) {
                bnDumpILExpression(fp, pool, iter->data, depth + 1);
                iter = iter->next;
        }
}

void bnGenerateILExprArrayLit(bnInterpreter* bone, bnILExprArrayLit* self,
                              bnEnviroment* env) {
        GList* iter = self->expressions;
        while (iter != NULL) {
                bnGenerateILExpression(bone, iter->data, env);
                iter = iter->next;
        }
        g_ptr_array_add(env->codeArray, BN_OP_GEN_ARRAY);
        g_ptr_array_add(env->codeArray, g_list_length(self->expressions));
}

void bnDeleteILExprArrayLit(bnILExprArrayLit* self) {}