#ifndef BONE_RUNTIME_MODULE_H
#define BONE_RUNTIME_MODULE_H
#include "../glib.h"

/**
 * bnModuleは動的リンクライブラリへのハンドルを保存する構造体です。
 */
typedef struct bnModule {
        char* path;
        void* handle;
} bnModule;

/**
 * 指定のパスの動的リンクライブラリをロードして、
 * 新しいbnModuleインスタンスを返します。
 * @param path
 * @return
 */
bnModule* bnNewModule(const char* path);

/**
 * 指定の名前でシンボルを読み込んで返します。
 * 実際に検索される名前は ファイル名_name です。
 * @param self
 * @param name
 * @return
 */
void* bnGetSymbol(bnModule* self, const char* name);

/**
 * ハンドルをクローズして、
 * モジュールを解放します。
 * @param self
 */
void bnDeleteModule(bnModule* self);
#endif