#include "il_expr_binop.h"
#include "../runtime/enviroment.h"
#include "../runtime/interpreter.h"
#include "../runtime/keyword.h"
#include "il_expr_all.h"

static bnStringView opToView(struct bnStringPool* pool, bnILExprBinOp* self);

bnILExprBinOp* bnNewILExprBinOp(bnILBinOpType type) {
        bnILExprBinOp* ret = BN_MALLOC(sizeof(bnILExprBinOp));
        ret->left = NULL;
        ret->right = NULL;
        ret->type = type;
        return ret;
}

void bnDumpILExprBinOp(FILE* fp, struct bnStringPool* pool, bnILExprBinOp* self,
                       int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "binop %s\n", bnView2Str(pool, opToView(pool, self)));
        bnDumpILExpression(fp, pool, self->left, depth + 1);
        bnDumpILExpression(fp, pool, self->right, depth + 1);
}

void bnGenerateILExprBinOp(bnInterpreter* bone, bnILExprBinOp* self,
                           bnEnviroment* env) {
        if (self->type == BN_IL_BINOP_ASSIGN) {
                bnILExpression* L = self->left;
                if (L->type == BN_IL_EXPR_VARIABLE) {
                        bnILExprVariable* var = L->u.vVariable;
                        bnGenerateILExpression(bone, self->right, env);
                        g_ptr_array_add(env->codeArray, BN_OP_STORE);
                        g_ptr_array_add(env->codeArray, var->name);
                } else if (L->type == BN_IL_EXPR_MEMBEROP) {
                        bnGenerateILExpression(bone, self->right, env);
                        bnILExprMemberOp* ilmem = L->u.vMemberOp;
                        bnGenerateILExpression(bone, ilmem->expr, env);
                        g_ptr_array_add(env->codeArray, BN_OP_PUT);
                        g_ptr_array_add(env->codeArray, ilmem->name);
                } else {
                        abort();
                }
        } else {
                bnGenerateILExpression(bone, self->left, env);
                g_ptr_array_add(env->codeArray, BN_OP_DUP);
                g_ptr_array_add(env->codeArray, BN_OP_GET);
                g_ptr_array_add(env->codeArray, opToView(bone->pool, self));
                bnGenerateILExpression(bone, self->right, env);
                g_ptr_array_add(env->codeArray, BN_OP_SWAP);
                g_ptr_array_add(env->codeArray, BN_OP_FUNCCALL);
                g_ptr_array_add(env->codeArray, 2);
        }
}

void bnDeleteILExprBinOp(bnILExprBinOp* self) {
        bnDeleteILExpression(self->left);
        bnDeleteILExpression(self->right);
        BN_FREE(self);
}
// private
static bnStringView opToView(struct bnStringPool* pool, bnILExprBinOp* self) {
        switch (self->type) {
                case BN_IL_BINOP_PLUS:
                        return bnIntern(pool, BN_KWD_PLUS);
                case BN_IL_BINOP_MINUS:
                        return bnIntern(pool, BN_KWD_MINUS);
                case BN_IL_BINOP_MULTIPLY:
                        return bnIntern(pool, BN_KWD_MULTIPLY);
                case BN_IL_BINOP_DIVIDE:
                        return bnIntern(pool, BN_KWD_DIVIDE);
                case BN_IL_BINOP_MODULO:
                        return bnIntern(pool, BN_KWD_MODULO);
                case BN_IL_BINOP_BIT_OR:
                        return bnIntern(pool, BN_KWD_BIT_OR);
                case BN_IL_BINOP_BIT_AND:
                        return bnIntern(pool, BN_KWD_BIT_AND);
                case BN_IL_BINOP_LOGIC_OR:
                        return bnIntern(pool, "BN_KWD_LOGIC_OR");
                case BN_IL_BINOP_LOGIC_AND:
                        return bnIntern(pool, "BN_KWD_LOGIC_AND");
                case BN_IL_BINOP_EXC_OR:
                        return bnIntern(pool, BN_KWD_EXC_OR);
                case BN_IL_BINOP_LT:
                        return bnIntern(pool, BN_KWD_LT);
                case BN_IL_BINOP_LE:
                        return bnIntern(pool, BN_KWD_LE);
                case BN_IL_BINOP_GT:
                        return bnIntern(pool, BN_KWD_GT);
                case BN_IL_BINOP_GE:
                        return bnIntern(pool, BN_KWD_GE);
                case BN_IL_BINOP_ASSIGN:
                        return bnIntern(pool, ":=");
                case BN_IL_BINOP_EQUAL:
                        return bnIntern(pool, BN_KWD_EQUAL);
                case BN_IL_BINOP_NOTEQUAL:
                        return bnIntern(pool, BN_KWD_NOTEQUAL);
        }
}