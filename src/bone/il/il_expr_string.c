#include "il_expr_string.h"

bnILExprString* bnNewILExprString(GString* value) {
        bnILExprString* ret = BN_MALLOC(sizeof(bnILExprString));
        ret->value = value;
        return ret;
}

void bnDumpILExprString(FILE* fp, bnILExprString* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "string \"%s\"\n", self->value->str);
}

void bnDeleteILExprString(bnILExprString* self) { BN_FREE(self); }