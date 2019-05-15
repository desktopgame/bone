#ifndef BONE_UTIL_ARGS_H
#define BONE_UTIL_ARGS_H
#include "../glib.h"

void bnInitArgs(int argc, char* argv[]);

int bnArgc();

char** bnArgv();
#endif