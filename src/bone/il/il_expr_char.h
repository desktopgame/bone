#ifndef BONE_IL_ILEXPRCHAR_H
#define BONE_IL_ILEXPRCHAR_H
#include "../bone.h"
struct bnEnviroment;

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

void bnGenerateILExprChar(bnILExprChar* self, struct bnEnviroment* env);

/**
 * free a bnILExprChar.
 * @param self
 */
void bnDeleteILExprChar(bnILExprChar* self);
#endif