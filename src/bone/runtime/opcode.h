#ifndef BONE_RUNTIME_OPCODE_H
#define BONE_RUNTIME_OPCODE_H
#include <stdio.h>
#include "../bone.h"
struct bnEnviroment;

/**
 * bnOpcodeはインタプリタが実行するバイトコードの一覧です。
 */
typedef enum bnOpcode {
        BN_OP_NOP,
        BN_OP_DUP,
        BN_OP_SWAP,

        BN_OP_GEN_INT,
        BN_OP_GEN_DOUBLE,
        BN_OP_GEN_STRING,
        BN_OP_GEN_ARRAY,
        BN_OP_GEN_LAMBDA_BEGIN,
        BN_OP_GEN_LAMBDA_END,

        BN_OP_SET_REGISTER_0,
        BN_OP_GET_REGISTER_0,
        BN_OP_PUSH_SELF,
        BN_OP_POP_SELF,

        BN_OP_SCOPE_INJECTION,
        BN_OP_OBJECT_INJECTION,

        BN_OP_STORE,
        BN_OP_LOAD,

        BN_OP_PUT,
        BN_OP_GET,

        BN_OP_GOTO,
        BN_OP_GOTO_IF,
        BN_OP_GOTO_ELSE,

        BN_OP_RETURN,

        BN_OP_FUNCCALL,
        BN_OP_GEN_CHAR,
        BN_OP_DEFER_PUSH,
        BN_OP_DEFER_BEGIN,
        BN_OP_DEFER_NEXT,
        BN_OP_DEFER_END,
        BN_OP_POP,
        BN_OP_CLEANUP_INJBUF,
} bnOpcode;

/**
 * オペコードを出力して、次のオペコードの位置を返します。
 * @param fp
 * @param pool
 * @param env
 * @param pos
 * @return
 */
int bnPrintOpcode(FILE* fp, struct bnStringPool* pool, struct bnEnviroment* env,
                  int pos);

/**
 * 指定のオペコードに必要なオペランドの数を返します。
 * @param data
 * @return
 */
int bnOperands(bnOpcode data);
#endif