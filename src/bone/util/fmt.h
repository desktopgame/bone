#ifndef BONE_UTIL_FMT_H
#define BONE_UTIL_FMT_H
#include <stdio.h>

/**
 * call bnFindent with  stdout.
 * @param depth
 */
void bnIndent(int depth);

/**
 * output spaces to file stream.
 * @param fp
 * @param depth
 */
void bnFindent(FILE* fp, int depth);
#endif