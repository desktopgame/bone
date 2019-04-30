#include "il_expression.h"
#include "../bone.h"
#include "il_expr_all.h"

bnILExpression* bnNewILExpression(bnILExpressionType type) {
        bnILExpression* ret = BN_MALLOC(sizeof(bnILExpression));
        ret->type = type;
        return ret;
}

void bnDumpILExpression(FILE* fp, bnILExpression* self, int depth) {
        switch (self->type) {
                case BN_IL_EXPR_NONE:
                        break;
                case BN_IL_EXPR_INT:
                        bnDumpILExprInt(fp, self->u.vInt, depth);
                        break;
                case BN_IL_EXPR_DOUBLE:
                        bnDumpILExprDouble(fp, self->u.vDouble, depth);
                        break;
                case BN_IL_EXPR_CHAR:
                        bnDumpILExprChar(fp, self->u.vChar, depth);
                        break;
                case BN_IL_EXPR_STRING:
                        bnDumpILExprString(fp, self->u.vString, depth);
                        break;
                case BN_IL_EXPR_BINOP:
                        bnDumpILExprBinOp(fp, self->u.vBinOp, depth);
                        break;
                case BN_IL_EXPR_UOP:
                        bnDumpILExprUOp(fp, self->u.vUOp, depth);
                        break;
                case BN_IL_EXPR_MEMBEROP:
                        bnDumpILExprMemberOp(fp, self->u.vMemberOp, depth);
                        break;
                case BN_IL_EXPR_FUNCCALLOP:
                        bnDumpILExprFuncCallOp(fp, self->u.vFuncCallOp, depth);
                        break;
                case BN_IL_EXPR_VARIABLE:
                        bnDumpILExprVariable(fp, self->u.vVariable, depth);
                        break;
                case BN_IL_EXPR_LAMBDA:
                        bnDumpILExprLambda(fp, self->u.vLambda, depth);
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
                        break;
                case BN_IL_EXPR_DOUBLE:
                        break;
                case BN_IL_EXPR_CHAR:
                        break;
                case BN_IL_EXPR_STRING:
                        break;
                case BN_IL_EXPR_FUNCCALLOP:
                        break;
                case BN_IL_EXPR_VARIABLE:
                        break;
                default:
                        break;
        }
}