#ifndef BONE_RUNTIME_INTERPRETER_H
#define BONE_RUNTIME_INTERPRETER_H
#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include "../glib.h"
#include "../util/jump_stack.h"
#include "../util/stack.h"
#include "../util/string_pool.h"
#include "storage.h"

#define BN_JMP_CODE_EXCEPTION (1)

struct bnObject;
struct bnFrame;
struct bnHeap;
enum bnObjectType;

/**
 * bnInterpreterはboneの実行に必要なあらゆる情報のルートです。
 * グローバル変数を持たないので、インタプリタのあらゆる箇所でこの構造体を引数に取ります。
 */
typedef struct bnInterpreter {
        const char* filenameRef;
        GList* plugins;
        int argc;
        GPtrArray* argv;
        GHashTable* externTable;
        GHashTable* sharedTable;
        struct bnStringPool* pool;
        struct bnFrame* frame;
        struct bnHeap* heap;
        bnStack* callStack;
        bnJStack* __jstack;
#if DEBUG
        FILE* out;
        FILE* err;
        FILE* in;
#endif
} bnInterpreter;

/**
 * プラグインを初期化するための関数ポインタ型です。
 */
typedef void (*bnPluginInit)(bnInterpreter*);

/**
 * プラグインを終了するための関数ポインタ型です。
 */
typedef void (*bnPluginDestroy)(bnInterpreter*);

/**
 * プラグインから対応するboneバージョンを取得するための関数ポインタ型です。
 */
typedef const char* (*bnPluginGetTargetVersion)();

/**
 * 新しいbnInterpreterインスタンスを返します。
 * @param filenameRef
 * @param argc
 * @param argv
 * @return
 */
bnInterpreter* bnNewInterpreter(const char* filenameRef, int argc,
                                char* argv[]);

/**
 * 指定のパスの動的リンクライブラリをプラグインとして読み込みます。
 * @param bone
 * @param path
 */
void bnLink(bnInterpreter* bone, const char* path);

/**
 * インタプリタを実行します。
 * @param
 * @return
 */
int bnEval(bnInterpreter* self);

/**
 * 組み込み変数を書き込みます。
 * @param self
 * @param frame
 * @param pool
 */
void bnWriteBuiltin(bnInterpreter* self, struct bnFrame* frame,
                    struct bnStringPool* pool);

/**
 * 指定のメッセージでインタプリタをパニック状態にしてロングジャンプします。
 * @param self
 * @param fmt
 * @param ...
 */
void bnFormatThrow(bnInterpreter* self, const char* fmt, ...);

/**
 * 指定のメッセージでインタプリタをパニック状態にしてロングジャンプします。
 * @param self
 * @param fmt
 * @param ...
 */
void bnVFormatThrow(bnInterpreter* self, const char* fmt, va_list ap);

/**
 * 指定のオブジェクトでインタプリタをパニック状態にしてロングジャンプします。
 * @param self
 * @param exception
 * @param code
 */
void bnThrow(bnInterpreter* self, bnReference exception, int code);

/**
 * 指定のオブジェクトでインタプリタをパニック状態にします。
 * @param self
 * @param exception
 */
void bnPanic(bnInterpreter* self, bnReference exception);

/**
 * スタックから要素を一つ取り出して nameRef, nameObj として定義します。
 * 必ずネイティブ関数の先頭で展開してください。
 * @param bone
 * @param frame
 * @param name
 */
#define bnPopArg(bone, frame, name)                        \
        bnReference name##Ref = bnPopStack(frame->vStack); \
        bnObject* name##Obj = bnGetObject(bone->heap, name##Ref)

/**
 * ネイティブ関数の先頭で行われる定型的な宣言をマクロにラップしたものです。
 * スタックから要素を一つ取り出して nameRef, nameObj として定義します。
 * さらに、型を検査して必要ならパニックします。
 * 必ずネイティブ関数の先頭で展開してください。
 * @param bone
 * @param frame
 * @param name 引数名
 * @param expect 期待する型
 * @param optExt (必要なら)期待するbnAnyの名前
 */
#define bnPopGenericArg(bone, frame, name, expect, optExt) \
        bnPopArg(bone, frame, name);                       \
        name##Obj = bnTypeAssert(bone, #name, name##Obj, expect, optExt)

#define bnPopCharArg(bone, frame, name) \
        bnPopGenericArg(bone, frame, name, BN_OBJECT_CHAR, NULL)

#define bnPopStringArg(bone, frame, name) \
        bnPopGenericArg(bone, frame, name, BN_OBJECT_STRING, NULL)

