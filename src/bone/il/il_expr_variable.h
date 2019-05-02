#ifndef BONE_IL_ILEXPRVARIABLE_H
#define BONE_IL_ILEXPRVARIABLE_H
#include "../bone.h"
struct bnEnviroment;

/**
 * bnILExprVariable is local variable.
 */
typedef struct bnILExprVariable {
        bnStringView name;
} bnILExprVariable;

/**
 * return new instance of bnILExprVariable.
 * @param name
 * @return
 */
bnILExprVariable* bnNewILExprVariable(bnStringView name);

/**
 * print a information of bnILExprVariable.
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprVariable(FILE* fp, struct bnStringPool* pool,
                          bnILExprVariable* self, int depth);

void bnGenerateILExprVariable(bnILExprVariable* self, struct bnEnviroment* env);

/**
 * free a bnILExprVariable.
 * @param self
 */
void bnDeleteILExprVariable(bnILExprVariable* self);
#endif
