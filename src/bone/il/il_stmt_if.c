#include "il_stmt_if.h"

bnILStmtIf* bnNewILStmtIf(bnILExpression* cond) {
        bnILStmtIf* ret = BN_MALLOC(sizeof(bnILStmtIf));
        ret->cond = cond;
        ret->statements = NULL;
        return ret;
}

void bnDeleteILStmtIf(bnILStmtIf* self) {}

bnILStmtIfElse* bnNewILStmtIfElse(bnILStmtIf* trueCase) {
        bnILStmtIfElse* ret = BN_MALLOC(sizeof(bnILStmtIfElse));
        ret->trueCase = trueCase;
        ret->statements = NULL;
        return ret;
}

void bnDeleteILStmtIfElse(bnILStmtIfElse* self) {}