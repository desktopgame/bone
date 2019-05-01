#ifndef BONE_IL_ILEXPRMEMBEROP_H
#define BONE_IL_ILEXPRMEMBEROP_H
#include "../bone.h"
#include "il_expression.h"

/**
 * bnILExprMemberOp is member operator.
 */
typedef struct bnILExprMemberOp {
        bnILExpression* expr;
        bnStringView name;
} bnILExprMemberOp;

/**
 * return new instance of bnILExprMemberOp.
 * @param name
 * @return
 */
bnILExprMemberOp* bnNewILExprMemberOp(bnStringView name);

/**
 * print a information of bnILExprMemberOp.
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprMemberOp(FILE* fp, struct bnStringPool* pool,
                          bnILExprMemberOp* self, int depth);

/**
 * free a bnILExprMemberOp.
 * @param self
 */
void bnDeleteILExprMemberOp(bnILExprMemberOp* self);
#endif
