#include "il_stmt_scope_injection.h"
#include "../runtime/enviroment.h"

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
                                    struct bnEnviroment* env) {
        bnGenerateILExpression(bone, self->expr, env);
        g_ptr_array_add(env->codeArray, BN_OP_SCOPE_INJECTION);
}

void bnDeleteILStmtScopeInjection(bnILStmtScopeInjection* self) {
        bnDeleteILExpression(self->expr);
        BN_FREE(self);
}