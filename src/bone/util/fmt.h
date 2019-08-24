#ifndef BONE_UTIL_FMT_H
#define BONE_UTIL_FMT_H
#include <stdio.h>

/**
 * bnFindent(FILE*, int)を使用して標準出力にインデントを出力します。
 * @param depth
 */
void bnIndent(int depth);

/**
 * 指定の深さだけインデントを出力します。
 * @param fp
 * @param depth
 */
void bnFindent(FILE* fp, int depth);
#endif