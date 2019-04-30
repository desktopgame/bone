#include "il_expr_binop.h"

bnILExprBinOp* bnNewILExprBinOp(bnILBinOpType type) {
        bnILExprBinOp* ret = BN_MALLOC(sizeof(bnILExprBinOp));
        ret->left = NULL;
        ret->right = NULL;
        ret->type = type;
        return ret;
}

void bnDumpILExprBinOp(FILE* fp, bnILExprBinOp* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "binop\n");
        bnDumpILExpression(fp, self->left, depth + 1);
        bnDumpILExpression(fp, self->right, depth + 1);
}

void bnDeleteILExprBinOp(bnILExprBinOp* self) {
        bnDeleteILExpression(self->left);
        bnDeleteILExpression(self->right);
        BN_FREE(self);
}