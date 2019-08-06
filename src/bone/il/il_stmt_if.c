#include "il_stmt_if.h"
#include "../runtime/enviroment.h"
#include "il_expression.h"
#include "il_statement.h"

bnILStmtIf* bnNewILStmtIf(bnILExpression* cond) {
        bnILStmtIf* ret = BN_MALLOC(sizeof(bnILStmtIf));
        ret->cond = cond;
        ret->statements = g_ptr_array_new_full(2, bnDeleteILStatement);
        return ret;
}

void bnDumpILStmtIf(FILE* fp, struct bnStringPool* pool, bnILStmtIf* self,
                    int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "If\n");
        bnDumpILExpression(fp, pool, self->cond, depth + 1);
        for (int i = 0; i < self->statements->len; i++) {
                bnDumpILStatement(fp, pool,
                                  g_ptr_array_index(self->statements, i),
                                  depth + 1);
        }
}

void bnGenerateILStmtIf(struct bnInterpreter* bone, bnILStmtIf* self,
                        bnEnviroment* env, bnCompileCache* ccache) {
        // generate condition
        bnGenerateILExpression(bone, self->cond, env, ccache);
        g_ptr_array_add(env->codeArray, BN_OP_GOTO_ELSE);
        bnLabel* ifFalse = bnGenerateLabel(env, -1);
        for (int i = 0; i < self->statements->len; i++) {
                bnGenerateILStatement(
                    bone, g_ptr_array_index(self->statements, i), env, ccache);
        }
        ifFalse->pos = bnGenerateNOP(env) - bnGetLambdaOffset(env);
}

void bnDeleteILStmtIf(bnILStmtIf* self) {
        bnDeleteILExpression(self->cond);
        g_ptr_array_free(self->statements, TRUE);
        BN_FREE(self);
}

bnILStmtIfElse* bnNewILStmtIfElse(bnILStmtIf* trueCase) {
        bnILStmtIfElse* ret = BN_MALLOC(sizeof(bnILStmtIfElse));
        ret->trueCase = trueCase;
        ret->Xstatements = g_ptr_array_new_full(2, bnDeleteILStatement);
        return ret;
}

void bnDumpILStmtIfElse(FILE* fp, struct bnStringPool* pool,
                        bnILStmtIfElse* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "if else\n");
        bnDumpILStmtIf(fp, pool, self->trueCase, depth + 1);
        for (int i = 0; i < self->Xstatements->len; i++) {
                bnDumpILStatement(fp, pool,
                                  g_ptr_array_index(self->Xstatements, i),
                                  depth + 1);
        }
}

void bnGenerateILStmtIfElse(struct bnInterpreter* bone, bnILStmtIfElse* self,
                            bnEnviroment* env, bnCompileCache* ccache) {
        // if(cond) { ... }
        bnGenerateILExpression(bone, self->trueCase->cond, env, ccache);
        g_ptr_array_add(env->codeArray, BN_OP_GOTO_ELSE);
        bnLabel* ifFalse = bnGenerateLabel(env, -1);
        for (int i = 0; i < self->trueCase->statements->len; i++) {
                bnGenerateILStatement(
                    bone, g_ptr_array_index(self->trueCase->statements, i), env,
                    ccache);
        }
        g_ptr_array_add(env->codeArray, BN_OP_GOTO);
        bnLabel* ifTrue = bnGenerateLabel(env, -1);
        // if(cond) { ... } else { ... }
        ifFalse->pos = bnGenerateNOP(env) - bnGetLambdaOffset(env);
        for (int i = 0; i < self->Xstatements->len; i++) {
                bnGenerateILStatement(
                    bone, g_ptr_array_index(self->Xstatements, i), env, ccache);
        }
        ifTrue->pos = bnGenerateNOP(env) - bnGetLambdaOffset(env);
}

void bnDeleteILStmtIfElse(bnILStmtIfElse* self) {
        bnDeleteILStmtIf(self->trueCase);
        g_ptr_array_free(self->Xstatements, TRUE);
        BN_FREE(self);
}