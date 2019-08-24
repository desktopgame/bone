#ifndef BONE_RUNTIME_LINERANGE_H
#define BONE_RUNTIME_LINERANGE_H

/**
 * bnLineRangeはオペコードのどこからどこまでがどの行に対応しているかを表す構造体です。
 */
typedef struct bnLineRange {
        int start;
        int end;
        int line;
} bnLineRange;

/**
 * 新しいbnLineRangeインスタンスを生成して返します。
 * @return
 */
bnLineRange* bnNewLineRange();

/**
 * ラインレンジを解放します。
 * @param self
 */
void bnDeleteLineRange(bnLineRange* self);
#endif