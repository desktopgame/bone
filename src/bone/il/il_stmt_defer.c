#include "il_stmt_defer.h"

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
                           struct bnEnviroment* env) {}

void bnDeleteILStmtDefer(bnILStmtDefer* self) {
        bnDeleteILStatement(self->stmt);
        BN_FREE(self);
}