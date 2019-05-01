#ifndef BONE_IL_ILEXPRUOP_H
#define BONE_IL_ILEXPRUOP_H
#include "../bone.h"
#include "il_expression.h"

/**
 * bnILUOpType is type of binary operator.
 */
typedef enum bnILUOpType {
        BN_IL_UNOP_NOT,
        BN_IL_UNOP_NEGATIVE,
        BN_IL_UNOP_CHILDA,
} bnILUOpType;

/**
 * bnILExprUOp is unary operator.
 */
typedef struct bnILExprUOp {
        bnILExpression* a;
        bnILUOpType type;
} bnILExprUOp;

/**
 * return new instance of bnILExprUOp.
 * @param type
 * @return
 */
bnILExprUOp* bnNewILExprUOp(bnILUOpType type);

/**
 * print a information of bnILExprUOp.
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprUOp(FILE* fp, struct bnStringPool* pool, bnILExprUOp* self,
                     int depth);

/**
 * free a bnILExprUOp.
 * @param self
 */
void bnDeleteILExprUOp(bnILExprUOp* self);
#endif
