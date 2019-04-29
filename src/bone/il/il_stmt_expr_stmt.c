#include "il_stmt_expr_stmt.h"
#include "il_expr_all.h"

bnILStmtExpr* bnNewILStmtExpr(bnILExpression* expr) {
        bnILStmtExpr* ret = BN_MALLOC(sizeof(bnILStmtExpr));
        ret->expr = expr;
        return ret;
}

void bnDumpILStmtExpr(FILE* fp, bnILStmtExpr* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "ExprStmt\n");
        bnDumpILExpression(fp, self->expr, depth + 1);
}

void bnDeleteILStmtExpr(bnILStmtExpr* self) {}