#ifndef BONE_PARSE_AST2IL_H
#define BONE_PARSE_AST2IL_H
#include "../il/il_toplevel.h"
#include "ast.h"

/**
 * ASTを再帰的に解析して中間表現に変換します。
 * @param a
 * @return
 */
bnILToplevel* bnAST2IL(bnAST* a);
#endif