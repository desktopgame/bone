#include "il_stmt_expr_stmt.h"
#include "../runtime/enviroment.h"
#include "il_expr_all.h"

bnILStmtExpr* bnNewILStmtExpr(bnILExpression* expr) {
        bnILStmtExpr* ret = BN_MALLOC(sizeof(bnILStmtExpr));
        ret->expr = expr;
        return ret;
}

void bnDumpILStmtExpr(FILE* fp, struct bnStringPool* pool, bnILStmtExpr* self,
                      int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "ExprStmt\n");
        bnDumpILExpression(fp, pool, self->expr, depth + 1);
}

void bnGenerateILStmtExpr(struct bnInterpreter* bone, bnILStmtExpr* self,
                          bnEnviroment* env, bnCompileCache* ccache) {
        bnGenerateILExpression(bone, self->expr, env, ccache);
}

void bnDeleteILStmtExpr(bnILStmtExpr* self) {
        bnDeleteILExpression(self->expr);
        BN_FREE(self);
}