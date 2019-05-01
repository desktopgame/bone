#include "il_expr_memberop.h"

bnILExprMemberOp* bnNewILExprMemberOp(GString* nameRef) {
        bnILExprMemberOp* ret = BN_MALLOC(sizeof(bnILExprMemberOp));
        ret->expr = NULL;
        ret->nameRef = nameRef;
        return ret;
}

void bnDumpILExprMemberOp(FILE* fp, bnILExprMemberOp* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "member %s\n", self->nameRef->str);
        bnDumpILExpression(fp, self->expr, depth + 1);
}

void bnDeleteILExprMemberOp(bnILExprMemberOp* self) {
        bnDeleteILExpression(self->expr);
        g_string_free(self->nameRef, TRUE);
        BN_FREE(self);
}