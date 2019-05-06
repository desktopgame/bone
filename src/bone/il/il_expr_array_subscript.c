#include "il_expr_array_subscript.h"

bnILExprArraySubscript* bnNewILExprArraySubscript(bnILExpression* arrayExpr,
                                                  bnILExpression* indexExpr) {
        bnILExprArraySubscript* ret = BN_MALLOC(sizeof(bnILExprArraySubscript));
        ret->arrayExpr = arrayExpr;
        ret->indexExpr = indexExpr;
        return ret;
}

void bnDumpILExprArraySubscript(FILE* fp, struct bnStringPool* pool,
                                bnILExprArraySubscript* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "[]\n");
        bnDumpILExpression(fp, pool, self->arrayExpr, depth + 1);
        bnDumpILExpression(fp, pool, self->indexExpr, depth + 1);
}

void bnDeleteILExprArraySubscript(bnILExprArraySubscript* self) {
        bnDeleteILExpression(self->arrayExpr);
        bnDeleteILExpression(self->indexExpr);
        BN_FREE(self);
}