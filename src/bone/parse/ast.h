#ifndef BONE_PARSE_PARSE_AST_H
#define BONE_PARSE_PARSE_AST_H
#include "../bone.h"

typedef enum ast_tag {
        ast_root,
        ast_int,
        ast_double,
        ast_ident,

        ast_pos,
        ast_neg,
        ast_childa,
        ast_not,

        ast_add,
        ast_sub,
        ast_mul,
        ast_div,
        ast_mod,

        ast_bit_and,
        ast_bit_or,
        ast_logic_and,
        ast_logic_or,
        ast_exc_or,

        ast_lshift,
        ast_rshift,

        ast_gt,
        ast_ge,
        ast_lt,
        ast_le,

        ast_equal,
        ast_notequal,

        ast_assign,
        ast_add_assign,
        ast_sub_assign,
        ast_mul_assign,
        ast_div_assign,
        ast_mod_assign,

        ast_and_assign,
        ast_or_assign,
        ast_exc_or_assign,
        ast_lshift_assign,
        ast_rshift_assign

} ast_tag;

typedef struct ast {
        union {
                int ivalue;
                double dvalue;
                char* svalue;
        } u;
        GList* children;
        ast_tag tag;
} ast;

ast* ast_new(ast_tag tag);

ast* ast_new_int(int ivalue);

ast* ast_new_double(double dvalue);

ast* ast_new_unary(ast_tag tag, ast* a);

ast* ast_new_binary(ast_tag tag, ast* left, ast* right);

void ast_push(ast* self, ast* a);

void ast_dump(ast* self);

void ast_print(ast* self);

void ast_delete(ast* self);

ast* ast_first(ast* self);

ast* ast_second(ast* self);

double ast_eval(ast* self);
#endif