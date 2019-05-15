#ifndef BONE_IL_ILEXPRBINOP_H
#define BONE_IL_ILEXPRBINOP_H
#include "../bone.h"
#include "il_expression.h"
struct bnEnviroment;
struct bnInterpreter;
/**
 * bnILBinOpType is type of binary operator.
 */
typedef enum bnILBinOpType {
        BN_IL_BINOP_PLUS,
        BN_IL_BINOP_MINUS,
        BN_IL_BINOP_MULTIPLY,
        BN_IL_BINOP_DIVIDE,
        BN_IL_BINOP_MODULO,
        BN_IL_BINOP_BIT_OR,
        BN_IL_BINOP_BIT_AND,
        BN_IL_BINOP_LOGIC_OR,
        BN_IL_BINOP_LOGIC_AND,
        BN_IL_BINOP_EXC_OR,
        BN_IL_BINOP_LSHIFT,
        BN_IL_BINOP_RSHIFT,
        BN_IL_BINOP_ASSIGN,
        BN_IL_BINOP_LT,
        BN_IL_BINOP_LE,
        BN_IL_BINOP_GT,
        BN_IL_BINOP_GE,
        BN_IL_BINOP_EQUAL,
        BN_IL_BINOP_NOTEQUAL,
} bnILBinOpType;

/**
 * bnILExprBinOp is binary operator.
 */
typedef struct bnILExprBinOp {
        bnILExpression* left;
        bnILExpression* right;
        bnILBinOpType type;
} bnILExprBinOp;

/**
 * return new instance of bnILExprBinOp.
 * @param type
 * @return
 */
bnILExprBinOp* bnNewILExprBinOp(bnILBinOpType type);

/**
 * print a information of bnILExprBinOp.
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprBinOp(FILE* fp, struct bnStringPool* pool, bnILExprBinOp* self,
                       int depth);

void bnGenerateILExprBinOp(struct bnInterpreter* bone, bnILExprBinOp* self,
                           struct bnEnviroment* env, bnCompileCache* ccache);

/**
 * free a bnILExprBinOp.
 * @param self
 */
void bnDeleteILExprBinOp(bnILExprBinOp* self);
#endif
