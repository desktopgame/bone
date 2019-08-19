#ifndef BONE_UTIL_STRINGPOOL_H
#define BONE_UTIL_STRINGPOOL_H

#define BN_NULL_STRING (NULL)
#define BN_EMPTY_STRING ("")

#define BN_NULL_VIEW (0)
#define BN_EMPTY_VIEW (1)

/**
 * 符号化された文字列を表す型です。
 */
typedef unsigned int bnStringView;

/**
 * bnStringPoolは文字列プールです。
 * 文字列と整数を相互変換するためのマッピングを保存しています。
 */
struct bnStringPool;

/**
 * 新しいbnStringPoolインスタンスを生成して返します。
 * @return
 */
struct bnStringPool* bnNewStringPool();

/**
 * 文字列を整数へ変換して返します。
 * 文字列が既にマッピング済みなら以前の値を返します。
 * @param self
 * @param str
 * @return
 */
bnStringView bnIntern(struct bnStringPool* self, const char* str);

/**
 * 文字列からマッピング済みの値を取得して返します。
 * マッピングされていない場合はアサーションによって失敗します。
 * @param self
 * @param str
 * @return
 */
bnStringView bnStr2View(struct bnStringPool* self, const char* str);

/**
 * 整数からマッピング済みの文字列を取得して返します。
 * マッピングされていない場合はアサーションによって失敗します。
 * @param self
 * @param view
 * @return
 */
const char* bnView2Str(struct bnStringPool* self, bnStringView view);

/**
 * 文字列プールとマッピングのためのテーブルを解放します。
 * @param self
 */
void bnDeleteStringPool(struct bnStringPool* self);
#endif