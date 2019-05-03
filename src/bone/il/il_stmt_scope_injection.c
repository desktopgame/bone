#include "il_stmt_scope_injection.h"

bnILStmtScopeInjection* bnNewILStmtScopeInjection(bnILExpression* expr) {
        bnILStmtScopeInjection* ret = BN_MALLOC(sizeof(bnILStmtScopeInjection));
        ret->expr = expr;
        return ret;
}

void bnDumpILStmtScopeInjection(FILE* fp, struct bnStringPool* pool,
                                bnILStmtScopeInjection* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "scope injection\n");
        bnDumpILExpression(fp, pool, self->expr, depth + 1);
}

void bnGenerateILStmtScopeInjection(struct bnInterpreter* bone,
                                    bnILStmtScopeInjection* self,
                                    struct bnEnviroment* env) {}

void bnDeleteILStmtScopeInjection(bnILStmtScopeInjection* self) {
        bnDeleteILExpression(self->expr);
        BN_FREE(self);
}