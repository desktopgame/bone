#ifndef BONE_IL_ILSTATEMENT_H
#define BONE_IL_ILSTATEMENT_H
typedef enum bnILStatementType {
        BN_IL_STMT_NONE,
        BN_IL_STMT_EXPRSTMT,
        BN_IL_STMT_IF,
        BN_IL_STMT_WHILE,
} bnILStatementType;

struct bnILStmtExprStmt;
struct bnILStmtIf;
struct bnILStmtIfElse;
struct bnILStmtWhile;

typedef struct bnILStatement {
        bnILStatementType type;
        union {
                struct bnILStmtExprStmt* vExprStmt;
                struct bnILStmtIf* vIf;
                struct bnILStmtIfElse* vElse;
                struct bnILStmtWhile* vWhile;
        } u;
} bnILStatement;

void bnDeleteILStatement(bnILStatement* self);
#endif