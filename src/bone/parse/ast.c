#include "ast.h"
#include <assert.h>
#include <stdio.h>

// proto
static void ast_child_delete(gpointer item);
static void bnDumpASTImpl(bnAST* self, int depth);

bnAST* bnNewAST(bnASTTag tag) {
        bnAST* ret = (bnAST*)malloc(sizeof(bnAST));
        ret->tag = tag;
        ret->children = NULL;
        return ret;
}

bnAST* bnNewIntAST(int ivalue) {
        bnAST* ret = bnNewAST(BN_AST_LIT);
        ret->u.ivalue = ivalue;
        return ret;
}

bnAST* bnNewDoubleAST(double dvalue) {
        bnAST* ret = bnNewAST(BN_AST_DOUBLE_LIT);
        ret->u.dvalue = dvalue;
        return ret;
}

bnAST* bnNewUnaryAST(bnASTTag tag, bnAST* a) {
        bnAST* ret = bnNewAST(tag);
        bnPushAST(ret, a);
        return ret;
}

bnAST* bnNewBinaryAST(bnASTTag tag, bnAST* left, bnAST* right) {
        bnAST* ret = bnNewAST(tag);
        bnPushAST(ret, left);
        bnPushAST(ret, right);
        return ret;
}

void bnPushAST(bnAST* self, bnAST* a) {
        assert(self != NULL && a != NULL);
        self->children = g_list_append(self->children, a);
}

void bnDumpAST(bnAST* self) { bnDumpASTImpl(self, 0); }

void bnPrintAST(bnAST* self) {
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

void bnDeleteAST(bnAST* self) {
        g_list_free_full(self->children, ast_child_delete);
        free(self);
}

bnAST* bnFirstAST(bnAST* self) { return self->children->data; }

bnAST* bnSecondAST(bnAST* self) { return self->children->next->data; }

double bnEvalAST(bnAST* self) {
        if (self->tag == BN_AST_LIT) {
                return (double)self->u.ivalue;
        }
        if (self->tag == BN_AST_DOUBLE_LIT) {
                return self->u.dvalue;
        }
        switch (self->tag) {
                case BN_AST_PLUS:
                        return (bnEvalAST(bnFirstAST(self)) +
                                bnEvalAST(bnSecondAST(self)));
                case BN_AST_POSITIVE:
                        return +(bnEvalAST(bnFirstAST(self)));
                case BN_AST_MINUS:
                        return (bnEvalAST(bnFirstAST(self)) -
                                bnEvalAST(bnSecondAST(self)));
                case BN_AST_NEGATIVE:
                        return -(bnEvalAST(bnFirstAST(self)));
                case BN_AST_MULTIPLY:
                        return (bnEvalAST(bnFirstAST(self)) *
                                bnEvalAST(bnSecondAST(self)));
                case BN_AST_DIVIDE:
                        return (bnEvalAST(bnFirstAST(self)) /
                                bnEvalAST(bnSecondAST(self)));
                case BN_AST_MODULO:
                        return ((int)bnEvalAST(bnFirstAST(self)) %
                                (int)bnEvalAST(bnSecondAST(self)));

                case BN_AST_BIT_OR:
                        return (int)bnEvalAST(bnFirstAST(self)) |
                               (int)bnEvalAST(bnSecondAST(self));
                case BN_AST_BIT_AND:
                        return (int)bnEvalAST(bnFirstAST(self)) &
                               (int)bnEvalAST(bnSecondAST(self));
                case BN_AST_LOGIC_OR:
                        return (int)bnEvalAST(bnFirstAST(self)) ||
                               (int)bnEvalAST(bnSecondAST(self));
                case BN_AST_LOGIC_AND:
                        return (int)bnEvalAST(bnFirstAST(self)) &&
                               (int)bnEvalAST(bnSecondAST(self));
                case BN_AST_EXC_OR:
                        return (int)bnEvalAST(bnFirstAST(self)) ^
                               (int)bnEvalAST(bnSecondAST(self));

                case BN_AST_LSHIFT:
                        return (int)bnEvalAST(bnFirstAST(self))
                               << (int)bnEvalAST(bnSecondAST(self));
                case BN_AST_RSHIFT:
                        return (int)bnEvalAST(bnFirstAST(self)) >>
                               (int)bnEvalAST(bnSecondAST(self));

                case BN_AST_GT:
                        return ((int)bnEvalAST(bnFirstAST(self)) >
                                (int)bnEvalAST(bnSecondAST(self)));
                case BN_AST_GE:
                        return ((int)bnEvalAST(bnFirstAST(self)) >=
                                (int)bnEvalAST(bnSecondAST(self)));
                case BN_AST_LT:
                        return ((int)bnEvalAST(bnFirstAST(self)) <
                                (int)bnEvalAST(bnSecondAST(self)));
                case BN_AST_LE:
                        return ((int)bnEvalAST(bnFirstAST(self)) <=
                                (int)bnEvalAST(bnSecondAST(self)));

                case BN_AST_CHILDA:
                        return ~(int)(bnEvalAST(bnFirstAST(self)));
                case BN_AST_NOT:
                        return !(int)(bnEvalAST(bnFirstAST(self)));

                case BN_AST_EQUAL:
                        return ((int)bnEvalAST(bnFirstAST(self)) ==
                                (int)bnEvalAST(bnSecondAST(self)));
                case BN_AST_NOTEQUAL:
                        return ((int)bnEvalAST(bnFirstAST(self)) !=
                                (int)bnEvalAST(bnSecondAST(self)));

                case BN_AST_ASSIGN:
                case BN_AST_PLUS_ASSIGN:
                case BN_AST_MINUS_ASSIGN:
                case BN_AST_MULTIPLY_ASSIGN:
                case BN_AST_DIVIDE_ASSIGN:
                case BN_AST_MODULO_ASSIGN:
                        return bnEvalAST(bnSecondAST(self));
                default:
                        return 0;
        }
        return 0.0;
}

// private
static void ast_child_delete(gpointer item) {
        bnAST* e = (bnAST*)item;
        bnDeleteAST(e);
}

static void bnDumpASTImpl(bnAST* self, int depth) {
        for (int i = 0; i < depth; i++) {
                printf(" ");
        }
        bnPrintAST(self);
        printf("\n");
        GList* iter = self->children;
        while (iter != NULL) {
                bnAST* e = iter->data;
                bnDumpASTImpl(e, depth + 1);
                iter = iter->next;
        }
}