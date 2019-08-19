#ifndef BONE_PARSE_PARSE_AST_H
#define BONE_PARSE_PARSE_AST_H
#include <stdio.h>
#include "../bone.h"

/**
 * bnASTTagはASTの種類を表す列挙です。
 */
typedef enum bnASTTag {
        BN_AST_ROOT,
        BN_AST_BLANK,
        BN_AST_ARGUMENT,
        BN_AST_ARGUMENT_LIST,
        BN_AST_PARAMETER,
        BN_AST_PARAMETER_LIST,
        BN_AST_STATEMENT_LIST,
        BN_AST_LAMBDA,
        BN_AST_EXPRSTMT,
        BN_AST_IF,
        BN_AST_IF_ELSE,
        BN_AST_WHILE,
        BN_AST_INT_LIT,
        BN_AST_DOUBLE_LIT,
        BN_AST_STRING_LIT,
        BN_AST_CHAR_LIT,
        BN_AST_VARIABLE,
        BN_AST_MEMBER_ACCESS,
        BN_AST_FUNCCALL,
        BN_AST_IDENT,
        BN_AST_RETURN,
        BN_AST_CONTINUE,
        BN_AST_BREAK,
        BN_AST_SCOPE_INJECTION,
        BN_AST_OBJECT_INJECTION,
        BN_AST_ARRAY_SUBSCRIPT,
        BN_AST_ARRAY_LITERAL,
        BN_AST_DEFER,

        BN_AST_POSITIVE,
        BN_AST_NEGATIVE,
        BN_AST_CHILDA,
        BN_AST_NOT,

        BN_AST_PLUS,
        BN_AST_MINUS,
        BN_AST_MULTIPLY,
        BN_AST_DIVIDE,
        BN_AST_MODULO,

        BN_AST_BIT_AND,
        BN_AST_BIT_OR,
        BN_AST_LOGIC_AND,
        BN_AST_LOGIC_OR,
        BN_AST_EXC_OR,

        BN_AST_LSHIFT,
        BN_AST_RSHIFT,

        BN_AST_GT,
        BN_AST_GE,
        BN_AST_LT,
        BN_AST_LE,

        BN_AST_EQUAL,
        BN_AST_NOTEQUAL,

        BN_AST_ASSIGN,
        BN_AST_PLUS_ASSIGN,
        BN_AST_MINUS_ASSIGN,
        BN_AST_MULTIPLY_ASSIGN,
        BN_AST_DIVIDE_ASSIGN,
        BN_AST_MODULO_ASSIGN,

        BN_AST_AND_ASSIGN,
        BN_AST_OR_ASSIGN,
        BN_AST_EXC_OR_ASSIGN,
        BN_AST_LSHIFT_ASSIGN,
        BN_AST_RSHIFT_ASSIGN

} bnASTTag;

/**
 * bnASTはプログラムの構造を表す要素です。
 */
typedef struct bnAST {
        union {
                int ivalue;
                double dvalue;
                char cvalue;
                bnStringView svvalue;
        } u;
        GPtrArray* children;
        bnASTTag tag;
        int line;
} bnAST;

/**
 * 構文解析に失敗したために、中途半端に構築途中のASTが残った場合はこれを呼び出します。
 * @param error
 */
void bnCleanAST(bool error);

/**
 * 指定のタグで新しいbnASTインスタンスを生成して返します。
 * @param tag
 * @return
 */
bnAST* bnNewAST(bnASTTag tag);

/**
 * リストが空であることを表す新しいbnASTインスタンスを生成して返します。
 * @return
 */
bnAST* bnNewBlankAST();

/**
 * 遅延実行ブロックを表す新しいbnASTインスタンスを生成して返します。
 * @param astmt
 * @return
 */
bnAST* bnNewDeferAST(bnAST* astmt);

/**
 * 配列リテラルを表す新しいbnASTインスタンスを生成して返します。
 * @param aargs
 * @return
 */
bnAST* bnNewArrayLit(bnAST* aargs);

/**
 * 配列参照を表す新しいbnASTインスタンスを生成して返します。
 * @param aexpr
 * @param aindex
 * @return
 */
bnAST* bnNewArraySubscriptAST(bnAST* aexpr, bnAST* aindex);

/**
 * 可変長戻り値を表す新しいbnASTインスタンスを生成して返します。
 * @return
 */
bnAST* bnNewDotDotDotAST();

/**
 * スコープインジェクションを表す新しいbnASTインスタンスを生成して返します。
 * @param aexpr
 * @return
 */
bnAST* bnNewScopeInjectionAST(bnAST* aexpr);

/**
 * オブジェクトインジェクションを表す新しいbnASTインスタンスを生成して返します。
 * @param aleft
 * @param aright
 * @return
 */
bnAST* bnNewObjectInjectionAST(bnAST* aleft, bnAST* aright);

/**
 * return文を表す新しいbnASTインスタンスを生成して返します。
 * @param expr
 * @return
 */
bnAST* bnNewReturnAST(bnAST* expr);

/**
 * 式だけで構成される文を表す新しいbnASTインスタンスを生成して返します。
 * @param aexpr
 * @return
 */
