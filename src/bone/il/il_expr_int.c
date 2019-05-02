#include "il_expr_int.h"
#include "../runtime/enviroment.h"

bnILExprInt* bnNewILExprInt(int value) {
        bnILExprInt* ret = BN_MALLOC(sizeof(bnILExprInt));
        ret->value = value;
        return ret;
}

void bnDumpILExprInt(FILE* fp, struct bnStringPool* pool, bnILExprInt* self,
                     int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "%d\n", self->value);
}

void bnGenerateILExprInt(bnILExprInt* self, bnEnviroment* env) {}

void bnDeleteILExprInt(bnILExprInt* self) { BN_FREE(self); }