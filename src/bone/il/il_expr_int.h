#ifndef BONE_IL_ILEXPRINT_H
#define BONE_IL_ILEXPRINT_H
#include "../bone.h"
#include "compile_cache.h"
struct bnEnviroment;
struct bnInterpreter;

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

void bnGenerateILExprInt(struct bnInterpreter* bone, bnILExprInt* self,
                         struct bnEnviroment* env, bnCompileCache* ccache);

/**
 * free a bnILExprInt.
 * @param self
 */
void bnDeleteILExprInt(bnILExprInt* self);
#endif