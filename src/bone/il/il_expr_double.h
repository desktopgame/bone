#ifndef BONE_IL_ILEXPRDOUBLE_H
#define BONE_IL_ILEXPRDOUBLE_H
#include "../bone.h"
struct bnEnviroment;
/**
 * bnILExprDouble is literal of double precision floating point type.
 */
typedef struct bnILExprDouble {
        double value;
} bnILExprDouble;

/**
 * return new instance of bnILExprDouble.
 * @param value
 * @return
 */
bnILExprDouble* bnNewILExprDouble(double value);

/**
 * print a information of bnILExprDouble.
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprDouble(FILE* fp, struct bnStringPool* pool,
                        bnILExprDouble* self, int depth);

void bnGenerateILExprDouble(bnILExprDouble* self, struct bnEnviroment* env);

/**
 * free a bnILExprDouble.
 * @param self
 */
void bnDeleteILExprDouble(bnILExprDouble* self);
#endif