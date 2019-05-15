#include "il_expr_memberop.h"
#include "../runtime/enviroment.h"

bnILExprMemberOp* bnNewILExprMemberOp(bnStringView name) {
        bnILExprMemberOp* ret = BN_MALLOC(sizeof(bnILExprMemberOp));
        ret->expr = NULL;
        ret->name = name;
        return ret;
}

void bnDumpILExprMemberOp(FILE* fp, struct bnStringPool* pool,
                          bnILExprMemberOp* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "member %s\n", bnView2Str(pool, self->name));
        bnDumpILExpression(fp, pool, self->expr, depth + 1);
}

void bnGenerateILExprMemberOp(struct bnInterpreter* bone,
                              bnILExprMemberOp* self, bnEnviroment* env,
                              bnCompileCache* ccache) {
        bnGenerateILExpression(bone, self->expr, env, ccache);
        // for instance based closure
        // see: il_expr_funccallop.c
        g_ptr_array_add(env->codeArray, BN_OP_DUP);
        g_ptr_array_add(env->codeArray, BN_OP_SET_REGISTER_0);
        g_ptr_array_add(env->codeArray, BN_OP_GET);
        g_ptr_array_add(env->codeArray, self->name);
}

void bnDeleteILExprMemberOp(bnILExprMemberOp* self) {
        bnDeleteILExpression(self->expr);
        BN_FREE(self);
}