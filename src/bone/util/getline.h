#ifndef BONE_UTIL_GETLINE_H
#define BONE_UTIL_GETLINE_H
#include <stdio.h>

#ifdef _MSC_VER
#define getline_win(lineptr, n, stream) (getline_win(lineptr, n, stream))
#else
#define bnGetLine(lineptr, n, stream) (getline(lineptr, n, stream))
#endif

#ifdef _MSC_VER
size_t getline_win(char **lineptr, size_t *n, FILE *stream);
#endif
#endif