bnAST* bnNewExprStmtAST(bnAST* aexpr);

/**
 * if文を表す新しいbnASTインスタンスを生成して返します。
 * @param aexpr
 * @param astmt
 * @return
 */
bnAST* bnNewIfAST(bnAST* aexpr, bnAST* astmt);

/**
 * if-else文を表す新しいbnASTインスタンスを生成して返します。
 * @param aif
 * @param astmt
 * @return
 */
bnAST* bnNewIfElseAST(bnAST* aif, bnAST* astmt);

/**
 * while文を表す新しいbnASTインスタンスを生成して返します。
 * @param aexpr
 * @param astmt
 * @return
 */
bnAST* bnNewWhileAST(bnAST* aexpr, bnAST* astmt);

/**
 * 実引数を表す新しいbnASTインスタンスを生成して返します。
 * @param aexpr
 * @return
 */
bnAST* bnNewArgumentAST(bnAST* aexpr);

/**
 * 実引数リストを表す新しいbnASTインスタンスを生成して返します。
 * @param aexpr
 * @param aargs
 * @return
 */
bnAST* bnNewArgumentListAST(bnAST* aexpr, bnAST* aargs);

/**
 * 仮引数を表す新しいbnASTインスタンスを生成して返します。
 * @param name
 * @return
 */
bnAST* bnNewParameterAST(bnStringView name);

/**
 * 仮引数リストを表す新しいbnASTインスタンスを生成して返します。
 * @param aparam
 * @param aparams
 * @return
 */
bnAST* bnNewParameterListAST(bnAST* aparam, bnAST* aparams);

/**
 * ラムダ式を表す新しいbnASTインスタンスを生成して返します。
 * @param aparams
 * @param areturn
 * @param astmt
 * @return
 */
bnAST* bnNewLambdaAST(bnAST* aparams, bnAST* areturn, bnAST* astmt);

/**
 * 文のリストを表す新しいbnASTインスタンスを生成して返します。
 * @param astmt
 * @param astmts
 * @return
 */
bnAST* bnNewStatementListAST(bnAST* astmt, bnAST* astmtList);

/**
 * 変数を表す新しいbnASTインスタンスを生成して返します。
 * @param name
 * @return
 */
bnAST* bnNewVariableAST(bnStringView name);

/**
 * メンバアクセスを表す新しいbnASTインスタンスを生成して返します。
 * @param aexpr
 * @param name
 * @return
 */
bnAST* bnNewMemberAccessAST(bnAST* aexpr, bnStringView name);

/**
 * 関数呼び出しを表す新しいbnASTインスタンスを生成して返します。
 * @param aexpr
 * @param aargs
 * @return
 */
bnAST* bnNewFuncCall(bnAST* aexpr, bnAST* aargs);

/**
 * 整数リテラルを表す新しいbnASTインスタンスを生成して返します。
 * @param ivalue
 * @return
 */
bnAST* bnNewIntAST(int ivalue);

/**
 * 文字列リテラルを表す新しいbnASTインスタンスを生成して返します。
 * @param value
 * @return
 */
bnAST* bnNewStringAST(bnStringView value);

/**
 * 文字リテラルを表す新しいbnASTインスタンスを生成して返します。
 * @param cvalue
 * @return
 */
bnAST* bnNewCharAST(char cvalue);

/**
 * 実数リテラルを表す新しいbnASTインスタンスを生成して返します。
 * @param value
 * @return
 */
bnAST* bnNewDoubleAST(double dvalue);

/**
 * 単項演算子を表す新しいbnASTインスタンスを生成して返します。
 * @param tag
 * @param a
 * @return
 */
bnAST* bnNewUnaryAST(bnASTTag tag, bnAST* a);

/**
 * 二項演算氏を表す新しいbnASTインスタンスを生成して返します。
 * @param tag
 * @param left
 * @param right
 * @return
 */
bnAST* bnNewBinaryAST(bnASTTag tag, bnAST* left, bnAST* right);

/**
 * selfにaを子要素として追加します。
 * @param self
 * @param a
 */
void bnPushAST(bnAST* self, bnAST* a);

/**
 * ASTをダンプします。
 * @param fp
 * @param pool
 * @param self
 */
void bnDumpAST(FILE* fp, struct bnStringPool* pool, bnAST* self);

/**
 * ASTを一段階だけ出力します。
 * @param fp
 * @param pool
 * @param self
 */
void bnPrintAST(FILE* fp, struct bnStringPool* pool, bnAST* self);

/**
 * ASTを再帰的に解放します。
 * @param self
 */
void bnDeleteAST(bnAST* self);

/**
 * 0番目の子要素を返します。
 * @param self
 * @return
 */
bnAST* bnFirstAST(bnAST* self);

/**
 * 1番目の子要素を返します。
 * @param self
 * @return
 */
bnAST* bnSecondAST(bnAST* self);

/**
 * 2番目の子要素を返します。
 * @param self
 * @return
 */
bnAST* bnThirdAST(bnAST* self);

/**
 * ASTの値を計算して返します。
 * @param self
 * @return
 */
double bnEvalAST(bnAST* self);
#endif