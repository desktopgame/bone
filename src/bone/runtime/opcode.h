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

        BN_OP_STORE,
        BN_OP_LOAD,

        BN_OP_PUT,
        BN_OP_GET,

        BN_OP_FUNCCALL,
} bnOpcode;

GList* bnPrintOpcode(FILE* fp, struct bnStringPool* pool, GList* list);
#endif