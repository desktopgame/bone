#include "ast2il.h"
#include "../bone.h"
#include "../il/il_expr_all.h"
#include "../il/il_stmt_all.h"

static bnILExprBinOp* ast2ilbinop(bnAST* a, bnILBinOpType type);
static bnILExpression* ast2expr(bnAST* a);
static bnILStatement* ast2stmt(bnAST* a);
static void ast2stmts(bnAST* a, GList* dest);

static bnILExprBinOp* ast2ilbinop(bnAST* a, bnILBinOpType type) {
        bnILExprBinOp* ret = bnNewILExprBinOp(type);
        bnAST* left = bnFirstAST(a);
        bnAST* right = bnSecondAST(a);
        ret->left = ast2expr(left);
        ret->left = ast2expr(right);
        return ret;
}

static bnILExpression* ast2expr(bnAST* a) {
        bnILExpression* ret = bnNewILExpression(BN_IL_EXPR_NONE);
        if (a->tag == BN_AST_INT_LIT) {
                ret->type = BN_IL_EXPR_INT;
                ret->u.vInt = bnNewILExprInt(a->u.ivalue);
        } else if (a->tag == BN_AST_DOUBLE_LIT) {
                ret->type = BN_IL_EXPR_DOUBLE;
                ret->u.vDouble = bnNewILExprDouble(a->u.dvalue);
        } else if (a->tag == BN_AST_CHAR_LIT) {
                ret->type = BN_IL_EXPR_CHAR;
                ret->u.vChar = bnNewILExprChar(a->u.cvalue);
        } else if (a->tag == BN_AST_STRING_LIT) {
                ret->type = BN_IL_EXPR_STRING;
                ret->u.vString = bnNewILExprString(a->u.svalue);
        } else if (a->tag == BN_AST_PLUS) {
                ret->type = BN_IL_EXPR_BINOP;
                ret->u.vBinOp = ast2ilbinop(a, BN_IL_BINOP_PLUS);
        } else if (a->tag == BN_AST_MINUS) {
                ret->type = BN_IL_EXPR_BINOP;
                ret->u.vBinOp = ast2ilbinop(a, BN_IL_BINOP_MINUS);
        } else if (a->tag == BN_AST_MULTIPLY) {
                ret->type = BN_IL_EXPR_BINOP;
                ret->u.vBinOp = ast2ilbinop(a, BN_IL_BINOP_MULTIPLY);
        } else if (a->tag == BN_AST_DIVIDE) {
                ret->type = BN_IL_EXPR_BINOP;
                ret->u.vBinOp = ast2ilbinop(a, BN_IL_BINOP_DIVIDE);
        } else if (a->tag == BN_AST_MODULO) {
                ret->type = BN_IL_EXPR_BINOP;
                ret->u.vBinOp = ast2ilbinop(a, BN_IL_BINOP_MODULO);
        }
        return ret;
}

static bnILStatement* ast2stmt(bnAST* a) {
        bnILStatement* ret = bnNewILStatement(BN_IL_STMT_NONE);
        if (a->tag == BN_AST_EXPRSTMT) {
                ret->type = BN_IL_STMT_EXPRSTMT;
                ret->u.vExprStmt = bnNewILStmtExpr(ast2expr(bnFirstAST(a)));
        } else if (a->tag == BN_AST_IF) {
                ret->type = BN_IL_STMT_IF;
                ret->u.vIf = bnNewILStmtIf(ast2expr(bnFirstAST(a)));
                ast2stmts(bnSecondAST(a), ret->u.vIf->statements);
        } else if (a->tag == BN_AST_IF_ELSE) {
                ret->type = BN_IL_STMT_IF_ELSE;
                bnAST* aif = bnFirstAST(a);
                bnAST* aElseBody = bnSecondAST(a);
                bnILStmtIf* ilIf = bnNewILStmtIf(ast2expr(bnFirstAST(aif)));
                ast2stmts(bnSecondAST(aif), ilIf->statements);
                ret->u.vIfElse = bnNewILStmtIfElse(ilIf);
                ast2stmts(aElseBody, ret->u.vIfElse->statements);
        } else if (a->tag == BN_AST_WHILE) {
                ret->type = BN_IL_STMT_WHILE;
                bnAST* aCond = bnFirstAST(a);
                bnAST* aBody = bnSecondAST(a);
                ret->u.vWhile = bnNewILStmtWhile(ast2expr(aCond));
                ast2stmt(aBody, ret->u.vWhile->statements);
        }
        return ret;
}

static void ast2stmts(bnAST* a, GList* dest) {
        if (a->tag == BN_AST_STATEMENT_LIST) {
                GList* iter = a->children;
                while (iter != NULL) {
                        ast2stmts(iter->data, dest);
                        iter = iter->next;
                }
        } else {
                g_list_append(dest, ast2stmt(a));
        }
}

bnILToplevel* bnAST2IL(bnAST* a) {
        assert(a != NULL);
        bnILToplevel* ret = bnNewILToplevel();
        GList* iter = a->children;
        while (iter != NULL) {
                bnAST* child = iter->data;
                if (child->tag == BN_AST_STATEMENT_LIST) {
                        ast2stmts(child, ret->statements);
                } else {
                        g_list_append(ret->statements, ast2stmt(child));
                }
                iter = iter->next;
        }
        return ret;
}