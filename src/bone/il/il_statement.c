#include "il_statement.h"
#include "../bone.h"
#include "il_lineno.h"
#include "il_stmt_all.h"

bnILStatement* bnNewILStatement(bnILStatementType type) {
        bnILStatement* ret = BN_MALLOC(sizeof(bnILStatement));
        ret->type = type;
        ret->line = -1;
        return ret;
}

void bnDumpILStatement(FILE* fp, bnILStatement* self, int depth) {
        bnDumpLineno(fp, self->line);
        switch (self->type) {
                case BN_IL_STMT_NONE:
                        break;
                case BN_IL_STMT_EXPRSTMT:
                        bnDumpILStmtExpr(fp, self->u.vExprStmt, depth);
                        break;
                case BN_IL_STMT_IF:
                        bnDumpILStmtIf(fp, self->u.vIf, depth);
                        break;
                case BN_IL_STMT_IF_ELSE:
                        bnDumpILStmtIfElse(fp, self->u.vIfElse, depth);
                        break;
                case BN_IL_STMT_WHILE:
                        bnDumpILStmtWhile(fp, self->u.vWhile, depth);
                        break;
                default:
                        assert(false);
                        break;
        }
}

void bnDeleteILStatement(bnILStatement* self) {
        switch (self->type) {
                case BN_IL_STMT_NONE:
                        break;
                case BN_IL_STMT_EXPRSTMT:
                        bnDeleteILStmtExpr(self->u.vExprStmt);
                        break;
                case BN_IL_STMT_IF:
                        bnDeleteILStmtIf(self->u.vIf);
                        break;
                case BN_IL_STMT_IF_ELSE:
                        bnDeleteILStmtIfElse(self->u.vIfElse);
                        break;
                case BN_IL_STMT_WHILE:
                        bnDeleteILStmtWhile(self->u.vWhile);
                        break;
                default:
                        assert(false);
                        break;
        }
        BN_FREE(self);
}