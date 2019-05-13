#ifndef BONE_UTIL_IO_H
#define BONE_UTIL_IO_H
#include <stdbool.h>
#include <stdio.h>

#define BN_STDOUT (bnStdout())
#define BN_STDERR (bnStderr())
#define BN_STDIN (bnStdin())

/**
 * save a instance of stdout, stderr and stdin.
 */
void bnInitIO();

/**
 * return a saved instance of stdout.
 * @return
 */
FILE* bnStdout();

/**
 * return a saved instance of stderr.
 * @return
 */
FILE* bnStderr();

/**
 * return a saved instance of stdin
 * @return
 */
FILE* bnStdin();

/**
 * @param path
 * @return
 */
bool bnExists(const char* path);
#endif