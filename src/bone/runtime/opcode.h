#ifndef BONE_RUNTIME_OPCODE_H
#define BONE_RUNTIME_OPCODE_H
#include <stdio.h>
#include "../bone.h"

/**
 * bnOpcode is used by bone vm.
 * bnOpcode/bone vm policy
 * -- don't use constant pool
 *    for able to prototype object a every
 *    so,  always instance a primitive type
 */
typedef enum bnOpcode {
        BN_OP_NOP,
        BN_OP_DUP,
        BN_OP_SWAP,

        BN_OP_GEN_INT,
        BN_OP_GEN_DOUBLE,
        BN_OP_GEN_STRING,
        BN_OP_GEN_LAMBDA_BEGIN,
        BN_OP_GEN_LAMBDA_END,

        BN_OP_SET_REGISTER_0,
        BN_OP_GET_REGISTER_0,
        BN_OP_PUSH_SELF,
        BN_OP_POP_SELF,

        BN_OP_STORE,
        BN_OP_LOAD,

        BN_OP_PUT,
        BN_OP_GET,

        BN_OP_GOTO,
        BN_OP_GOTO_IF,

        BN_OP_FUNCCALL,
} bnOpcode;

int bnPrintOpcode(FILE* fp, struct bnStringPool* pool, GPtrArray* ary, int pos);
#endif