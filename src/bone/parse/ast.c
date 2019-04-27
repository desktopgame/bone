#include "ast.h"
#include <assert.h>
#include <stdio.h>

// proto
static void ast_child_delete(gpointer item);
static void ast_dumpImpl(ast* self, int depth);

ast* ast_new(ast_tag tag) {
        ast* ret = (ast*)malloc(sizeof(ast));
        ret->tag = tag;
        ret->children = NULL;
        return ret;
}

ast* ast_new_int(int ivalue) {
        ast* ret = ast_new(ast_int);
        ret->u.ivalue = ivalue;
        return ret;
}

ast* ast_new_double(double dvalue) {
        ast* ret = ast_new(ast_double);
        ret->u.dvalue = dvalue;
        return ret;
}

ast* ast_new_unary(ast_tag tag, ast* a) {
        ast* ret = ast_new(tag);
        ast_push(ret, a);
        return ret;
}

ast* ast_new_binary(ast_tag tag, ast* left, ast* right) {
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
                case ast_root:
                        p("root");
                case ast_int:
                        printf("%d", self->u.ivalue);
                        break;
                case ast_double:
                        printf("%lf", self->u.dvalue);
                        break;
                case ast_add:
                        p("+");
                case ast_pos:
                        p("+");
                case ast_sub:
                        p("-");
                case ast_neg:
                        p("-");
                case ast_mul:
                        p("*");
                case ast_div:
                        p("/");
                case ast_mod:
                        p("%%");

                case ast_bit_or:
                        p("|");
                case ast_bit_and:
                        p("&");

                case ast_logic_or:
                        p("||");
                case ast_logic_and:
                        p("&&");

                case ast_lshift:
                        p("<<");
                case ast_rshift:
                        p(">>");

                case ast_equal:
                        p("==");
                case ast_notequal:
                        p("!=");

                case ast_gt:
                        p(">");
                case ast_ge:
                        p(">=");
                case ast_lt:
                        p("<");
                case ast_le:
                        p("<=");

                case ast_exc_or:
                        p("^");
                case ast_not:
                        p("!");

                case ast_assign:
                        p("=");
                case ast_add_assign:
                        p("+=");
                case ast_sub_assign:
                        p("-=");
                case ast_mul_assign:
                        p("*=");
                case ast_div_assign:
                        p("/=");
                case ast_mod_assign:
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
        if (self->tag == ast_int) {
                return (double)self->u.ivalue;
        }
        if (self->tag == ast_double) {
                return self->u.dvalue;
        }
        switch (self->tag) {
                case ast_add:
                        return (ast_eval(ast_first(self)) +
                                ast_eval(ast_second(self)));
                case ast_pos:
                        return +(ast_eval(ast_first(self)));
                case ast_sub:
                        return (ast_eval(ast_first(self)) -
                                ast_eval(ast_second(self)));
                case ast_neg:
                        return -(ast_eval(ast_first(self)));
                case ast_mul:
                        return (ast_eval(ast_first(self)) *
                                ast_eval(ast_second(self)));
                case ast_div:
                        return (ast_eval(ast_first(self)) /
                                ast_eval(ast_second(self)));
                case ast_mod:
                        return ((int)ast_eval(ast_first(self)) %
                                (int)ast_eval(ast_second(self)));

                case ast_bit_or:
                        return (int)ast_eval(ast_first(self)) |
                               (int)ast_eval(ast_second(self));
                case ast_bit_and:
                        return (int)ast_eval(ast_first(self)) &
                               (int)ast_eval(ast_second(self));
                case ast_logic_or:
                        return (int)ast_eval(ast_first(self)) ||
                               (int)ast_eval(ast_second(self));
                case ast_logic_and:
                        return (int)ast_eval(ast_first(self)) &&
                               (int)ast_eval(ast_second(self));
                case ast_exc_or:
                        return (int)ast_eval(ast_first(self)) ^
                               (int)ast_eval(ast_second(self));

                case ast_lshift:
                        return (int)ast_eval(ast_first(self))
                               << (int)ast_eval(ast_second(self));
                case ast_rshift:
                        return (int)ast_eval(ast_first(self)) >>
                               (int)ast_eval(ast_second(self));

                case ast_gt:
                        return ((int)ast_eval(ast_first(self)) >
                                (int)ast_eval(ast_second(self)));
                case ast_ge:
                        return ((int)ast_eval(ast_first(self)) >=
                                (int)ast_eval(ast_second(self)));
                case ast_lt:
                        return ((int)ast_eval(ast_first(self)) <
                                (int)ast_eval(ast_second(self)));
                case ast_le:
                        return ((int)ast_eval(ast_first(self)) <=
                                (int)ast_eval(ast_second(self)));

                case ast_childa:
                        return ~(int)(ast_eval(ast_first(self)));
                case ast_not:
                        return !(int)(ast_eval(ast_first(self)));

                case ast_equal:
                        return ((int)ast_eval(ast_first(self)) ==
                                (int)ast_eval(ast_second(self)));
                case ast_notequal:
                        return ((int)ast_eval(ast_first(self)) !=
                                (int)ast_eval(ast_second(self)));

                case ast_assign:
                case ast_add_assign:
                case ast_sub_assign:
                case ast_mul_assign:
                case ast_div_assign:
                case ast_mod_assign:
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