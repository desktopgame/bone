#include "ast.h"
#include <assert.h>
#include <stdio.h>
#include "parser.h"

// proto
static void ast_child_delete(gpointer item);
static void bnDumpASTImpl(FILE* fp, bnAST* self, int depth);

bnAST* bnNewAST(bnASTTag tag) {
        bnAST* ret = (bnAST*)BN_MALLOC(sizeof(bnAST));
        ret->tag = tag;
        ret->children = NULL;
        ret->line = bnGetParseLine();
        bnPushParseLine(ret->line);
        return ret;
}

bnAST* bnNewBlankAST() {
        bnAST* ret = bnNewAST(BN_AST_BLANK);
        return ret;
}

bnAST* bnNewExprStmtAST(bnAST* aexpr) {
        bnAST* ret = bnNewAST(BN_AST_EXPRSTMT);
        bnPushAST(ret, aexpr);
        return ret;
}

bnAST* bnNewIfAST(bnAST* aexpr, bnAST* astmt) {
        bnAST* ret = bnNewAST(BN_AST_IF);
        bnPushAST(ret, aexpr);
        bnPushAST(ret, astmt);
        return ret;
}

bnAST* bnNewIfElseAST(bnAST* aif, bnAST* astmt) {
        bnAST* ret = bnNewAST(BN_AST_IF_ELSE);
        bnPushAST(ret, aif);
        bnPushAST(ret, astmt);
        return ret;
}

bnAST* bnNewWhileAST(bnAST* aexpr, bnAST* astmt) {
        bnAST* ret = bnNewAST(BN_AST_WHILE);
        bnPushAST(ret, aexpr);
        bnPushAST(ret, astmt);
        return ret;
}

bnAST* bnNewArgumentAST(bnAST* aexpr) {
        bnAST* ret = bnNewAST(BN_AST_ARGUMENT);
        bnPushAST(ret, aexpr);
        return ret;
}

bnAST* bnNewArgumentListAST(bnAST* aexpr, bnAST* aargs) {
        bnAST* ret = bnNewAST(BN_AST_ARGUMENT_LIST);
        bnPushAST(ret, aargs);
        bnPushAST(ret, aexpr);
        return ret;
}

bnAST* bnNewParameterAST(GString* name) {
        bnAST* ret = bnNewAST(BN_AST_PARAMETER);
        ret->u.svalue = name;
        return ret;
}

bnAST* bnNewParameterListAST(bnAST* aparam, bnAST* aparams) {
        bnAST* ret = bnNewAST(BN_AST_PARAMETER_LIST);
        bnPushAST(ret, aparams);
        bnPushAST(ret, aparam);
        return ret;
}

bnAST* bnNewLambda(bnAST* aparams, bnAST* areturn, bnAST* astmt) {
        bnAST* ret = bnNewAST(BN_AST_LAMBDA);
        bnPushAST(ret, aparams);
        bnPushAST(ret, areturn);
        bnPushAST(ret, astmt);
        return ret;
}

bnAST* bnNewStatementListAST(bnAST* astmt, bnAST* astmtList) {
        bnAST* ret = bnNewAST(BN_AST_STATEMENT_LIST);
        bnPushAST(ret, astmt);
        bnPushAST(ret, astmtList);
        return ret;
}

bnAST* bnNewVariableAST(GString* svalue) {
        bnAST* ret = bnNewAST(BN_AST_VARIABLE);
        ret->u.svalue = svalue;
        return ret;
}

bnAST* bnNewMemberAccessAST(bnAST* aexpr, GString* name) {
        bnAST* ret = bnNewAST(BN_AST_MEMBER_ACCESS);
        ret->u.svalue = name;
        bnPushAST(ret, aexpr);
        return ret;
}

bnAST* bnNewFuncCall(bnAST* aexpr, bnAST* aargs) {
        bnAST* ret = bnNewAST(BN_AST_FUNCCALL);
        bnPushAST(ret, aexpr);
        bnPushAST(ret, aargs);
        return ret;
}

bnAST* bnNewIntAST(int ivalue) {
        bnAST* ret = bnNewAST(BN_AST_INT_LIT);
        ret->u.ivalue = ivalue;
        return ret;
}

bnAST* bnNewStringAST(GString* svalue) {
        bnAST* ret = bnNewAST(BN_AST_STRING_LIT);
        ret->u.svalue = svalue;
        return ret;
}

bnAST* bnNewCharAST(char cvalue) {
        bnAST* ret = bnNewAST(BN_AST_CHAR_LIT);
        ret->u.cvalue = cvalue;
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
        self->line = bnPopParseLine();
}

void bnDumpAST(FILE* fp, bnAST* self) { bnDumpASTImpl(fp, self, 0); }

void bnPrintAST(FILE* fp, bnAST* self) {
#define p(a)              \
        fprintf(fp, (a)); \
        break
        switch (self->tag) {
                case BN_AST_ROOT:
                        p("root");
                case BN_AST_INT_LIT:
                        fprintf(fp, "%d", self->u.ivalue);
                        break;
                case BN_AST_DOUBLE_LIT:
                        fprintf(fp, "%lf", self->u.dvalue);
                        break;
                case BN_AST_STRING_LIT:
                        fprintf(fp, "%s", self->u.svalue->str);
                        break;
                case BN_AST_CHAR_LIT:
                        fprintf(fp, "%c", self->u.cvalue);
                        break;
                case BN_AST_VARIABLE:
                        fprintf(fp, "$%s", self->u.svalue->str);
                        break;
                case BN_AST_BLANK:
                        fprintf(fp, "BLANK");
                        break;
                case BN_AST_FUNCCALL:
                        fprintf(fp, "FuncCall");
                        break;
                case BN_AST_IF:
                        fprintf(fp, "if");
                        break;
                case BN_AST_IF_ELSE:
                        fprintf(fp, "if-else");
                        break;
                case BN_AST_WHILE:
                        fprintf(fp, "while");
                        break;
                case BN_AST_STATEMENT_LIST:
                        fprintf(fp, "StmtList");
                        break;
                case BN_AST_MEMBER_ACCESS:
                        fprintf(fp, ".%s", self->u.svalue->str);
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
                        p(":=");
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
                        fprintf(fp, "undefined");
                        break;
        }
        fprintf(fp, "<%d>", self->line);
#undef p
}

void bnDeleteAST(bnAST* self) {
        if (self == NULL) {
                return;
        }
        g_list_free_full(self->children, ast_child_delete);
        free(self);
}

bnAST* bnFirstAST(bnAST* self) { return self->children->data; }

bnAST* bnSecondAST(bnAST* self) { return self->children->next->data; }

bnAST* bnThirdAST(bnAST* self) { return self->children->next->next->data; }

double bnEvalAST(bnAST* self) {
        if (self->tag == BN_AST_INT_LIT) {
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

static void bnDumpASTImpl(FILE* fp, bnAST* self, int depth) {
        for (int i = 0; i < depth; i++) {
                fprintf(fp, " ");
        }
        bnPrintAST(fp, self);
        fprintf(fp, "\n");
        GList* iter = self->children;
        while (iter != NULL) {
                bnAST* e = iter->data;
                bnDumpASTImpl(fp, e, depth + 1);
                iter = iter->next;
        }
}