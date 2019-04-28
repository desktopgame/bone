#ifndef BONE_PARSE_PARSE_AST_H
#define BONE_PARSE_PARSE_AST_H
#include "../bone.h"

typedef enum bnASTTag {
        BN_AST_ROOT,
        BN_AST_LIT,
        BN_AST_DOUBLE_LIT,
        BN_AST_IDENT,

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

typedef struct ast {
        union {
                int ivalue;
                double dvalue;
                char* svalue;
        } u;
        GList* children;
        bnASTTag tag;
} ast;

ast* bnNewAST(bnASTTag tag);

ast* bnNewIntAST(int ivalue);

ast* bnNewDoubleAST(double dvalue);

ast* bnNewUnaryAST(bnASTTag tag, ast* a);

ast* bnNewBinaryAST(bnASTTag tag, ast* left, ast* right);

void bnPushAST(ast* self, ast* a);

void bnDumpAST(ast* self);

void bnPrintAST(ast* self);

void bnDeleteAST(ast* self);

ast* bnFirstAST(ast* self);

ast* bnSecondAST(ast* self);

double bnEvalAST(ast* self);
#endif