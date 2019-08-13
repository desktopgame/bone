#include "il_expr_array_subscript.h"
#include "../runtime/enviroment.h"
#include "../runtime/interpreter.h"
#include "../runtime/keyword.h"
#include "il_expr_all.h"

bnILExprArraySubscript* bnNewILExprArraySubscript(bnILExpression* arrayExpr,
                                                  bnILExpression* indexExpr) {
        bnILExprArraySubscript* ret = BN_MALLOC(sizeof(bnILExprArraySubscript));
        ret->arrayExpr = arrayExpr;
        ret->indexExpr = indexExpr;
        return ret;
}

void bnDumpILExprArraySubscript(FILE* fp, struct bnStringPool* pool,
                                bnILExprArraySubscript* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "[]\n");
        bnDumpILExpression(fp, pool, self->arrayExpr, depth + 1);
        bnDumpILExpression(fp, pool, self->indexExpr, depth + 1);
}

void bnGenerateILExprArraySubscript(bnInterpreter* bone,
                                    bnILExprArraySubscript* self,
                                    bnEnviroment* env, bnCompileCache* ccache) {
        bnGenerateILExpression(bone, self->arrayExpr, env, ccache);
        bnWriteCode(env, BN_OP_CLEANUP_INJBUF);
        bnWriteCode(env, BN_OP_DUP);
        bnWriteCode(env, BN_OP_GET);
        bnWriteCode(env, bnIntern(bone->pool, BN_KWD_ARRAY_GET));
        bnGenerateILExpression(bone, self->indexExpr, env, ccache);
        bnWriteCode(env, BN_OP_CLEANUP_INJBUF);
        bnWriteCode(env, BN_OP_SWAP);
        bnWriteCode(env, BN_OP_FUNCCALL);
        bnWriteCode(env, 2);
}

void bnDeleteILExprArraySubscript(bnILExprArraySubscript* self) {
        bnDeleteILExpression(self->arrayExpr);
        bnDeleteILExpression(self->indexExpr);
        BN_FREE(self);
}