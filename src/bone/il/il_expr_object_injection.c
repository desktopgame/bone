#include "il_expr_object_injection.h"
#include "../runtime/enviroment.h"

bnILExprObjectInjection* bnNewILExprObjectInjection(bnILExpression* leftExpr,
                                                    bnILExpression* rightExpr) {
        bnILExprObjectInjection* ret =
            BN_MALLOC(sizeof(bnILExprObjectInjection));
        ret->leftExpr = leftExpr;
        ret->rightExpr = rightExpr;
        return ret;
}

void bnDumpILExprObjectInjection(FILE* fp, struct bnStringPool* pool,
                                 bnILExprObjectInjection* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "object inject\n");
        bnDumpILExpression(fp, pool, self->leftExpr, depth + 1);
        bnDumpILExpression(fp, pool, self->rightExpr, depth + 1);
}

void bnGenerateILExprObjectInjection(struct bnInterpreter* bone,
                                     bnILExprObjectInjection* self,
                                     struct bnEnviroment* env,
                                     bnCompileCache* ccache) {
        bnGenerateILExpression(bone, self->leftExpr, env, ccache);
        bnGenerateILExpression(bone, self->rightExpr, env, ccache);
        g_ptr_array_add(env->codeArray, BN_OP_OBJECT_INJECTION);
}

void bnDeleteILExprObjectInjection(bnILExprObjectInjection* self) {
        bnDeleteILExpression(self->leftExpr);
        bnDeleteILExpression(self->rightExpr);
        BN_FREE(self);
}