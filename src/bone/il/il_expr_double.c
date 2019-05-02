#include "il_expr_double.h"
#include "../runtime/enviroment.h"

bnILExprDouble* bnNewILExprDouble(double value) {
        bnILExprDouble* ret = BN_MALLOC(sizeof(bnILExprDouble));
        ret->value = value;
        return ret;
}

void bnDumpILExprDouble(FILE* fp, struct bnStringPool* pool,
                        bnILExprDouble* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "%lf\n", self->value);
}

void bnGenerateILExprDouble(bnILExprDouble* self, bnEnviroment* env) {}

void bnDeleteILExprDouble(bnILExprDouble* self) { BN_FREE(self); }