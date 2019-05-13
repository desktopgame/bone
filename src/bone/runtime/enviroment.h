#ifndef BONE_RUNTIME_ENVIROMENT_H
#define BONE_RUNTIME_ENVIROMENT_H
#include "../bone.h"
#include "../util/stack.h"
#include "label.h"
#include "opcode.h"

struct bnLambda;

typedef struct bnEnviroment {
        GPtrArray* codeArray;
        GPtrArray* labels;
        GPtrArray* ranges;
        bnStack* labelFixStack;
} bnEnviroment;

/**
 * return new instance of bnEnviroment.
 * @param type
 * @return
 */
bnEnviroment* bnNewEnviroment();

/**
 * @param self
 * @param lineno
 */
void bnAddLineRange(bnEnviroment* self, int lineno);

/**
 * @param self
 * @param pos
 * @return
 */
bnLabel* bnAutoNewLabel(bnEnviroment* self, int pos);

/**
 * @param self
 * @param pos
 * @return
 */
bnLabel* bnGenerateLabel(bnEnviroment* self, int pos);

/**
 * @param self
 * @return
 */
int bnGenerateNOP(bnEnviroment* self);

/**
 * @param self
 * @param count
 */
void bnGenerateFillNOP(bnEnviroment* self, int count);

/**
 * @param self
 */
void bnGenerateEnterLambda(bnEnviroment* self);

/**
 * @param self
 */
void bnGenerateExitLambda(bnEnviroment* self);

/**
 * @param self
 * @return
 */
int bnGetLambdaOffset(bnEnviroment* self);

/**
 * @param self
 * @return
 */
int bnGetPrependPos(bnEnviroment* self);

/**
 * free a bnEnviroment.
 * @param self
 */
void bnDeleteEnviroment(bnEnviroment* self);
#endif