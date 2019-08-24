#ifndef BN_RUNTIME_LABEL_H
#define BN_RUNTIME_LABEL_H
/**
 * bnLabelはジャンプ先の位置を保存する構造体です。
 */
typedef struct bnLabel {
        int pos;
        int refCount;
} bnLabel;

/**
 * 新しいbnLabelインスタンスを生成して返します。
 * @param pos
 * @return
 */
bnLabel* bnNewLabel(int pos);

/**
 * 参照カウントを減らして、0になったならラベルを解放します。
 * @param self
 */
void bnDeleteLabel(bnLabel* self);
#endif