#include "il_expr_char.h"
#include "../runtime/enviroment.h"

bnILExprChar* bnNewILExprChar(char value) {
        bnILExprChar* ret = BN_MALLOC(sizeof(bnILExprChar));
        ret->value = value;
        return ret;
}

void bnDumpILExprChar(FILE* fp, struct bnStringPool* pool, bnILExprChar* self,
                      int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "char %c\n", self->value);
}

void bnGenerateILExprChar(struct bnInterpreter* bone, bnILExprChar* self,
                          bnEnviroment* env, bnCompileCache* ccache) {
        bnWriteCode(env, BN_OP_GEN_CHAR);
        bnWriteCode(env, self->value);
}

void bnDeleteILExprChar(bnILExprChar* self) { BN_FREE(self); }