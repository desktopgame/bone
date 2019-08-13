#include "il_expr_lambda.h"
#include "../runtime/enviroment.h"
#include "../runtime/interpreter.h"
#include "../runtime/keyword.h"
#include "il_statement.h"

static bool is_instance_base(struct bnStringPool* pool, bnILExprLambda* self);

bnILExprLambda* bnNewILExprLambda(bnStringView filename, int lineno) {
        bnILExprLambda* ret = BN_MALLOC(sizeof(bnILExprLambda));
        ret->parameters = g_ptr_array_new_full(2, NULL);
        ret->returns = g_ptr_array_new_full(2, NULL);
        ret->statements = g_ptr_array_new_full(2, bnDeleteILStatement);
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
        for (int i = 0; i < self->parameters->len; i++) {
                bnStringView param = g_ptr_array_index(self->parameters, i);
                bnFindent(fp, depth + 2);
                fprintf(fp, "%s\n", bnView2Str(pool, param));
        }
        // returns
        bnFindent(fp, depth + 1);
        fprintf(fp, "returns\n");
        for (int i = 0; i < self->returns->len; i++) {
                bnStringView param = g_ptr_array_index(self->returns, i);
                bnFindent(fp, depth + 2);
                fprintf(fp, "%s\n", bnView2Str(pool, param));
        }
        // statements
        bnFindent(fp, depth + 1);
        fprintf(fp, "statements\n");
        for (int i = 0; i < self->statements->len; i++) {
                bnILStatement* stmt = g_ptr_array_index(self->statements, i);
                bnDumpILStatement(fp, pool, stmt, depth + 2);
        }
}

void bnGenerateILExprLambda(bnInterpreter* bone, bnILExprLambda* self,
                            bnEnviroment* env, bnCompileCache* ccache) {
        bnWriteCode(env, BN_OP_GEN_LAMBDA_BEGIN);
        bnWriteCode(env, self->lineno);
        bnWriteCode(env, self->parameters->len);
        for (int i = 0; i < self->parameters->len; i++) {
                bnStringView name = g_ptr_array_index(self->parameters, i);
                bnWriteCode(env, name);
        }
        bnWriteCode(env, self->returns->len);
        for (int i = 0; i < self->returns->len; i++) {
                bnStringView name = g_ptr_array_index(self->returns, i);
                bnWriteCode(env, name);
        }
        bnGenerateEnterLambda(env);
        for (int i = 0; i < self->parameters->len; i++) {
                bnStringView name = g_ptr_array_index(self->parameters, i);
                bnWriteCode(env, BN_OP_STORE);
                bnWriteCode(env, name);
        }
        for (int i = 0; i < self->statements->len; i++) {
                bnILStatement* ilstmt = g_ptr_array_index(self->statements, i);
                bnGenerateILStatement(bone, ilstmt, env, ccache);
        }
        bnWriteCode(env, BN_OP_DEFER_NEXT);
        bnGenerateExitLambda(env);
        bnWriteCode(env, BN_OP_GEN_LAMBDA_END);
}

void bnDeleteILExprLambda(bnILExprLambda* self) {
        g_ptr_array_free(self->parameters, TRUE);
        g_ptr_array_free(self->returns, TRUE);
        g_ptr_array_free(self->statements, TRUE);
        BN_FREE(self);
}

static bool is_instance_base(struct bnStringPool* pool, bnILExprLambda* self) {
        guint len = g_list_length(self->parameters);
        if (len == 0) {
                return false;
        }
        bnStringView name = g_ptr_array_index(self->parameters, 0);
        return name == bnIntern(pool, BN_KWD_SELF);
}
