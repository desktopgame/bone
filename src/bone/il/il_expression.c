#include "il_expression.h"
#include "../bone.h"
#include "il_expr_all.h"
#include "il_lineno.h"

bnILExpression* bnNewILExpression(bnILExpressionType type) {
        bnILExpression* ret = BN_MALLOC(sizeof(bnILExpression));
        ret->type = type;
        ret->line = -1;
        return ret;
}

void bnDumpILExpression(FILE* fp, struct bnStringPool* pool,
                        bnILExpression* self, int depth) {
        bnDumpLineno(fp, self->line);
        switch (self->type) {
                case BN_IL_EXPR_NONE:
                        break;
                case BN_IL_EXPR_INT:
                        bnDumpILExprInt(fp, pool, self->u.vInt, depth);
                        break;
                case BN_IL_EXPR_DOUBLE:
                        bnDumpILExprDouble(fp, pool, self->u.vDouble, depth);
                        break;
                case BN_IL_EXPR_CHAR:
                        bnDumpILExprChar(fp, pool, self->u.vChar, depth);
                        break;
                case BN_IL_EXPR_STRING:
                        bnDumpILExprString(fp, pool, self->u.vString, depth);
                        break;
                case BN_IL_EXPR_BINOP:
                        bnDumpILExprBinOp(fp, pool, self->u.vBinOp, depth);
                        break;
                case BN_IL_EXPR_UOP:
                        bnDumpILExprUOp(fp, pool, self->u.vUOp, depth);
                        break;
                case BN_IL_EXPR_MEMBEROP:
                        bnDumpILExprMemberOp(fp, pool, self->u.vMemberOp,
                                             depth);
                        break;
                case BN_IL_EXPR_FUNCCALLOP:
                        bnDumpILExprFuncCallOp(fp, pool, self->u.vFuncCallOp,
                                               depth);
                        break;
                case BN_IL_EXPR_VARIABLE:
                        bnDumpILExprVariable(fp, pool, self->u.vVariable,
                                             depth);
                        break;
                case BN_IL_EXPR_LAMBDA:
                        bnDumpILExprLambda(fp, pool, self->u.vLambda, depth);
                        break;
                default:
                        assert(false);
                        break;
        }
}

void bnDeleteILExpression(bnILExpression* self) {
        switch (self->type) {
                case BN_IL_EXPR_NONE:
                        break;
                case BN_IL_EXPR_INT:
                        bnDeleteILExprInt(self->u.vInt);
                        break;
                case BN_IL_EXPR_DOUBLE:
                        bnDeleteILExprDouble(self->u.vDouble);
                        break;
                case BN_IL_EXPR_CHAR:
                        bnDeleteILExprChar(self->u.vChar);
                        break;
                case BN_IL_EXPR_STRING:
                        bnDeleteILExprString(self->u.vString);
                        break;
                case BN_IL_EXPR_BINOP:
                        bnDeleteILExprBinOp(self->u.vBinOp);
                        break;
                case BN_IL_EXPR_UOP:
                        bnDeleteILExprUOp(self->u.vUOp);
                        break;
                case BN_IL_EXPR_MEMBEROP:
                        bnDeleteILExprMemberOp(self->u.vMemberOp);
                        break;
                case BN_IL_EXPR_FUNCCALLOP:
                        bnDeleteILExprFuncCallOp(self->u.vFuncCallOp);
                        break;
                case BN_IL_EXPR_VARIABLE:
                        bnDeleteILExprVariable(self->u.vVariable);
                        break;
                case BN_IL_EXPR_LAMBDA:
                        bnDeleteILExprLambda(self->u.vLambda);
                        break;
                default:
                        assert(false);
                        break;
        }
        BN_FREE(self);
}