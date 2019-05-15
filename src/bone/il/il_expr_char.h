#ifndef BONE_IL_ILEXPRCHAR_H
#define BONE_IL_ILEXPRCHAR_H
#include "../bone.h"
#include "compile_cache.h"
struct bnEnviroment;
struct bnInterpreter;

/**
 * bnILExprChar is literal of character.
 */
typedef struct bnILExprChar {
        char value;
} bnILExprChar;

/**
 * return new instance of bnILExprChar.
 * @param value
 * @return
 */
bnILExprChar* bnNewILExprChar(char value);

/**
 * print a information of bnILExprChar.
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILExprChar(FILE* fp, struct bnStringPool* pool, bnILExprChar* self,
                      int depth);

void bnGenerateILExprChar(struct bnInterpreter* bone, bnILExprChar* self,
                          struct bnEnviroment* env, bnCompileCache* ccache);

/**
 * free a bnILExprChar.
 * @param self
 */
void bnDeleteILExprChar(bnILExprChar* self);
#endif