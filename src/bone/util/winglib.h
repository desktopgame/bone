#ifndef BONE_UTIL_WINGLIB_H
#define BONE_UTIL_WINGLIB_H
#if _MSC_VER
struct GString;
/**
 * g_string_append_cの代わりにg_string_appendを呼び出す関数です。
 * Windows環境ではg_string_append_cが正しく動作しないために用意しました。
 * @param str
 * @param c
 * @return
 */
struct GString* bnGStringAppendC(struct GString* str, char c);
#else

/**
 * g_string_append_cのラッパーです。
 * Windows環境では異なる実装に置き換えられます。
 * @param str
 * @param c
 * @return
 */
#define bnGStringAppendC(str, c) (g_string_append_c(str, c))
#endif
#endif