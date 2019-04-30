#include "il_stmt_while.h"
#include "il_expression.h"
#include "il_statement.h"

bnILStmtWhile* bnNewILStmtWhile(bnILExpression* cond) {
        bnILStmtWhile* ret = BN_MALLOC(sizeof(bnILStmtWhile));
        ret->cond = cond;
        ret->statements = NULL;
        return ret;
}

void bnDumpILStmtWhile(FILE* fp, bnILStmtWhile* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "while\n");
        bnDumpILExpression(fp, self->cond, depth + 1);
        GList* iter = self->statements;
        while (iter != NULL) {
                bnDumpILStatement(fp, iter->data, depth + 1);
                iter = iter->next;
        }
}

void bnDeleteILStmtWhile(bnILStmtWhile* self) {}