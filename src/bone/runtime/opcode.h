#ifndef BONE_RUNTIME_OPCODE_H
#define BONE_RUNTIME_OPCODE_H

/**
 * bnOpcode is used by bone vm.
 * bnOpcode/bone vm policy
 * -- don't use constant pool
 *    for able to prototype object a every
 *    so,  always instance a primitive type
 */
typedef enum bnOpcode {
        BN_OP_NOP,

        BN_OP_GEN_INT,
        BN_OP_GEN_DOUBLE,
        BN_OP_GEN_STRING,

        BN_OP_STORE,
        BN_OP_LOAD,

        BN_OP_PUT,
        BN_OP_GET,

        BN_OP_FUNCCALL,
        BN_OP_POSITIVE,
        BN_OP_NEGATIVE,
        BN_OP_CHILDA,
        BN_OP_NOT,

        BN_OP_PLUS,
        BN_OP_MINUS,
        BN_OP_MULTIPLY,
        BN_OP_DIVIDE,
        BN_OP_MODULO,

        BN_OP_BIT_AND,
        BN_OP_BIT_OR,
        BN_OP_LOGIC_AND,
        BN_OP_LOGIC_OR,
        BN_OP_EXC_OR,

        BN_OP_LSHIFT,
        BN_OP_RSHIFT,

        BN_OP_GT,
        BN_OP_GE,
        BN_OP_LT,
        BN_OP_LE,

        BN_OP_EQUAL,
        BN_OP_NOTEQUAL,
} bnOpcode;
#endif