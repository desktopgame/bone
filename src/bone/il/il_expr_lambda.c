#include "il_expr_lambda.h"
#include "../runtime/enviroment.h"
#include "../runtime/interpreter.h"
#include "il_statement.h"

bnILExprLambda* bnNewILExprLambda() {
        bnILExprLambda* ret = BN_MALLOC(sizeof(bnILExprLambda));
        ret->parameters = NULL;
        ret->returns = NULL;
        ret->statements = NULL;
        return ret;
}

void bnDumpILExprLambda(FILE* fp, struct bnStringPool* pool,
                        bnILExprLambda* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "lambda\n");
        // parameters
        bnFindent(fp, depth + 1);
        fprintf(fp, "parameters\n");
        GList* iter = self->parameters;
        while (iter != NULL) {
                bnStringView param = iter->data;
                bnFindent(fp, depth + 2);
                fprintf(fp, "%s\n", bnView2Str(pool, param));
                iter = iter->next;
        }
        // returns
        bnFindent(fp, depth + 1);
        fprintf(fp, "returns\n");
        iter = self->returns;
        while (iter != NULL) {
                bnStringView param = iter->data;
                bnFindent(fp, depth + 2);
                fprintf(fp, "%s\n", bnView2Str(pool, param));
                iter = iter->next;
        }
        // statements
        bnFindent(fp, depth + 1);
        fprintf(fp, "statements\n");
        iter = self->statements;
        while (iter != NULL) {
                bnILStatement* stmt = iter->data;
                bnDumpILStatement(fp, pool, stmt, depth + 2);
                iter = iter->next;
        }
}

void bnGenerateILExprLambda(bnInterpreter* bone, bnILExprLambda* self,
                            bnEnviroment* env) {
        env->binary = g_list_append(env->binary, BN_OP_GEN_LAMBDA_BEGIN);
        GList* iter = self->parameters;
        while (iter != NULL) {
                bnStringView name = iter->data;
                env->binary = g_list_append(env->binary, BN_OP_STORE);
                env->binary =
                    g_list_append(env->binary, bnIntern(bone->pool, name));
                iter = iter->next;
        }
        iter = self->statements;
        while (iter != NULL) {
                bnILStatement* ilstmt = iter->data;
                bnGenerateILStatement(bone, ilstmt, env);
                iter = iter->next;
        }
        env->binary = g_list_append(env->binary, BN_OP_GEN_LAMBDA_END);
}

void bnDeleteILExprLambda(bnILExprLambda* self) {
        g_list_free(self->parameters);
        g_list_free(self->returns);
        g_list_free_full(self->statements, bnDeleteILStatement);
}