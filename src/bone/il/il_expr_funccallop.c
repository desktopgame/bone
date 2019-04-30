#include "il_expr_funccallop.h"

bnILExprFuncCallOp* bnNewILExprFuncCallOp(bnILExpression* expr) {
        bnILExprFuncCallOp* ret = BN_MALLOC(sizeof(bnILExprFuncCallOp));
        ret->expr = expr;
        ret->arguments = NULL;
        return ret;
}

void bnDumpILExprFuncCallOp(FILE* fp, bnILExprFuncCallOp* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "()\n");
        bnDumpILExpression(fp, self->expr, depth + 1);
        GList* iter = self->arguments;
        while (iter != NULL) {
                bnDumpILExpression(fp, iter->data, depth + 1);
                iter = iter->next;
        }
}

void bnDeleteILExprFuncCallOp(bnILExprFuncCallOp* self) {
        bnDeleteILExpression(self->expr);
}