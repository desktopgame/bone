#include "il_expr_uop.h"
#include "../runtime/enviroment.h"
#include "../runtime/interpreter.h"
#include "../runtime/keyword.h"

static bnStringView opToView(struct bnStringPool* pool, bnILExprUOp* self);

bnILExprUOp* bnNewILExprUOp(bnILUOpType type) {
        bnILExprUOp* ret = BN_MALLOC(sizeof(bnILExprUOp));
        ret->a = NULL;
        ret->type = type;
        return ret;
}

void bnDumpILExprUOp(FILE* fp, struct bnStringPool* pool, bnILExprUOp* self,
                     int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "uop\n");
        bnDumpILExpression(fp, pool, self->a, depth + 1);
}

void bnGenerateILExprUOp(bnInterpreter* bone, bnILExprUOp* self,
                         bnEnviroment* env, bnCompileCache* ccache) {
        bnGenerateILExpression(bone, self->a, env, ccache);
        bnWriteCode(env, BN_OP_CLEANUP_INJBUF);
        bnWriteCode(env, BN_OP_DUP);
        bnWriteCode(env, BN_OP_GET);
        bnWriteCode(env, opToView(bone->pool, self));
        bnWriteCode(env, BN_OP_FUNCCALL);
        bnWriteCode(env, 1);
}

void bnDeleteILExprUOp(bnILExprUOp* self) {
        bnDeleteILExpression(self->a);
        BN_FREE(self);
}
// private
static bnStringView opToView(struct bnStringPool* pool, bnILExprUOp* self) {
        switch (self->type) {
                case BN_IL_UNOP_NOT:
                        return bnIntern(pool, BN_KWD_NOT);
                case BN_IL_UNOP_NEGATIVE:
                        return bnIntern(pool, BN_KWD_NEGATIVE);
		case BN_IL_UNOP_CHILDA:
		        return bnIntern(pool, BN_KWD_CHILDA);
        }
	abort();
	return 0;
}
