#include "il_expression.h"
#include "../bone.h"
#include "../runtime/enviroment.h"
#include "il_expr_all.h"
#include "il_lineno.h"

bnILExpression* bnNewILExpression(bnILExpressionType type) {
        bnILExpression* ret = BN_MALLOC(sizeof(bnILExpression));
        ret->type = type;
        ret->line = -1;
        return ret;
}

void bnDumpILExpression(FILE* fp, struct bnStringPool* pool,
                        bnILExpression* self, int depth) {
        bnDumpLineno(fp, self->line);
        switch (self->type) {
                case BN_IL_EXPR_NONE:
                        break;
                case BN_IL_EXPR_INT:
                        bnDumpILExprInt(fp, pool, self->u.vInt, depth);
                        break;
                case BN_IL_EXPR_DOUBLE:
                        bnDumpILExprDouble(fp, pool, self->u.vDouble, depth);
                        break;
                case BN_IL_EXPR_CHAR:
                        bnDumpILExprChar(fp, pool, self->u.vChar, depth);
                        break;
                case BN_IL_EXPR_STRING:
                        bnDumpILExprString(fp, pool, self->u.vString, depth);
                        break;
                case BN_IL_EXPR_BINOP:
                        bnDumpILExprBinOp(fp, pool, self->u.vBinOp, depth);
                        break;
                case BN_IL_EXPR_UOP:
                        bnDumpILExprUOp(fp, pool, self->u.vUOp, depth);
                        break;
                case BN_IL_EXPR_MEMBEROP:
                        bnDumpILExprMemberOp(fp, pool, self->u.vMemberOp,
                                             depth);
                        break;
                case BN_IL_EXPR_FUNCCALLOP:
                        bnDumpILExprFuncCallOp(fp, pool, self->u.vFuncCallOp,
                                               depth);
                        break;
                case BN_IL_EXPR_VARIABLE:
                        bnDumpILExprVariable(fp, pool, self->u.vVariable,
                                             depth);
                        break;
                case BN_IL_EXPR_LAMBDA:
                        bnDumpILExprLambda(fp, pool, self->u.vLambda, depth);
                        break;
                case BN_IL_EXPR_OBJECT_INJECTION:
                        bnDumpILExprObjectInjection(fp, pool, self->u.vObjInj,
                                                    depth);
                        break;
                case BN_IL_EXPR_ARRAY_SUBSCRIPT:
                        bnDumpILExprArraySubscript(fp, pool, self->u.vArraySub,
                                                   depth);
                        break;
                case BN_IL_EXPR_ARRAY_LIT:
                        bnDumpILExprArrayLit(fp, pool, self->u.vArrayLit,
                                             depth);
                        break;
                default:
                        assert(false);
                        break;
        }
}

void bnGenerateILExpression(struct bnInterpreter* bone, bnILExpression* self,
                            bnEnviroment* env) {
        switch (self->type) {
                case BN_IL_EXPR_NONE:
                        break;
                case BN_IL_EXPR_INT:
                        bnGenerateILExprInt(bone, self->u.vInt, env);
                        break;
                case BN_IL_EXPR_DOUBLE:
                        bnGenerateILExprDouble(bone, self->u.vDouble, env);
                        break;
                case BN_IL_EXPR_CHAR:
                        bnGenerateILExprChar(bone, self->u.vChar, env);
                        break;
                case BN_IL_EXPR_STRING:
                        bnGenerateILExprString(bone, self->u.vString, env);
                        break;
                case BN_IL_EXPR_BINOP:
                        bnGenerateILExprBinOp(bone, self->u.vBinOp, env);
                        break;
                case BN_IL_EXPR_UOP:
                        bnGenerateILExprUOp(bone, self->u.vUOp, env);
                        break;
                case BN_IL_EXPR_MEMBEROP:
                        bnGenerateILExprMemberOp(bone, self->u.vMemberOp, env);
                        break;
                case BN_IL_EXPR_FUNCCALLOP:
                        bnGenerateILExprFuncCallOp(bone, self->u.vFuncCallOp,
                                                   env);
                        break;
                case BN_IL_EXPR_VARIABLE:
                        bnGenerateILExprVariable(bone, self->u.vVariable, env);
                        break;
                case BN_IL_EXPR_LAMBDA:
                        bnGenerateILExprLambda(bone, self->u.vLambda, env);
                        break;
                case BN_IL_EXPR_OBJECT_INJECTION:
                        bnGenerateILExprObjectInjection(bone, self->u.vObjInj,
                                                        env);
                        break;
                case BN_IL_EXPR_ARRAY_SUBSCRIPT:
                        bnGenerateILExprArraySubscript(bone, self->u.vArraySub,
                                                       env);
                        break;
                case BN_IL_EXPR_ARRAY_LIT:
                        bnGenerateILExprArrayLit(bone, self->u.vArrayLit, env);
                        break;
                default:
                        assert(false);
                        break;
        }
        bnAddLineRange(env, self->line);
}

void bnDeleteILExpression(bnILExpression* self) {
        if (self == NULL) {
                return;
        }
        switch (self->type) {
                case BN_IL_EXPR_NONE:
                        break;
                case BN_IL_EXPR_INT:
                        bnDeleteILExprInt(self->u.vInt);
                        break;
                case BN_IL_EXPR_DOUBLE:
                        bnDeleteILExprDouble(self->u.vDouble);
                        break;
                case BN_IL_EXPR_CHAR:
                        bnDeleteILExprChar(self->u.vChar);
                        break;
                case BN_IL_EXPR_STRING:
                        bnDeleteILExprString(self->u.vString);
                        break;
                case BN_IL_EXPR_BINOP:
                        bnDeleteILExprBinOp(self->u.vBinOp);
                        break;
                case BN_IL_EXPR_UOP:
                        bnDeleteILExprUOp(self->u.vUOp);
                        break;
                case BN_IL_EXPR_MEMBEROP:
                        bnDeleteILExprMemberOp(self->u.vMemberOp);
                        break;
                case BN_IL_EXPR_FUNCCALLOP:
                        bnDeleteILExprFuncCallOp(self->u.vFuncCallOp);
                        break;
                case BN_IL_EXPR_VARIABLE:
                        bnDeleteILExprVariable(self->u.vVariable);
                        break;
                case BN_IL_EXPR_LAMBDA:
                        bnDeleteILExprLambda(self->u.vLambda);
                        break;
                case BN_IL_EXPR_OBJECT_INJECTION:
                        bnDeleteILExprObjectInjection(self->u.vObjInj);
                        break;
                case BN_IL_EXPR_ARRAY_SUBSCRIPT:
                        bnDeleteILExprArraySubscript(self->u.vArraySub);
                        break;
                case BN_IL_EXPR_ARRAY_LIT:
                        bnDeleteILExprArrayLit(self->u.vArrayLit);
                        break;
                default:
                        assert(false);
                        break;
        }
        BN_FREE(self);
}