#define bnPopArrayArg(bone, frame, name) \
        bnPopGenericArg(bone, frame, name, BN_OBJECT_ARRAY, NULL)

#define bnPopBoolArg(bone, frame, name) \
        bnPopGenericArg(bone, frame, name, BN_OBJECT_BOOL, NULL)

#define bnPopLambdaArg(bone, frame, name) \
        bnPopGenericArg(bone, frame, name, BN_OBJECT_LAMBDA, NULL)

#define bnPopIntArg(bone, frame, name) \
        bnPopGenericArg(bone, frame, name, BN_OBJECT_INTEGER, NULL)

#define bnPopDoubleArg(bone, frame, name) \
        bnPopGenericArg(bone, frame, name, BN_OBJECT_DOUBLE, NULL)

#define bnPopAnyArg(bone, frame, name, anyName) \
        bnPopGenericArg(bone, frame, name, BN_OBJECT_ANY, anyName)

#define bnTypeAssert(bone, paramName, obj, expect, optExtensionName)      \
        (bnTypeAssertFunc(bone, paramName, obj, expect, optExtensionName, \
                          __FILE__, __LINE__))
/**
 * 指定のオブジェクトのタイプを検証します。
 * タイプが正しくない場合はパニックしてロングジャンプします。
 * @param bone
 * @param paramName 引数名
 * @param obj 実引数
 * @param expect 期待する型
 * @param optExtensionName (必要なら)期待するbnAnyの名前
 * @param filename
 * @param lineno
 * @return
 */
struct bnObject* bnTypeAssertFunc(bnInterpreter* bone, const char* paramName,
                                  struct bnObject* obj,
                                  enum bnObjectType expect,
                                  const char* optExtensionName,
                                  const char* filename, int lineno);

/**
 * 変数テーブルから対応する真偽値型のインスタンスを返します。
 * @param pool
 * @param frame
 * @param cond
 * @return
 */
bnReference bnGetBool(struct bnStringPool* pool, struct bnFrame* frame,
                      bool cond);

/**
 * 変数テーブルから"true"を読み出して返します。
 * @param pool
 * @param frame
 * @return
 */
bnReference bnGetTrue(struct bnStringPool* pool, struct bnFrame* frame);

/**
 * 変数テーブルから"false"を読み出して返します。
 * @param pool
 * @param frame
 * @return
 */
bnReference bnGetFalse(struct bnStringPool* pool, struct bnFrame* frame);

/**
 * 指定の名前でエクスポートテーブルにオブジェクトを追加します。
 * @param self
 * @param name
 * @param ref
 */
void bnWriteExtern(bnInterpreter* self, bnStringView name, bnReference ref);

/**
 * 指定の名前でエクスポートテーブルにオブジェクトを追加します。
 * @param self
 * @param str
 * @param ref
 */
void bnWriteExtern2(bnInterpreter* self, const char* str, bnReference ref);

/**
 * 指定の名前でエクスポートテーブルからオブジェクトを読み出します。
 * @param self
 * @param name
 * @return
 */
bnReference bnReadExtern(bnInterpreter* self, bnStringView name);

/**
 * 指定の名前でエクスポートテーブルからオブジェクトを読み出します。
 * @param self
 * @param str
 * @return
 */
bnReference bnReadExtern2(bnInterpreter* self, const char* str);

/**
 * 指定の名前で共有されるオブジェクトがあるならtrueを返します。
 * @param self
 * @param name
 * @return
 */
bool bnIsShared(bnInterpreter* self, bnStringView name);

/**
 * 指定の名前で共有されるオブジェクトがあるならtrueを返します。
 * @param self
 * @param str
 * @return
 */
bool bnIsShared2(bnInterpreter* self, const char* str);

/**
 * 指定の名前でオブジェクトを共有します。
 * @param self
 * @param name
 * @param ref
 * @return
 */
bnReference bnAddShared(bnInterpreter* self, bnStringView name,
                        bnReference ref);

/**
 * 指定の名前でオブジェクトを共有します。
 * @param self
 * @param str
 * @param ref
 * @return
 */
bnReference bnAddShared2(bnInterpreter* self, const char* str, bnReference ref);

/**
 * 指定の名前の共有されたオブジェクトを返します。
 * @param self
 * @param name
 * @return
 */
bnReference bnGetShared(bnInterpreter* self, bnStringView name);

/**
 * 指定の名前の共有されたオブジェクトを返します。
 * @param self
 * @param str
 * @return
 */
bnReference bnGetShared2(bnInterpreter* self, const char* str);

/**
 * インタプリタに関連するあらゆる状態を解放します。
 * @param self
 */
void bnDeleteInterpreter(bnInterpreter* self);
#endif