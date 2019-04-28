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

ast* ast_new(bnASTTag tag);

ast* ast_new_int(int ivalue);

ast* ast_new_double(double dvalue);

ast* ast_new_unary(bnASTTag tag, ast* a);

ast* ast_new_binary(bnASTTag tag, ast* left, ast* right);

void ast_push(ast* self, ast* a);

void ast_dump(ast* self);

void ast_print(ast* self);

void ast_delete(ast* self);

ast* ast_first(ast* self);

ast* ast_second(ast* self);

double ast_eval(ast* self);
#endif