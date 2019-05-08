#include "il_expr_array_lit.h"

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

void bnGenerateILExprArrayLit(struct bnInterpreter* bone,
                              bnILExprArrayLit* self,
                              struct bnEnviroment* env) {}

void bnDeleteILExprArrayLit(bnILExprArrayLit* self) {}