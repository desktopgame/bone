#include "il_stmt_panic.h"
#include "../runtime/enviroment.h"
#include "../runtime/interpreter.h"
#include "../runtime/keyword.h"
#include "il_expression.h"

bnILStmtPanic* bnNewILStmtPanic(bnStringView name, bnILExpression* expr) {
        bnILStmtPanic* ret = BN_MALLOC(sizeof(bnILStmtPanic));
        ret->name = name;
        ret->expr = expr;
        return ret;
}

void bnDumpILStmtPanic(FILE* fp, struct bnStringPool* pool, bnILStmtPanic* self,
                       int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "panic\n");
        bnFindent(fp, depth + 1);
        fprintf(fp, "%s\n", bnView2Str(pool, self->name));
        bnDumpILExpression(fp, pool, self->expr, depth + 1);
}

void bnGenerateILStmtPanic(bnInterpreter* bone, bnILStmtPanic* self,
                           bnEnviroment* env) {
        bnGenerateILExpression(bone, self->expr, env);
        g_ptr_array_add(env->codeArray, BN_OP_STORE);
        g_ptr_array_add(env->codeArray, self->name);
        g_ptr_array_add(env->codeArray, BN_OP_PANIC);
        g_ptr_array_add(env->codeArray, self->name);
}

void bnDeleteILStmtPanic(bnILStmtPanic* self) {}