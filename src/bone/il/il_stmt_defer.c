#include "il_stmt_defer.h"
#include "../runtime/enviroment.h"

bnILStmtDefer* bnNewILStmtDefer(bnILStatement* stmt) {
        bnILStmtDefer* ret = BN_MALLOC(sizeof(bnILStmtDefer));
        ret->stmt = stmt;
        return ret;
}

void bnDumpILStmtDefer(FILE* fp, struct bnStringPool* pool, bnILStmtDefer* self,
                       int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "defer\n");
        bnDumpILStatement(fp, pool, self->stmt, depth + 1);
}

void bnGenerateILStmtDefer(struct bnInterpreter* bone, bnILStmtDefer* self,
                           bnEnviroment* env, bnCompileCache* ccache) {
        bnLabel* lab = bnAutoNewLabel(env, 0);
        bnWriteCode(env, BN_OP_DEFER_PUSH);
        bnWriteLabel(env, lab);
        bnWriteCode(env, BN_OP_DEFER_BEGIN);
        lab->pos = bnGenerateNOP(env) - bnGetLambdaOffset(env);
        bnGenerateNOP(env);
        bnGenerateILStatement(bone, self->stmt, env, ccache);
        bnWriteCode(env, BN_OP_DEFER_NEXT);
        bnWriteCode(env, BN_OP_DEFER_END);
}

void bnDeleteILStmtDefer(bnILStmtDefer* self) {
        bnDeleteILStatement(self->stmt);
        BN_FREE(self);
}