#ifndef BONE_RUNTIME_ANY_H
#define BONE_RUNTIME_ANY_H
#include "object.h"
struct bnInterpreter;

/**
 * bnAnyは拡張ライブラリのための構造体です。
 * 独自に定義される構造体はこれにキャスト可能なポインタでなければなりません。
 */
typedef struct bnAny {
        bnObject base;
        bnStringView type;
} bnAny;

/**
 * bnAnyインスタンスを初期化します。
 * @param bone
 * @param self
 * @param type 拡張ライブラリの指定の型を一意に表す名前
 */
void bnInitAny(struct bnInterpreter* bone, bnAny* self, const char* type);

/**
 * bnAnyが指定のタイプであるかどうか判別します。
 * @param bone
 * @param self
 * @parram type
 * @return
 */
bool bnMatchType(struct bnInterpreter* bone, bnObject* self, const char* type);
#endif