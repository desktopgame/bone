#ifndef BONE_RUNTIME_INTERACTIVE_H
#define BONE_RUNTIME_INTERACTIVE_H
#include <stdio.h>

/**
 * 指定のファイルを入力としてインタラクティブモードを開始します。
 * 入力は常に"1行内で正しい"必要があります。
 * @param in
 * @return 正常終了したなら0
 */
int bnInteractive(FILE* in);
#endif