#include "il_expr_variable.h"
#include "il_expression.h"

bnILExprVariable* bnNewILExprVariable(GString* nameRef) {
        bnILExprVariable* ret = BN_MALLOC(sizeof(bnILExprVariable));
        ret->nameRef = nameRef;
        return ret;
}

void bnDumpILExprVariable(FILE* fp, bnILExprVariable* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "%s\n", self->nameRef->str);
}

void bnDeleteILExprVariable(bnILExprVariable* self) {
        g_string_free(self->nameRef, TRUE);
        BN_FREE(self);
}