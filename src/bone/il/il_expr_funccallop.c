﻿#include "il_expr_funccallop.h"
#include "../runtime/enviroment.h"
#include "../runtime/keyword.h"
#include "il_expr_all.h"

static void delete_il_expression(gpointer data);

bnILExprFuncCallOp* bnNewILExprFuncCallOp(bnILExpression* expr) {
        bnILExprFuncCallOp* ret = BN_MALLOC(sizeof(bnILExprFuncCallOp));
        ret->expr = expr;
        ret->arguments = g_ptr_array_new_full(2, delete_il_expression);
        return ret;
}

void bnDumpILExprFuncCallOp(FILE* fp, struct bnStringPool* pool,
                            bnILExprFuncCallOp* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "()\n");
        bnDumpILExpression(fp, pool, self->expr, depth + 1);
        for (int i = 0; i < self->arguments->len; i++) {
                bnDumpILExpression(
                    fp, pool, g_ptr_array_index(self->arguments, i), depth + 1);
        }
}

void bnGenerateILExprFuncCallOp(struct bnInterpreter* bone,
                                bnILExprFuncCallOp* self, bnEnviroment* env,
                                bnCompileCache* ccache) {
        int count = 0;
        for (int i = 0; i < self->arguments->len; i++) {
                count++;
                bnGenerateILExpression(
                    bone, g_ptr_array_index(self->arguments, i), env, ccache);
                bnWriteCode(env, BN_OP_CLEANUP_INJBUF);
        }
        bnGenerateILExpression(bone, self->expr, env, ccache);
        //インスタンスベースのラムダであるなら、
        //レシーバーを第一引数として認識させる。
        if (self->expr->type == BN_IL_EXPR_MEMBEROP) {
                bnWriteCode(env, BN_OP_GET_REGISTER_0);
                bnWriteCode(env, BN_OP_PUSH_SELF);
        }
        bnWriteCode(env, BN_OP_FUNCCALL);
        bnWriteCode(env, count);
        if (self->expr->type == BN_IL_EXPR_MEMBEROP) {
                bnWriteCode(env, BN_OP_POP_SELF);
        }
}

void bnDeleteILExprFuncCallOp(bnILExprFuncCallOp* self) {
        bnDeleteILExpression(self->expr);
        g_ptr_array_free(self->arguments, TRUE);
        BN_FREE(self);
}

static void delete_il_expression(gpointer data) {
        bnDeleteILExpression((bnILExpression*)data);
}