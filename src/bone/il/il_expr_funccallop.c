#include "il_expr_funccallop.h"
#include "../runtime/enviroment.h"
#include "../runtime/keyword.h"
#include "il_expr_all.h"

bnILExprFuncCallOp* bnNewILExprFuncCallOp(bnILExpression* expr) {
        bnILExprFuncCallOp* ret = BN_MALLOC(sizeof(bnILExprFuncCallOp));
        ret->expr = expr;
        ret->arguments = NULL;
        return ret;
}

void bnDumpILExprFuncCallOp(FILE* fp, struct bnStringPool* pool,
                            bnILExprFuncCallOp* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "()\n");
        bnDumpILExpression(fp, pool, self->expr, depth + 1);
        GList* iter = self->arguments;
        while (iter != NULL) {
                bnDumpILExpression(fp, pool, iter->data, depth + 1);
                iter = iter->next;
        }
}

void bnGenerateILExprFuncCallOp(struct bnInterpreter* bone,
                                bnILExprFuncCallOp* self, bnEnviroment* env) {
        int count = 0;
        GList* iter = self->arguments;
        while (iter != NULL) {
                count++;
                bnGenerateILExpression(bone, iter->data, env);
                iter = iter->next;
        }
        bnGenerateILExpression(bone, self->expr, env);
        // instance based closure?
        if (self->expr->type == BN_IL_EXPR_MEMBEROP) {
                env->binary = g_list_append(env->binary, BN_OP_GET_REGISTER_0);
                env->binary = g_list_append(env->binary, BN_OP_PUSH_SELF);
        }
        env->binary = g_list_append(env->binary, BN_OP_FUNCCALL);
        env->binary = g_list_append(env->binary, count);
        if (self->expr->type == BN_IL_EXPR_MEMBEROP) {
                env->binary = g_list_append(env->binary, BN_OP_POP_SELF);
        }
}

void bnDeleteILExprFuncCallOp(bnILExprFuncCallOp* self) {
        bnDeleteILExpression(self->expr);
        g_list_free_full(self->arguments, bnDeleteILExpression);
        BN_FREE(self);
}