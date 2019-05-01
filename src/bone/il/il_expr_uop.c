#include "il_expr_uop.h"

bnILExprUOp* bnNewILExprUOp(bnILUOpType type) {
        bnILExprUOp* ret = BN_MALLOC(sizeof(bnILUOpType));
        ret->a = NULL;
        ret->type = type;
        return ret;
}

void bnDumpILExprUOp(FILE* fp, struct bnStringPool* pool, bnILExprUOp* self,
                     int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "uop\n");
        bnDumpILExpression(fp, pool, self->a, depth + 1);
}

void bnDeleteILExprUOp(bnILExprUOp* self) {
        bnDeleteILExpression(self->a);
        BN_FREE(self);
}