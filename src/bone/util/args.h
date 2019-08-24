#ifndef BONE_UTIL_ARGS_H
#define BONE_UTIL_ARGS_H
#include "../glib.h"

/**
 * argc, argv を保存してグローバルに参照可能にします。
 * @param argc
 * @param argv
 */
void bnInitArgs(int argc, char* argv[]);

/**
 * argcを返します。
 * @return
 */
int bnArgc();

/**
 * argvを返します。
 * @return
 */
char** bnArgv();
#endif