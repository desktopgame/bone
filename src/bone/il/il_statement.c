#include "il_statement.h"

void bnDeleteILStatement(bnILStatement* self) {
        switch (self->type) {
                case BN_IL_STMT_NONE:
                        break;
                case BN_IL_STMT_EXPRSTMT:
                        break;
                case BN_IL_STMT_IF:
                        break;
                case BN_IL_STMT_WHILE:
                        break;
                default:
                        break;
        }
}