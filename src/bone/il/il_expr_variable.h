#ifndef BONE_IL_ILEXPRVARIABLE_H
#define BONE_IL_ILEXPRVARIABLE_H
#include "../bone.h"

/**
 * bnILExprVariable is local variable.
 */
typedef struct bnILExprVariable {
        GString* nameRef;
} bnILExprVariable;

/**
 * return new instance of bnILExprVariable.
 * @param nameRef
 * @return
 */
bnILExprVariable* bnNewILExprVariable(GString* nameRef);

/**
 * print a information of bnILExprVariable.
 * @param fp
 * @param self
 * @param depth
 */
void bnDumpILExprVariable(FILE* fp, bnILExprVariable* self, int depth);

/**
 * free a bnILExprVariable.
 * @param self
 */
void bnDeleteILExprVariable(bnILExprVariable* self);
#endif
