#ifndef BONE_PARSE_PARSER_H
#define BONE_PARSE_PARSER_H
#include "../util/string_pool.h"
#include "ast.h"

/**
 * bnParserInputTagはパーサの入力元を表す列挙です。
 */
typedef enum bnParserInputTag {
        BN_PARSER_INPUT_FROM_SOURCE,
        BN_PARSER_INPUT_FROM_FILE
} bnParserInputTag;

/**
 * ファイルを入力元として構文解析を実行し、bnASTを返します。
 * @param pool
 * @param filename
 * @return
 */
bnAST* bnParseFile(struct bnStringPool* pool, const char* filename);

/**
 * 文字列を入力元として構文解析を実行し、bnASTを返します。
 * @param pool
 * @param source
 * @return
 */
bnAST* bnParseString(struct bnStringPool* pool, const char* source);

/**
 * パーサのタグを返します。
 * @return
 */
bnParserInputTag bnGetParserInputTag();

/**
 * 現在バッファ中の文字列をインターンします。
 * @param str
 * @return
 */
bnStringView bnInternIdentifier(const char* str);

/**
 * 文字列バッファを初期化します。
 */
void bnBeginStringLit();

/**
 * 文字列バッファに追記します。
 * @param c
 */
void bnAppendStringLit(char c);

/**
 * 文字列バッファから文字列リテラルを表すbnASTを生成して返します。
 * @return
 */
bnAST* bnEndStringLit();

/**
 * 解析中の行を設定します。
 * @param line
 */
void bnSetParseLine(int line);

/**
 * 解析中の行を返します。
 * @return
 */
int bnGetParseLine();

/**
 * 解析中の列を設定します。
 * @param column
 */
void bnSetParseColumn(int column);

/**
 * 解析中の列を返します。
 * @return
 */
int bnGetParseColumn();

/**
 * 指定の行をスタックへプッシュします。
 * @param lineno
 */
void bnPushParseLine(int lineno);

/**
 * 最後にプッシュされた行を返します。
 * @return
 */
int bnPopParseLine();
#endif