#include "il_expr_memberop.h"

bnILExprMemberOp* bnNewILExprMemberOp(bnStringView name) {
        bnILExprMemberOp* ret = BN_MALLOC(sizeof(bnILExprMemberOp));
        ret->expr = NULL;
        ret->name = name;
        return ret;
}

void bnDumpILExprMemberOp(FILE* fp, struct bnStringPool* pool,
                          bnILExprMemberOp* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "member %s\n", bnView2Str(pool, self->name));
        bnDumpILExpression(fp, pool, self->expr, depth + 1);
}

void bnDeleteILExprMemberOp(bnILExprMemberOp* self) {
        bnDeleteILExpression(self->expr);
        BN_FREE(self);
}