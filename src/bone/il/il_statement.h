#ifndef BONE_IL_ILSTATEMENT_H
#define BONE_IL_ILSTATEMENT_H
#include <stdio.h>
#include "../util/string_pool.h"
#include "compile_cache.h"

typedef enum bnILStatementType {
        BN_IL_STMT_NONE,
        BN_IL_STMT_EXPRSTMT,
        BN_IL_STMT_IF,
        BN_IL_STMT_IF_ELSE,
        BN_IL_STMT_WHILE,
        BN_IL_STMT_RETURN,
        BN_IL_STMT_SCOPE_INJECTION,
        BN_IL_STMT_PANIC,
        BN_IL_STMT_DEFER,
        BN_IL_STMT_CONTINUE,
        BN_IL_STMT_BREAK,
} bnILStatementType;

struct bnILStmtExprStmt;
struct bnILStmtIf;
struct bnILStmtIfElse;
struct bnILStmtWhile;
struct bnILStmtReturn;
struct bnILStmtScopeInjection;
struct bnEnviroment;
struct bnInterpreter;
struct bnILStmtDefer;

typedef struct bnILStatement {
        bnILStatementType type;
        union {
                struct bnILStmtExprStmt* vExprStmt;
                struct bnILStmtIf* vIf;
                struct bnILStmtIfElse* vIfElse;
                struct bnILStmtWhile* vWhile;
                struct bnILStmtReturn* vReturn;
                struct bnILStmtScopeInjection* vScopeInj;
                struct bnILStmtDefer* vDefer;
        } u;
        int line;
} bnILStatement;

bnILStatement* bnNewILStatement(bnILStatementType type);

void bnDumpILStatement(FILE* fp, struct bnStringPool* pool, bnILStatement* self,
                       int depth);

void bnDeleteILStatement(bnILStatement* self);

void bnGenerateILStatement(struct bnInterpreter* bone, bnILStatement* self,
                           struct bnEnviroment* env, bnCompileCache* ccache);
#endif