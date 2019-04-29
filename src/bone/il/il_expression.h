#ifndef BONE_IL_ILEXPRESSION_H
#define BONE_IL_ILEXPRESSION_H
typedef enum bnILExpressionType {
        BN_IL_EXPR_NONE,
        BN_IL_EXPR_INT,
        BN_IL_EXPR_DOUBLE,
        BN_IL_EXPR_CHAR,
        BN_IL_EXPR_STRING,
} bnILExpressionType;

struct bnILExprInt;
struct bnILExprDouble;
struct bnILExprChar;
struct bnILExprString;

typedef struct bnILExpression {
        bnILExpressionType type;
        union {
                struct bnILExprInt* vInt;
                struct bnILExprDouble* vDouble;
                struct bnILExprChar* vChar;
                struct bnILExprString* vString;
        } u;
} bnILExpression;

void bnDeleteILExpression(bnILExpression* self);
#endif