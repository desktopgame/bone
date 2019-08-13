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
                           bnEnviroment* env, bnCompileCache* ccache) {
        if (self->type == BN_IL_BINOP_ASSIGN) {
                bnILExpression* L = self->left;
                if (L->type == BN_IL_EXPR_VARIABLE) {
                        bnILExprVariable* var = L->u.vVariable;
                        bnGenerateILExpression(bone, self->right, env, ccache);
                        bnWriteCode(env, BN_OP_CLEANUP_INJBUF);
                        bnWriteCode(env, BN_OP_STORE);
                        bnWriteCode(env, var->name);
                } else if (L->type == BN_IL_EXPR_MEMBEROP) {
                        bnGenerateILExpression(bone, self->right, env, ccache);
                        bnWriteCode(env, BN_OP_CLEANUP_INJBUF);
                        bnILExprMemberOp* ilmem = L->u.vMemberOp;
                        bnGenerateILExpression(bone, ilmem->expr, env, ccache);
                        bnWriteCode(env, BN_OP_CLEANUP_INJBUF);
                        bnWriteCode(env, BN_OP_PUT);
                        bnWriteCode(env, ilmem->name);
                } else if (L->type == BN_IL_EXPR_ARRAY_SUBSCRIPT) {
                        bnILExprArraySubscript* arr = L->u.vArraySub;
                        bnGenerateILExpression(bone, arr->arrayExpr, env,
                                               ccache);
                        bnWriteCode(env, BN_OP_CLEANUP_INJBUF);
                        bnWriteCode(env, BN_OP_DUP);
                        bnWriteCode(env, BN_OP_GET);
                        bnWriteCode(env,
                                    bnIntern(bone->pool, BN_KWD_ARRAY_SET));
                        bnGenerateILExpression(bone, arr->indexExpr, env,
                                               ccache);
                        bnWriteCode(env, BN_OP_CLEANUP_INJBUF);
                        bnWriteCode(env, BN_OP_SWAP);
                        bnGenerateILExpression(bone, self->right, env, ccache);
                        bnWriteCode(env, BN_OP_CLEANUP_INJBUF);
                        bnWriteCode(env, BN_OP_SWAP);
                        bnWriteCode(env, BN_OP_FUNCCALL);
                        bnWriteCode(env, 3);
                } else {
                        abort();
                }
        } else if (self->type == BN_IL_BINOP_LOGIC_OR) {
                bnLabel* ifshort = bnNewLabel(-1);
                g_ptr_array_add(env->labels, ifshort);
                bnLabel* iflazy = bnNewLabel(-1);
                g_ptr_array_add(env->labels, iflazy);

                bnGenerateILExpression(bone, self->left, env, ccache);
                bnWriteCode(env, BN_OP_DUP);
                bnWriteCode(env, BN_OP_GOTO_IF);
                bnWriteLabel(env, ifshort);
                bnWriteCode(env, BN_OP_POP);
                bnGenerateILExpression(bone, self->right, env, ccache);
                bnWriteCode(env, BN_OP_DUP);
                bnWriteCode(env, BN_OP_GOTO);
                bnWriteLabel(env, iflazy);
                bnGenerateNOP(env);
                ifshort->pos = bnGenerateNOP(env) - bnGetLambdaOffset(env);
                bnGenerateNOP(env);
                iflazy->pos = bnGenerateNOP(env) - bnGetLambdaOffset(env);
        } else if (self->type == BN_IL_BINOP_LOGIC_AND) {
                bnLabel* ifshort = bnNewLabel(-1);
                g_ptr_array_add(env->labels, ifshort);
                bnLabel* iflazy = bnNewLabel(-1);
                g_ptr_array_add(env->labels, iflazy);

                bnGenerateILExpression(bone, self->left, env, ccache);
                bnWriteCode(env, BN_OP_DUP);
                bnWriteCode(env, BN_OP_GOTO_ELSE);
                bnWriteLabel(env, ifshort);
                bnWriteCode(env, BN_OP_POP);
                bnGenerateILExpression(bone, self->right, env, ccache);
                bnWriteCode(env, BN_OP_DUP);
                bnWriteCode(env, BN_OP_GOTO);
                bnWriteLabel(env, iflazy);
                bnGenerateNOP(env);
                ifshort->pos = bnGenerateNOP(env) - bnGetLambdaOffset(env);
                bnGenerateNOP(env);
                iflazy->pos = bnGenerateNOP(env) - bnGetLambdaOffset(env);
        } else {
                bnGenerateILExpression(bone, self->left, env, ccache);
                bnWriteCode(env, BN_OP_DUP);
                bnWriteCode(env, BN_OP_GET);
                bnWriteCode(env, opToView(bone->pool, self));
                bnGenerateILExpression(bone, self->right, env, ccache);
                bnWriteCode(env, BN_OP_SWAP);
                bnWriteCode(env, BN_OP_FUNCCALL);
                bnWriteCode(env, 2);
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
                case BN_IL_BINOP_LSHIFT:
                        return bnIntern(pool, BN_KWD_LSHIFT);
                case BN_IL_BINOP_RSHIFT:
                        return bnIntern(pool, BN_KWD_RSHIFT);
                case BN_IL_BINOP_NOTEQUAL:
                        return bnIntern(pool, BN_KWD_NOTEQUAL);
        }
}