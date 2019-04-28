#include "ast.h"
#include <assert.h>
#include <stdio.h>

// proto
static void ast_child_delete(gpointer item);
static void ast_dumpImpl(ast* self, int depth);

ast* ast_new(bnASTTag tag) {
        ast* ret = (ast*)malloc(sizeof(ast));
        ret->tag = tag;
        ret->children = NULL;
        return ret;
}

ast* ast_new_int(int ivalue) {
        ast* ret = ast_new(BN_AST_LIT);
        ret->u.ivalue = ivalue;
        return ret;
}

ast* ast_new_double(double dvalue) {
        ast* ret = ast_new(BN_AST_DOUBLE_LIT);
        ret->u.dvalue = dvalue;
        return ret;
}

ast* ast_new_unary(bnASTTag tag, ast* a) {
        ast* ret = ast_new(tag);
        ast_push(ret, a);
        return ret;
}

ast* ast_new_binary(bnASTTag tag, ast* left, ast* right) {
        ast* ret = ast_new(tag);
        ast_push(ret, left);
        ast_push(ret, right);
        return ret;
}

void ast_push(ast* self, ast* a) {
        assert(self != NULL && a != NULL);
        self->children = g_list_append(self->children, a);
}

void ast_dump(ast* self) { ast_dumpImpl(self, 0); }

void ast_print(ast* self) {
#define p(a)         \
        printf((a)); \
        break
        switch (self->tag) {
                case BN_AST_ROOT:
                        p("root");
                case BN_AST_LIT:
                        printf("%d", self->u.ivalue);
                        break;
                case BN_AST_DOUBLE_LIT:
                        printf("%lf", self->u.dvalue);
                        break;
                case BN_AST_PLUS:
                        p("+");
                case BN_AST_POSITIVE:
                        p("+");
                case BN_AST_MINUS:
                        p("-");
                case BN_AST_NEGATIVE:
                        p("-");
                case BN_AST_MULTIPLY:
                        p("*");
                case BN_AST_DIVIDE:
                        p("/");
                case BN_AST_MODULO:
                        p("%%");

                case BN_AST_BIT_OR:
                        p("|");
                case BN_AST_BIT_AND:
                        p("&");

                case BN_AST_LOGIC_OR:
                        p("||");
                case BN_AST_LOGIC_AND:
                        p("&&");

                case BN_AST_LSHIFT:
                        p("<<");
                case BN_AST_RSHIFT:
                        p(">>");

                case BN_AST_EQUAL:
                        p("==");
                case BN_AST_NOTEQUAL:
                        p("!=");

                case BN_AST_GT:
                        p(">");
                case BN_AST_GE:
                        p(">=");
                case BN_AST_LT:
                        p("<");
                case BN_AST_LE:
                        p("<=");

                case BN_AST_EXC_OR:
                        p("^");
                case BN_AST_NOT:
                        p("!");

                case BN_AST_ASSIGN:
                        p("=");
                case BN_AST_PLUS_ASSIGN:
                        p("+=");
                case BN_AST_MINUS_ASSIGN:
                        p("-=");
                case BN_AST_MULTIPLY_ASSIGN:
                        p("*=");
                case BN_AST_DIVIDE_ASSIGN:
                        p("/=");
                case BN_AST_MODULO_ASSIGN:
                        p("%%=");
                default:
                        printf("undefined");
                        break;
        }
#undef p
}

void ast_delete(ast* self) {
        g_list_free_full(self->children, ast_child_delete);
        free(self);
}

ast* ast_first(ast* self) { return self->children->data; }

ast* ast_second(ast* self) { return self->children->next->data; }

double ast_eval(ast* self) {
        if (self->tag == BN_AST_LIT) {
                return (double)self->u.ivalue;
        }
        if (self->tag == BN_AST_DOUBLE_LIT) {
                return self->u.dvalue;
        }
        switch (self->tag) {
                case BN_AST_PLUS:
                        return (ast_eval(ast_first(self)) +
                                ast_eval(ast_second(self)));
                case BN_AST_POSITIVE:
                        return +(ast_eval(ast_first(self)));
                case BN_AST_MINUS:
                        return (ast_eval(ast_first(self)) -
                                ast_eval(ast_second(self)));
                case BN_AST_NEGATIVE:
                        return -(ast_eval(ast_first(self)));
                case BN_AST_MULTIPLY:
                        return (ast_eval(ast_first(self)) *
                                ast_eval(ast_second(self)));
                case BN_AST_DIVIDE:
                        return (ast_eval(ast_first(self)) /
                                ast_eval(ast_second(self)));
                case BN_AST_MODULO:
                        return ((int)ast_eval(ast_first(self)) %
                                (int)ast_eval(ast_second(self)));

                case BN_AST_BIT_OR:
                        return (int)ast_eval(ast_first(self)) |
                               (int)ast_eval(ast_second(self));
                case BN_AST_BIT_AND:
                        return (int)ast_eval(ast_first(self)) &
                               (int)ast_eval(ast_second(self));
                case BN_AST_LOGIC_OR:
                        return (int)ast_eval(ast_first(self)) ||
                               (int)ast_eval(ast_second(self));
                case BN_AST_LOGIC_AND:
                        return (int)ast_eval(ast_first(self)) &&
                               (int)ast_eval(ast_second(self));
                case BN_AST_EXC_OR:
                        return (int)ast_eval(ast_first(self)) ^
                               (int)ast_eval(ast_second(self));

                case BN_AST_LSHIFT:
                        return (int)ast_eval(ast_first(self))
                               << (int)ast_eval(ast_second(self));
                case BN_AST_RSHIFT:
                        return (int)ast_eval(ast_first(self)) >>
                               (int)ast_eval(ast_second(self));

                case BN_AST_GT:
                        return ((int)ast_eval(ast_first(self)) >
                                (int)ast_eval(ast_second(self)));
                case BN_AST_GE:
                        return ((int)ast_eval(ast_first(self)) >=
                                (int)ast_eval(ast_second(self)));
                case BN_AST_LT:
                        return ((int)ast_eval(ast_first(self)) <
                                (int)ast_eval(ast_second(self)));
                case BN_AST_LE:
                        return ((int)ast_eval(ast_first(self)) <=
                                (int)ast_eval(ast_second(self)));

                case BN_AST_CHILDA:
                        return ~(int)(ast_eval(ast_first(self)));
                case BN_AST_NOT:
                        return !(int)(ast_eval(ast_first(self)));

                case BN_AST_EQUAL:
                        return ((int)ast_eval(ast_first(self)) ==
                                (int)ast_eval(ast_second(self)));
                case BN_AST_NOTEQUAL:
                        return ((int)ast_eval(ast_first(self)) !=
                                (int)ast_eval(ast_second(self)));

                case BN_AST_ASSIGN:
                case BN_AST_PLUS_ASSIGN:
                case BN_AST_MINUS_ASSIGN:
                case BN_AST_MULTIPLY_ASSIGN:
                case BN_AST_DIVIDE_ASSIGN:
                case BN_AST_MODULO_ASSIGN:
                        return ast_eval(ast_second(self));
                default:
                        return 0;
        }
        return 0.0;
}

// private
static void ast_child_delete(gpointer item) {
        ast* e = (ast*)item;
        ast_delete(e);
}

static void ast_dumpImpl(ast* self, int depth) {
        for (int i = 0; i < depth; i++) {
                printf(" ");
        }
        ast_print(self);
        printf("\n");
        GList* iter = self->children;
        while (iter != NULL) {
                ast* e = iter->data;
                ast_dumpImpl(e, depth + 1);
                iter = iter->next;
        }
}