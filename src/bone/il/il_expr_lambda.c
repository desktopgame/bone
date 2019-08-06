#include "il_expr_lambda.h"
#include "../runtime/enviroment.h"
#include "../runtime/interpreter.h"
#include "../runtime/keyword.h"
#include "il_statement.h"

static bool is_instance_base(struct bnStringPool* pool, bnILExprLambda* self);

bnILExprLambda* bnNewILExprLambda(bnStringView filename, int lineno) {
        bnILExprLambda* ret = BN_MALLOC(sizeof(bnILExprLambda));
        ret->Xparameters = g_ptr_array_new_full(2, NULL);
        ret->Xreturns = g_ptr_array_new_full(2, NULL);
        ret->Xstatements = g_ptr_array_new_full(2, bnDeleteILStatement);
        ret->filename = filename;
        ret->lineno = lineno;
        return ret;
}

void bnDumpILExprLambda(FILE* fp, struct bnStringPool* pool,
                        bnILExprLambda* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "lambda\n");
        // parameters
        bnFindent(fp, depth + 1);
        fprintf(fp, "parameters\n");
        for (int i = 0; i < self->Xparameters->len; i++) {
                bnStringView param = g_ptr_array_index(self->Xparameters, i);
                bnFindent(fp, depth + 2);
                fprintf(fp, "%s\n", bnView2Str(pool, param));
        }
        // returns
        bnFindent(fp, depth + 1);
        fprintf(fp, "returns\n");
        for (int i = 0; i < self->Xreturns->len; i++) {
                bnStringView param = g_ptr_array_index(self->Xreturns, i);
                bnFindent(fp, depth + 2);
                fprintf(fp, "%s\n", bnView2Str(pool, param));
        }
        // statements
        bnFindent(fp, depth + 1);
        fprintf(fp, "statements\n");
        for (int i = 0; i < self->Xstatements->len; i++) {
                bnILStatement* stmt = g_ptr_array_index(self->Xstatements, i);
                bnDumpILStatement(fp, pool, stmt, depth + 2);
        }
}

void bnGenerateILExprLambda(bnInterpreter* bone, bnILExprLambda* self,
                            bnEnviroment* env, bnCompileCache* ccache) {
        g_ptr_array_add(env->codeArray, BN_OP_GEN_LAMBDA_BEGIN);
        g_ptr_array_add(env->codeArray, self->lineno);
        g_ptr_array_add(env->codeArray, self->Xparameters->len);
        for (int i = 0; i < self->Xparameters->len; i++) {
                bnStringView name = g_ptr_array_index(self->Xparameters, i);
                g_ptr_array_add(env->codeArray, name);
        }
        g_ptr_array_add(env->codeArray, self->Xreturns->len);
        for (int i = 0; i < self->Xreturns->len; i++) {
                bnStringView name = g_ptr_array_index(self->Xreturns, i);
                g_ptr_array_add(env->codeArray, name);
        }
        bnGenerateEnterLambda(env);
        for (int i = 0; i < self->Xparameters->len; i++) {
                bnStringView name = g_ptr_array_index(self->Xparameters, i);
                g_ptr_array_add(env->codeArray, BN_OP_STORE);
                g_ptr_array_add(env->codeArray, name);
        }
        for (int i = 0; i < self->Xstatements->len; i++) {
                bnILStatement* ilstmt = g_ptr_array_index(self->Xstatements, i);
                bnGenerateILStatement(bone, ilstmt, env, ccache);
        }
        g_ptr_array_add(env->codeArray, BN_OP_DEFER_NEXT);
        bnGenerateExitLambda(env);
        g_ptr_array_add(env->codeArray, BN_OP_GEN_LAMBDA_END);
}

void bnDeleteILExprLambda(bnILExprLambda* self) {
        g_ptr_array_free(self->Xparameters, TRUE);
        g_ptr_array_free(self->Xreturns, TRUE);
        g_ptr_array_free(self->Xstatements, TRUE);
        BN_FREE(self);
}

static bool is_instance_base(struct bnStringPool* pool, bnILExprLambda* self) {
        guint len = g_list_length(self->Xparameters);
        if (len == 0) {
                return false;
        }
        bnStringView name = g_ptr_array_index(self->Xparameters, 0);
        return name == bnIntern(pool, BN_KWD_SELF);
}
