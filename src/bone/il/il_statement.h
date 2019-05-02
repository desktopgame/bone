#ifndef BONE_IL_ILSTATEMENT_H
#define BONE_IL_ILSTATEMENT_H
#include <stdio.h>
#include "../util/string_pool.h"

typedef enum bnILStatementType {
        BN_IL_STMT_NONE,
        BN_IL_STMT_EXPRSTMT,
        BN_IL_STMT_IF,
        BN_IL_STMT_IF_ELSE,
        BN_IL_STMT_WHILE,
} bnILStatementType;

struct bnILStmtExprStmt;
struct bnILStmtIf;
struct bnILStmtIfElse;
struct bnILStmtWhile;
struct bnEnviroment;

typedef struct bnILStatement {
        bnILStatementType type;
        union {
                struct bnILStmtExprStmt* vExprStmt;
                struct bnILStmtIf* vIf;
                struct bnILStmtIfElse* vIfElse;
                struct bnILStmtWhile* vWhile;
        } u;
        int line;
} bnILStatement;

bnILStatement* bnNewILStatement(bnILStatementType type);

void bnDumpILStatement(FILE* fp, struct bnStringPool* pool, bnILStatement* self,
                       int depth);

void bnDeleteILStatement(bnILStatement* self);

void bnGenerateILStatement(bnILStatement* self, struct bnEnviroment* env);
#endif