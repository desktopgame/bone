#ifndef BONE_IL_ILEXPRINT_H
#define BONE_IL_ILEXPRINT_H
#include "../bone.h"
struct bnEnviroment;

/**
 * bnILExprInt is literal of integer.
 */
typedef struct bnILExprInt {
        int value;
} bnILExprInt;

/**
 * return new instance of bnILExprInt.
 * @param value
 * @return
 */
bnILExprInt* bnNewILExprInt(int value);

/**
 * print a information of bnILExprInt.
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprInt(FILE* fp, struct bnStringPool* pool, bnILExprInt* self,
                     int depth);

void bnGenerateILExprInt(bnILExprInt* self, struct bnEnviroment* env);

/**
 * free a bnILExprInt.
 * @param self
 */
void bnDeleteILExprInt(bnILExprInt* self);
#endif