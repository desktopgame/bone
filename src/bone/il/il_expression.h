#ifndef BONE_IL_ILEXPRESSION_H
#define BONE_IL_ILEXPRESSION_H
#include <stdio.h>
#include "../util/string_pool.h"
struct bnInterpreter;

typedef enum bnILExpressionType {
        BN_IL_EXPR_NONE,
        BN_IL_EXPR_INT,
        BN_IL_EXPR_DOUBLE,
        BN_IL_EXPR_CHAR,
        BN_IL_EXPR_STRING,
        BN_IL_EXPR_BINOP,
        BN_IL_EXPR_UOP,
        BN_IL_EXPR_MEMBEROP,
        BN_IL_EXPR_VARIABLE,
        BN_IL_EXPR_FUNCCALLOP,
        BN_IL_EXPR_LAMBDA,
        BN_IL_EXPR_OBJECT_INJECTION,
        BN_IL_EXPR_ARRAY_SUBSCRIPT
} bnILExpressionType;

struct bnEnviroment;
struct bnILExprInt;
struct bnILExprDouble;
struct bnILExprChar;
struct bnILExprString;
struct bnILExprBinOp;
struct bnILExprUOp;
struct bnILExprMemberOp;
struct bnILExprVariable;
struct bnILExprFuncCallOp;
struct bnILExprLambda;
struct bnILExprObjectInjection;
struct bnILExprArraySubscript;

typedef struct bnILExpression {
        bnILExpressionType type;
        union {
                struct bnILExprInt* vInt;
                struct bnILExprDouble* vDouble;
                struct bnILExprChar* vChar;
                struct bnILExprString* vString;
                struct bnILExprBinOp* vBinOp;
                struct bnILExprUOp* vUOp;
                struct bnILExprMemberOp* vMemberOp;
                struct bnILExprVariable* vVariable;
                struct bnILExprFuncCallOp* vFuncCallOp;
                struct bnILExprLambda* vLambda;
                struct bnILExprObjectInjection* vObjInj;
                struct bnILExprArraySubscript* vArraySub;
        } u;
        int line;
} bnILExpression;

bnILExpression* bnNewILExpression(bnILExpressionType type);

void bnDumpILExpression(FILE* fp, struct bnStringPool* pool,
                        bnILExpression* self, int depth);

void bnGenerateILExpression(struct bnInterpreter* bone, bnILExpression* self,
                            struct bnEnviroment* env);

void bnDeleteILExpression(bnILExpression* self);
#endif