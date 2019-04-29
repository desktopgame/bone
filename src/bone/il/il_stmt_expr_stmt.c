#include "il_stmt_expr_stmt.h"

bnILStmtExpr* bnNewILStmtExpr(bnILExpression* expr) {
        bnILStmtExpr* ret = BN_MALLOC(sizeof(bnILStmtExpr));
        ret->expr = expr;
        return ret;
}

void bnDeleteILStmtExpr(bnILStmtExpr* self) {
	
}