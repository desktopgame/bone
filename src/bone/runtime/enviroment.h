#ifndef BONE_RUNTIME_ENVIROMENT_H
#define BONE_RUNTIME_ENVIROMENT_H
#include "../bone.h"

typedef struct bnEnviroment {
        GList* binary;
} bnEnviroment;

/**
 * return new instance of bnEnviroment.
 * @param type
 * @return
 */
bnEnviroment* bnNewEnviroment();

/**
 * free a bnEnviroment.
 * @param self
 */
void bnDeleteEnviroment(bnEnviroment* self);
#endif