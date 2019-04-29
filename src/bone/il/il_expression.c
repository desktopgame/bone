#include "il_expression.h"

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