#include "il_expr_lambda.h"
#include "../runtime/enviroment.h"
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

void bnGenerateILExprLambda(bnILExprLambda* self, bnEnviroment* env) {}

void bnDeleteILExprLambda(bnILExprLambda* self) {
        g_list_free(self->parameters);
        g_list_free(self->returns);
        g_list_free_full(self->statements, bnDeleteILStatement);
}