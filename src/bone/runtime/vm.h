#ifndef BONE_RUNTIME_VM_H
#define BONE_RUNTIME_VM_H
#include "enviroment.h"
#include "frame.h"
#include "interpreter.h"
struct bnObject;
struct bnLambda;

/**
 * 入力オブジェクトのインジェクションバッファの値を出力フレームに書き込みます。
 * @param bone
 * @param src
 * @param dst
 */
void bnScopeInjection(bnInterpreter* bone, struct bnObject* src, bnFrame* dst);

/**
 * 入力オブジェクトのインジェクションバッファの値を出力オブジェクトに書き込みます。
 * @param bone
 * @param src
 * @param dst
 */
void bnObjectInjection(bnInterpreter* bone, struct bnObject* src,
                       struct bnObject* dst);

/**
 * 現在実行中の関数呼び出しの文字列表現を生成して返します。
 * @param bone
 * @parram env
 * @param lambda
 * @param pc
 */
GString* bnCreateStackFrameString(bnInterpreter* bone, bnEnviroment* env,
                                  struct bnObject* lambda, int PC);

/**
 * 実行中のバイトコードをそのまま入力としてラムダインスタンスを生成します。
 * @param bone
 * @param env
 * @param frame
 * @param pPC
 */
bnReference bnCreateLambdaInActiveCode(bnInterpreter* bone, bnEnviroment* env,
                                       struct bnFrame* frame, int* pPC);

/**
 * 指定のバイトコードを実行します。
 * @param bone
 * @param env
 * @param frame
 * @return 正常終了したなら0
 */
int bnExecute(bnInterpreter* bone, bnEnviroment* env, bnFrame* frame);
#endif