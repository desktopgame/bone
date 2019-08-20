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
        bnWriteCode(env, BN_OP_DUP);
        bnWriteCode(env, BN_OP_SET_REGISTER_0);
        bnWriteCode(env, BN_OP_GET);
        bnWriteCode(env, self->name);
}

void bnDeleteILExprMemberOp(bnILExprMemberOp* self) {
        bnDeleteILExpression(self->expr);
        BN_FREE(self);
}