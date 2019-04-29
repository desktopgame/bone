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
                default:
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
                default:
                        break;
        }
}