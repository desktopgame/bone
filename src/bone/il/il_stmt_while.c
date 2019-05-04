#include "il_stmt_while.h"
#include "../runtime/enviroment.h"
#include "il_expression.h"
#include "il_statement.h"

bnILStmtWhile* bnNewILStmtWhile(bnILExpression* cond) {
        bnILStmtWhile* ret = BN_MALLOC(sizeof(bnILStmtWhile));
        ret->cond = cond;
        ret->statements = NULL;
        return ret;
}

void bnDumpILStmtWhile(FILE* fp, struct bnStringPool* pool, bnILStmtWhile* self,
                       int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "while\n");
        bnDumpILExpression(fp, pool, self->cond, depth + 1);
        GList* iter = self->statements;
        while (iter != NULL) {
                bnDumpILStatement(fp, pool, iter->data, depth + 1);
                iter = iter->next;
        }
}

void bnGenerateILStmtWhile(struct bnInterpreter* bone, bnILStmtWhile* self,
                           bnEnviroment* env) {
        int pos = bnGenerateNOP(env);
        bnGenerateILExpression(bone, self->cond, env);
        g_ptr_array_add(env->codeArray, BN_OP_GOTO_ELSE);
        bnLabel* loopEnd = bnGenerateLabel(env, 0);
        GList* iter = self->statements;
        while (iter != NULL) {
                bnGenerateILStatement(bone, iter->data, env);
                iter = iter->next;
        }
        g_ptr_array_add(env->codeArray, BN_OP_GOTO);
        bnGenerateLabel(env, pos);
        loopEnd->pos = bnGenerateNOP(env);
}

void bnDeleteILStmtWhile(bnILStmtWhile* self) {
        bnDeleteILExpression(self->cond);
        g_list_free_full(self->statements, bnDeleteILStatement);
        BN_FREE(self);
}