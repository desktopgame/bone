#ifndef BONE_RUNTIME_ENVIROMENT_H
#define BONE_RUNTIME_ENVIROMENT_H
#include "../bone.h"
#include "label.h"
#include "opcode.h"

typedef struct bnEnviroment {
        GPtrArray* codeArray;
        GPtrArray* labels;
} bnEnviroment;

/**
 * return new instance of bnEnviroment.
 * @param type
 * @return
 */
bnEnviroment* bnNewEnviroment();

/**
 * @param env
 * @param pos
 * @return
 */
bnLabel* bnGenerateLabel(bnEnviroment* env, int pos);

/**
 * @param env
 * @return
 */
int bnGenerateNOP(bnEnviroment* env);

/**
 * free a bnEnviroment.
 * @param self
 */
void bnDeleteEnviroment(bnEnviroment* self);
#endif