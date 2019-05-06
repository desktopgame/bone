#ifndef BONE_PARSE_PARSE_AST_H
#define BONE_PARSE_PARSE_AST_H
#include <stdio.h>
#include "../bone.h"

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
        BN_AST_SCOPE_INJECTION,
        BN_AST_OBJECT_INJECTION,
        BN_AST_ARRAY_SUBSCRIPT,
        BN_AST_PANIC,

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

typedef struct bnAST {
        union {
                int ivalue;
                double dvalue;
                char cvalue;
                bnStringView svvalue;
        } u;
        GList* children;
        bnASTTag tag;
        int line;
} bnAST;

bnAST* bnNewAST(bnASTTag tag);

bnAST* bnNewBlankAST();

bnAST* bnNewPanicAST(bnStringView name, bnAST* aexpr);

bnAST* bnNewArraySubscriptAST(bnAST* aexpr, bnAST* aindex);

bnAST* bnNewDotDotDotAST();

bnAST* bnNewScopeInjectionAST(bnAST* aexpr);

bnAST* bnNewObjectInjectionAST(bnAST* aleft, bnAST* aright);

bnAST* bnNewReturnAST(bnAST* expr);

bnAST* bnNewExprStmtAST(bnAST* aexpr);

bnAST* bnNewIfAST(bnAST* aexpr, bnAST* astmt);

bnAST* bnNewIfElseAST(bnAST* aif, bnAST* astmt);

bnAST* bnNewWhileAST(bnAST* aexpr, bnAST* astmt);

bnAST* bnNewArgumentAST(bnAST* aexpr);

bnAST* bnNewArgumentListAST(bnAST* aexpr, bnAST* aargs);

bnAST* bnNewParameterAST(bnStringView name);

bnAST* bnNewParameterListAST(bnAST* aparam, bnAST* aparams);

bnAST* bnNewLambdaAST(bnAST* aparams, bnAST* areturn, bnAST* astmt);

bnAST* bnNewStatementListAST(bnAST* astmt, bnAST* astmtList);

bnAST* bnNewVariableAST(bnStringView name);

bnAST* bnNewMemberAccessAST(bnAST* aexpr, bnStringView name);

bnAST* bnNewFuncCall(bnAST* aexpr, bnAST* aargs);

bnAST* bnNewIntAST(int ivalue);

bnAST* bnNewStringAST(bnStringView value);

bnAST* bnNewCharAST(char cvalue);

bnAST* bnNewDoubleAST(double dvalue);

bnAST* bnNewUnaryAST(bnASTTag tag, bnAST* a);

bnAST* bnNewBinaryAST(bnASTTag tag, bnAST* left, bnAST* right);

void bnPushAST(bnAST* self, bnAST* a);

void bnDumpAST(FILE* fp, struct bnStringPool* pool, bnAST* self);

void bnPrintAST(FILE* fp, struct bnStringPool* pool, bnAST* self);

void bnDeleteAST(bnAST* self);

bnAST* bnFirstAST(bnAST* self);

bnAST* bnSecondAST(bnAST* self);

bnAST* bnThirdAST(bnAST* self);

double bnEvalAST(bnAST* self);
#endif