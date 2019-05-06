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
                                    bnEnviroment* env) {
        bnGenerateILExpression(bone, self->arrayExpr, env);
        g_ptr_array_add(env->codeArray, BN_OP_DUP);
        g_ptr_array_add(env->codeArray, BN_OP_GET);
        g_ptr_array_add(env->codeArray, bnIntern(bone->pool, BN_KWD_ARRAY_GET));
        bnGenerateILExpression(bone, self->indexExpr, env);
        g_ptr_array_add(env->codeArray, BN_OP_SWAP);
        g_ptr_array_add(env->codeArray, BN_OP_FUNCCALL);
        g_ptr_array_add(env->codeArray, 2);
}

void bnDeleteILExprArraySubscript(bnILExprArraySubscript* self) {
        bnDeleteILExpression(self->arrayExpr);
        bnDeleteILExpression(self->indexExpr);
        BN_FREE(self);
}