#include "il_expr_lambda.h"
#include "../runtime/enviroment.h"
#include "../runtime/interpreter.h"
#include "../runtime/keyword.h"
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
        g_ptr_array_add(env->codeArray, BN_OP_GEN_LAMBDA_BEGIN);
        g_ptr_array_add(env->codeArray, bnIsInstanceBase(bone->pool, self));
        GList* iter = self->parameters;
        while (iter != NULL) {
                bnStringView name = iter->data;
                g_ptr_array_add(env->codeArray, BN_OP_STORE);
                g_ptr_array_add(env->codeArray, name);
                iter = iter->next;
        }
        iter = self->statements;
        while (iter != NULL) {
                bnILStatement* ilstmt = iter->data;
                bnGenerateILStatement(bone, ilstmt, env);
                iter = iter->next;
        }
        g_ptr_array_add(env->codeArray, BN_OP_GEN_LAMBDA_END);
}

bool bnIsInstanceBase(struct bnStringPool* pool, bnILExprLambda* self) {
        guint len = g_list_length(self->parameters);
        if (len == 0) {
                return false;
        }
        bnStringView name = self->parameters->data;
        return name == bnIntern(pool, BN_KWD_SELF);
}

bool bnIsNamedReturn(bnILExprLambda* self) {
        guint len = g_list_length(self->returns);
        return len > 0;
}

void bnDeleteILExprLambda(bnILExprLambda* self) {
        g_list_free(self->parameters);
        g_list_free(self->returns);
        g_list_free_full(self->statements, bnDeleteILStatement);
}