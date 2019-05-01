#include "il_stmt_if.h"
#include "il_expression.h"
#include "il_statement.h"

bnILStmtIf* bnNewILStmtIf(bnILExpression* cond) {
        bnILStmtIf* ret = BN_MALLOC(sizeof(bnILStmtIf));
        ret->cond = cond;
        ret->statements = NULL;
        return ret;
}

void bnDumpILStmtIf(FILE* fp, struct bnStringPool* pool, bnILStmtIf* self,
                    int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "If\n");
        bnDumpILExpression(fp, pool, self->cond, depth + 1);
        GList* iter = self->statements;
        while (iter != NULL) {
                bnDumpILStatement(fp, pool, iter->data, depth + 1);
                iter = iter->next;
        }
}

void bnDeleteILStmtIf(bnILStmtIf* self) {
        bnDeleteILExpression(self->cond);
        g_list_free_full(self->statements, bnDeleteILStatement);
        BN_FREE(self);
}

bnILStmtIfElse* bnNewILStmtIfElse(bnILStmtIf* trueCase) {
        bnILStmtIfElse* ret = BN_MALLOC(sizeof(bnILStmtIfElse));
        ret->trueCase = trueCase;
        ret->statements = NULL;
        return ret;
}

void bnDumpILStmtIfElse(FILE* fp, struct bnStringPool* pool,
                        bnILStmtIfElse* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "if else\n");
        bnDumpILStmtIf(fp, pool, self->trueCase, depth + 1);
        GList* iter = self->statements;
        while (iter != NULL) {
                bnDumpILStatement(fp, pool, iter->data, depth + 1);
                iter = iter->next;
        }
}

void bnDeleteILStmtIfElse(bnILStmtIfElse* self) {
        bnDeleteILStmtIf(self->trueCase);
        g_list_free_full(self->statements, bnDeleteILStatement);
        BN_FREE(self);
}