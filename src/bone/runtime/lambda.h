#ifndef BONE_RUNTIME_LAMBDA_H
#define BONE_RUNTIME_LAMBDA_H
#include <stdarg.h>
#include <stdbool.h>
#include "enviroment.h"
#include "object.h"

struct bnFrame;
struct bnInterpreter;
typedef void (*bnNativeFunc)(struct bnInterpreter* bone, struct bnFrame* frame);

#define BN_C_ADD_PARAM (0)
#define BN_C_ADD_RETURN (1)
#define BN_C_ADD_EXIT (2)

/**
 * bnLambdaTypeはラムダの種類を表します。
 */
typedef enum bnLambdaType {
        BN_LAMBDA_SCRIPT,
        BN_LAMBDA_NATIVE,
} bnLambdaType;

#define bnNewLambda(bone, type) \
        (bnNewLambdaFunc(bone, type, __FILE__, __LINE__))
/**
 * 新しいラムダインスタンスを生成して、参照を返します。
 * @param bone
 * @param type
 * @param filename
 * @param lineno
 * @return
 */
bnReference bnNewLambdaFunc(struct bnInterpreter* bone, bnLambdaType type,
                            const char* filename, int lineno);

#define bnNewLambdaFromCFunc(bone, func, pool, ...)                     \
        (bnNewLambdaFromCFuncFunc(bone, func, pool, __FILE__, __LINE__, \
                                  __VA_ARGS__))
/**
 * 新しいラムダインスタンスを生成して、参照を返します。
 * @param bone
 * @param func
 * @param pool
 * @param filename
 * @param lineno
 * @param ... (parameter, named returns)
 * @return
 */
bnReference bnNewLambdaFromCFuncFunc(struct bnInterpreter* bone,
                                     bnNativeFunc func,
                                     struct bnStringPool* pool,
                                     const char* filename, int lineno, ...);

/**
 * 指定のラムダインスタンスがインスタンスベースのラムダであるなら true
 * を返します。 インスタンスベースのラムダの条件は、引数の最初の名前が self
 * であることです。
 * これを満たすラムダインスタンスがあるオブジェクトのフィールドとして格納されると、
 * 通常のメソッドのように呼び出せるようになります。
 * @param pool
 * @param self
 * @return
 */
bool bnIsInstanceBaseLambda(struct bnStringPool* pool, bnObject* self);

/**
 * 戻り値が可変長ならtrueを返します。
 * @param pool
 * @param self
 * @return
 */
bool bnIsVariadicReturn(struct bnStringPool* pool, bnObject* self);

/**
 * ラムダインスタンスのバイトコードを上書きします。
 * @param obj
 * @param env
 */
void bnSetEnviroment(bnObject* obj, bnEnviroment* env);

/**
 * ラムダインスタンスのバイトコードを返します。
 * @param obj
 * @return
 */
bnEnviroment* bnGetEnviroment(bnObject* obj);

/**
 * 指定のラムダインスタンスが、スクリプトではなくネイティブメソッドとして実装されているなら、
 * C関数へのポインタを返します。
 * @param obj
 * @return
 */
bnNativeFunc bnGetNativeFunc(bnObject* obj);

/**
 * 指定のラムダインスタンスに引数を追加します。
 * @param obj
 * @param param
 */
void bnAddParameter(bnObject* obj, gpointer param);

/**
 * 指定のラムダインスタンスの引数の一覧を返します。
 * @param obj
 * @return
 */
GList* bnGetParameterList(bnObject* obj);

/**
 * 指定のラムダインスタンスに戻り値を追加します。
 * @param obj
 * @param retval
 */
void bnAddReturnValue(bnObject* obj, gpointer retval);

/**
 * 指定のラムダインスタンスの戻り値の一覧を返します。
 * @param obj
 * @return
 */
GList* bnGetReturnValueList(bnObject* obj);

/**
 * 指定のラムダインスタンスがキャプチャしている変数テーブルを返します。
 * @param obj
 * @return
 */
GHashTable* bnGetCapturedMap(bnObject* obj);

/**
 * ラムダインスタンスの定義されたファイル名を上書きします。
 * エラー情報のために使用されます。
 * @param obj
 * @param filename
 */
void bnSetLambdaFileName(bnObject* obj, bnStringView filename);

/**
 * ラムダインスタンスの定義されたファイル名を返します。
 * エラー情報のために使用されます。
 * @param obj
 * @return
 */
bnStringView bnGetLambdaFileName(bnObject* obj);

/**
 * ラムダインスタンスの定義された行番号を上書きします。
 * エラー情報のために使用されます。
 * @param obj
 * @param line
 */
void bnSetLambdaLineNumber(bnObject* obj, int line);

/**
 * ラムダインスタンスの定義された行番号を返します。
 * エラー情報のために使用されます。
 * @param obj
 * @return
 */
int bnGetLambdaLineNumber(bnObject* obj);

/**
 * 指定のラムダインスタンスの種類を返します。
 * @param obj
 * @return
 */
bnLambdaType bnGetLambdaType(bnObject* obj);
#endif