#ifndef BONE_RUNTIME_ENVIROMENT_H
#define BONE_RUNTIME_ENVIROMENT_H
#include "../bone.h"
#include "../util/stack.h"
#include "label.h"
#include "opcode.h"

struct bnLambda;

/**
 * bnEnviromentはバイトコード、ジャンプテーブルなどのバイトコード生成が
 * 完了してから不変なデータを保存する構造体です。
 */
typedef struct bnEnviroment {
        bnStringView filename;
        GPtrArray* codeArray;
        GPtrArray* labels;
        GPtrArray* ranges;
        GArray* doubleConstants;
        bnStack* labelFixStack;
        int lineOffset;
} bnEnviroment;

/**
 * 新しいbnEnviromentインスタンスを生成して返します。
 * @param filename
 * @return
 */
bnEnviroment* bnNewEnviroment(bnStringView filename);

/**
 * 末尾にオペコードを一つ書き出します。
 * @param self
 * @param code
 */
void bnWriteCode(bnEnviroment* self, int code);
/**
 * 指定位置からオペコードをひとつ読み出します。
 * @param self
 * @param pc
 * @return
 */
int bnReadCode(bnEnviroment* self, int pc);

/**
 * 指定位置からラベルをひとつ読み出します。
 * @param self
 * @param pc
 * @return
 */
bnLabel* bnReadLabel(bnEnviroment* self, int pc);

/**
 * 末尾へラベルを書き出します。
 * ラベルはポインタをそのまま保存します。
 * これはif文などのコード生成で後方へジャンプする必要が生じるためです。
 * @param self
 * @param label
 */
void bnWriteLabel(bnEnviroment* self, bnLabel* label);

/**
 * 行範囲を追加します。オペコードから行番号を逆算するために、
 * まとまったコード生成が終わるたびに呼び出します。
 * @param self
 * @param lineno
 */
void bnAddLineRange(bnEnviroment* self, int lineno);

/**
 * オペコードから行番号を逆算します。
 * @param self
 * @param pc
 * @return
 */
int bnFindLineRange(bnEnviroment* self, int pc);

/**
 * 指定の位置を指すラベルを生成して書き出します。
 * @param self
 * @param pos
 * @return
 */
bnLabel* bnAutoNewLabel(bnEnviroment* self, int pos);

/**
 * ラムダによるオフセットを加算した上でラベルを書き出します。
 * @param self
 * @param pos
 * @return
 */
bnLabel* bnGenerateLabel(bnEnviroment* self, int pos);

/**
 * 末尾にNOP(No OPeration)を書き出して位置を返します。
 * ジャンプ先として便利です。
 * @param self
 * @return
 */
int bnGenerateNOP(bnEnviroment* self);

/**
 * 指定の数だけNOP(No OPeration)を書き出します。
 * @param self
 * @param count
 */
void bnGenerateFillNOP(bnEnviroment* self, int count);

/**
 * ラムダ式の内部のコード生成において、
 * 正しいラベル位置を計算するためにラムダ式のコード生成の最初で呼び出します。
 * @param self
 */
void bnGenerateEnterLambda(bnEnviroment* self);

/**
 * ラムダ式の内部のコード生成において、
 * 正しいラベル位置を計算するためにラムダ式のコード生成の最後で呼び出します。
 * @param self
 */
void bnGenerateExitLambda(bnEnviroment* self);

/**
 * 最後のラムダ式からのオフセットを返します。
 * @param self
 * @return
 */
int bnGetLambdaOffset(bnEnviroment* self);

/**
 * 全てのオペコードやジャンプテーブルを解放します。
 * @param self
 */
void bnDeleteEnviroment(bnEnviroment* self);
#endif