#ifndef BONE_IL_ILEXPRMEMBEROP_H
#define BONE_IL_ILEXPRMEMBEROP_H
#include "../bone.h"
#include "il_expression.h"

/**
 * bnILExprMemberOp is member operator.
 */
typedef struct bnILExprMemberOp {
        bnILExpression* expr;
        GString* nameRef;
} bnILExprMemberOp;

/**
 * return new instance of bnILExprMemberOp.
 * @param nameRef
 * @return
 */
bnILExprMemberOp* bnNewILExprMemberOp(GString* nameRef);

/**
 * print a information of bnILExprMemberOp.
 * @param fp
 * @param self
 * @param depth
 */
void bnDumpILExprMemberOp(FILE* fp, bnILExprMemberOp* self, int depth);

/**
 * free a bnILExprMemberOp.
 * @param self
 */
void bnDeleteILExprMemberOp(bnILExprMemberOp* self);
#endif
