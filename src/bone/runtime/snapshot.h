#ifndef BONE_RUNTIME_SNAPSHOT_H
#define BONE_RUNTIME_SNAPSHOT_H
#include "../glib.h"

/**
 * bnSnapShotはインタプリタの実行中の状態を保存する構造体です。
 * deferによる遅延実行を実装するために使用されます。
 */
typedef struct bnSnapShot {
        int pc;
        GHashTable* table;
} bnSnapShot;

/**
 * 新しいbnSnapShotインスタンスを返します。
 * @param pc
 * @return
 */
bnSnapShot* bnNewSnapShot(int pc);

/**
 * スナップショットを解放します。
 * @param self
 */
void bnDeleteSnapShot(bnSnapShot* self);
#endif