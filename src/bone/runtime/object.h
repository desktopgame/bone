#ifndef BONE_RUNTIME_OBJECT_H
#define BONE_RUNTIME_OBJECT_H
#include "../bone.h"
#include "storage.h"

struct bnInterpreter;
struct bnFrame;
struct bnHeap;
struct bnObject;
/**
 * bnFreeObjectFuncはオブジェクトを解放するための関数ポインタ型です。
 */
typedef void (*bnFreeObjectFunc)(bnStorage* storage, bnReference ref,
                                 struct bnObject* obj);

/**
 * bnObjectTypeはオブジェクトの種類を判別するための列挙です。
 */
typedef enum bnObjectType {
        BN_OBJECT_PROTO,
        BN_OBJECT_INTEGER,
        BN_OBJECT_DOUBLE,
        BN_OBJECT_CHAR,
        BN_OBJECT_STRING,
        BN_OBJECT_BOOL,
        BN_OBJECT_LAMBDA,
        BN_OBJECT_ARRAY,
        BN_OBJECT_ANY,
} bnObjectType;

/**
 * bnObjectはbone言語のあらゆるデータを格納する構造体です。
 */
typedef struct bnObject {
        GHashTable* table;
        bool mark;
        bnObjectType type;
        bnFreeObjectFunc freeFunc;
} bnObject;

/**
 * 指定のbnObjectを初期化します。
 * @param bone
 * @param self
 * @param type
 */
void bnInitObject(struct bnInterpreter* bone, bnObject* self,
                  bnObjectType type);

/**
 * 新しいbnObjectインスタンスを生成して、参照を返します。
 * @param bone
 * @return
 */
bnReference bnNewObject(struct bnInterpreter* bone);

/**
 * 指定の名前でオブジェクトに新しいメンバーを定義します。
 * @param self
 * @param name
 * @param ref
 */
void bnDefine(bnObject* self, bnStringView name, bnReference ref);

/**
 * 指定の名前でオブジェクトに新しいメンバーを定義します。
 * @param self
 * @param pool
 * @param str
 * @param ref
 */
void bnDefine2(bnObject* self, struct bnStringPool* pool, const char* str,
               bnReference ref);

/**
 * 指定の名前でオブジェクトのメンバーを取得します。
 * @param self
 * @param name
 * @return
 */
bnReference bnLookup(bnObject* self, bnStringView name);

/**
 * 指定の名前でオブジェクトのメンバーを取得します。
 * @param self
 * @param pool
 * @param str
 * @return
 */
bnReference bnLookup2(bnObject* self, struct bnStringPool* pool,
                      const char* str);

/**
 * 指定の名前のメンバーをオブジェクトから削除します。
 * @param self
 * @param name
 * @return
 */
bool bnUndef(bnObject* self, bnStringView name);

/**
 * 指定の名前のメンバーをオブジェクトから削除します。
 * @param self
 * @param pool
 * @param str
 * @return
 */
bool bnUndef2(bnObject* self, struct bnStringPool* pool, const char* str);

/**
 * 指定の名前でメンバーが定義されているならtrueを返します。
 * @param self
 * @param name
 * @return
 */
bool bnDefined(bnObject* self, bnStringView name);

/**
 * 指定の名前でメンバーが定義されているならtrueを返します。
 * @param self
 * @param pool
 * @param str
 * @return
 */
bool bnDefined2(bnObject* self, struct bnStringPool* pool, const char* str);

/**
 * 指定のフレームの子フレームを作成して、
 * 指定のラムダインスタンスへの参照を実行します。
 * 戻り値として子フレームを返します。
 * @param ref
 * @param bone
 * @param frame
 * @param argc
 * @return
 */
struct bnFrame* bnFuncCall(bnReference ref, struct bnInterpreter* bone,
                           struct bnFrame* frame, int argc);

/**
 * オブジェクトのデバッグ表現を出力します。
 * @param fp
 * @param bone
 * @param self
 */
void bnPrintObject(FILE* fp, struct bnInterpreter* bone, bnObject* self);

/**
 * インジェクションのためのバッファをクリアします。
 * @param pool
 * @param self
 */
void bnCleanupInjectionBuffer(struct bnStringPool* pool, bnObject* self);

/**
 * 指定のメンバーをエクスポートするための修飾済みの名前を返します。
 * @param pool
 * @param view
 * @return
 */
bnStringView bnGetExportVariableName(struct bnStringPool* pool,
                                     bnStringView view);

/**
 * オブジェクトとテーブルを解放します。
 * 必要なら、解放用の関数ポインタも実行します。
 * @param storage
 * @param ref
 * @param obj
 * @return
 */
void bnDeleteObject(bnStorage* storage, bnReference ref, struct bnObject* obj);
#endif