#ifndef BONE_UTIL_IO_H
#define BONE_UTIL_IO_H
#include <stdio.h>

#define BN_STDOUT (bnStdout())
#define BN_STDERR (bnStderr())
#define BN_STDIN (bnStdin())

void bnInitIO();

FILE* bnStdout();

FILE* bnStderr();

FILE* bnStdin();
#endif