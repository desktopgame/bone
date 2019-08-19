#ifndef BONE_UTIL_IO_H
#define BONE_UTIL_IO_H
#include <stdbool.h>
#include <stdio.h>

#define BN_STDOUT (bnStdout())
#define BN_STDERR (bnStderr())
#define BN_STDIN (bnStdin())

/**
 * stdout, stderr, stdinを保存します。
 */
void bnInitIO();

/**
 * stdoutを返します。
 * @return
 */
FILE* bnStdout();

/**
 * stderrを返します。
 * @return
 */
FILE* bnStderr();

/**
 * stdinを返します。
 * @return
 */
FILE* bnStdin();

/**
 * 指定のファイルが存在するならtrueを返します。
 * @param path
 * @return
 */
bool bnExists(const char* path);
#endif