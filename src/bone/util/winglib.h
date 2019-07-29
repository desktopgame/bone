#ifndef BONE_UTIL_WINGLIB_H
#define BONE_UTIL_WINGLIB_H
#if _MSC_VER
struct GString;
/**
 * @param str
 * @param c
 * @return
 */
struct GString* bnGStringAppendC(struct GString* str, char c);
#else

#include "../glib.h"
#define bnGStringAppendC(str, c) (g_string_append_c(str, c))
#endif
#endif