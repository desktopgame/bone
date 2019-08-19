#ifndef BONE_UTIL_STRING_H
#define BONE_UTIL_STRING_H

/**
 * 文字列を後ろから辿って、最初に / が見つかった位置を返します。
 * つまり、パスからファイル名を取得するために使えます。
 * @param path
 * @return
 */
int bnLastPathComponent(const char* path);
#endif