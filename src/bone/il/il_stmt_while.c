#include "il_stmt_while.h"
#include "../runtime/enviroment.h"
#include "il_expression.h"
#include "il_statement.h"

bnILStmtWhile* bnNewILStmtWhile(bnILExpression* cond) {
        bnILStmtWhile* ret = BN_MALLOC(sizeof(bnILStmtWhile));
        ret->cond = cond;
        ret->statements = g_ptr_array_new_full(2, bnDeleteILStatement);
        return ret;
}

void bnDumpILStmtWhile(FILE* fp, struct bnStringPool* pool, bnILStmtWhile* self,
                       int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "while\n");
        bnDumpILExpression(fp, pool, self->cond, depth + 1);
        for (int i = 0; i < self->statements->len; i++) {
                bnDumpILStatement(fp, pool,
                                  g_ptr_array_index(self->statements, i),
                                  depth + 1);
        }
}

void bnGenerateILStmtWhile(struct bnInterpreter* bone, bnILStmtWhile* self,
                           bnEnviroment* env, bnCompileCache* ccache) {
        int pos = bnGenerateNOP(env);
        bnLabel* loopStart = bnNewLabel(0);
        bnGenerateILExpression(bone, self->cond, env, ccache);
        bnWriteCode(env, BN_OP_GOTO_ELSE);
        bnLabel* loopEnd = bnGenerateLabel(env, 0);
        bnPushStack(ccache->whileStartStack, loopStart);
        bnPushStack(ccache->whileEndStack, loopEnd);
        for (int i = 0; i < self->statements->len; i++) {
                bnGenerateILStatement(
                    bone, g_ptr_array_index(self->statements, i), env, ccache);
        }
        bnWriteCode(env, BN_OP_GOTO);
        // bnGenerateLabel(env, pos);
        bnWriteLabel(env, loopStart);
        g_ptr_array_add(env->labels, loopStart);
        loopStart->pos = pos - bnGetLambdaOffset(env);
        loopEnd->pos = bnGenerateNOP(env) - bnGetLambdaOffset(env);
        bnPopStack(ccache->whileStartStack);
        bnPopStack(ccache->whileEndStack);
}

void bnDeleteILStmtWhile(bnILStmtWhile* self) {
        bnDeleteILExpression(self->cond);
        g_ptr_array_free(self->statements, TRUE);
        BN_FREE(self);
}