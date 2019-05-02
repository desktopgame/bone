#include "il_expr_string.h"
#include "../runtime/enviroment.h"

bnILExprString* bnNewILExprString(bnStringView value) {
        bnILExprString* ret = BN_MALLOC(sizeof(bnILExprString));
        ret->value = value;
        return ret;
}

void bnDumpILExprString(FILE* fp, struct bnStringPool* pool,
                        bnILExprString* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "string \"%s\"\n", bnView2Str(pool, self->value));
}

void bnGenerateILExprString(struct bnInterpreter* bone, bnILExprString* self,
                            bnEnviroment* env) {}

void bnDeleteILExprString(bnILExprString* self) { BN_FREE(self); }