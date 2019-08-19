#ifndef BONE_RUNTIME_STORAGE_H
#define BONE_RUNTIME_STORAGE_H
#include <stdbool.h>

/**
 * bnReferenceはオブジェクトのハンドルです。
 */
typedef int* bnReference;

/**
 * bnStorageはコンパクション可能なオブジェクトプールです。
 */
typedef struct bnStorage {
        char* pool;
        int* table;
        bool* bitmap;
        int objectSize;
        int objectCount;
        int use;
        int offset;
        int nextFree;
        struct bnStorage* next;
} bnStorage;

/**
 * 新しいbnStorageインスタンスを生成して返します。
 * @param objectSize
 * @param objectCount
 * @return
 */
bnStorage* bnNewStorage(int objectSize, int objectCount);

/**
 * 未使用のオブジェクトを検索して返します。
 * 返されるオブジェクトは使用済みとしてマークされます。
 * @param self
 * @return
 */
bnReference bnAllocMemory(bnStorage* self);

/**
 * オブジェクトを未使用としてマークします。
 * @param self
 * @param index
 */
void bnFreeMemory(bnStorage* self, bnReference index);

/**
 * 指定のハンドルから実際のポインタを返します。
 * @param self
 * @param index
 */
void* bnGetMemory(bnStorage* self, bnReference index);

/**
 * 指定のハンドルから対応するストレージと相対的な位置を返します。
 * @param self
 * @param index
 * @param outFixedIndex
 * @return
 */
bnStorage* bnGetStorage(bnStorage* self, bnReference index, int* outFixedPos);

/**
 * グローバルストレージインデックスからハンドルを返します。
 * デバッグ用の関数です。
 * @param self
 * @param gindex
 * @return
 */
bnReference bnGetReferenceFromGlobalStorageIndex(bnStorage* self, int gindex);

/**
 * ポインタからグローバルストレージインデックスを返します。
 * ハンドルを格納するべきところに誤ってオブジェクトを格納していないか確認できます。
 * デバッグ用の関数です。
 * @param self
 * @param ptr
 * @return
 */
int bnGetGlobalStorageIndexFromPointer(bnStorage* self, void* ptr);

/**
 * ストレージの使用状態を出力します。
 * デバッグ用の関数です。
 * @param self
 */
void bnShowStorage(bnStorage* self);

/**
 * コンパクションを実行します。
 * @param self
 */
void bnCompact(bnStorage* self);

/**
 * オブジェクトをプールごと解放します。
 * 連結された他の全てのストレージも解放されます。
 * @param self
 */
void bnDeleteStorage(bnStorage* self);
#endif