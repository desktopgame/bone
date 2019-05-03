#include "il_expr_object_injection.h"

bnILExprObjectInjection* bnNewILExprObjectInjection(bnILExpression* leftExpr,
                                                    bnILExpression* rightExpr) {
        bnILExprObjectInjection* ret =
            BN_MALLOC(sizeof(bnILExprObjectInjection));
        ret->leftExpr = leftExpr;
        ret->rightExpr = rightExpr;
        return ret;
}

void bnDumpILExprObjectInjection(FILE* fp, struct bnStringPool* pool,
                                 bnILExprObjectInjection* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "object inject\n");
        bnDumpILExpression(fp, pool, self->leftExpr, depth + 1);
        bnDumpILExpression(fp, pool, self->rightExpr, depth + 1);
}

void bnGenerateILExprObjectInjection(struct bnInterpreter* bone,
                                     bnILExprObjectInjection* self,
                                     struct bnEnviroment* env) {}

void bnDeleteILExprObjectInjection(bnILExprObjectInjection* self) {
        bnDeleteILExpression(self->leftExpr);
        bnDeleteILExpression(self->rightExpr);
}