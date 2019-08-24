#ifndef BONE_IL_COMPILECACHE_H
#define BONE_IL_COMPILECACHE_H
#include "../util/stack.h"

/**
 * bnCompileCacheはバイトコードへの変換中に、
 * 構文木をまたいで参照する必要がある情報を保存する構造体です。
 */
typedef struct bnCompileCache {
        bnStack* whileStartStack;
        bnStack* whileEndStack;
} bnCompileCache;

/**
 * 新しいbnCompileCacheインスタンスを生成して返します。
 * @return
 */
bnCompileCache* bnNewCompileCache();

/**
 * キャッシュを全て解放します。
 * @param self
 */
void bnDeleteCompileCache(bnCompileCache* self);
#endif