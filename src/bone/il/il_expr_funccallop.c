#include "il_expr_funccallop.h"
#include "../runtime/enviroment.h"
#include "../runtime/keyword.h"

bnILExprFuncCallOp* bnNewILExprFuncCallOp(bnILExpression* expr) {
        bnILExprFuncCallOp* ret = BN_MALLOC(sizeof(bnILExprFuncCallOp));
        ret->expr = expr;
        ret->arguments = NULL;
        return ret;
}

void bnDumpILExprFuncCallOp(FILE* fp, struct bnStringPool* pool,
                            bnILExprFuncCallOp* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "()\n");
        bnDumpILExpression(fp, pool, self->expr, depth + 1);
        GList* iter = self->arguments;
        while (iter != NULL) {
                bnDumpILExpression(fp, pool, iter->data, depth + 1);
                iter = iter->next;
        }
}

void bnGenerateILExprFuncCallOp(struct bnInterpreter* bone,
                                bnILExprFuncCallOp* self, bnEnviroment* env) {}

void bnDeleteILExprFuncCallOp(bnILExprFuncCallOp* self) {
        bnDeleteILExpression(self->expr);
        g_list_free_full(self->arguments, bnDeleteILExpression);
        BN_FREE(self);
}