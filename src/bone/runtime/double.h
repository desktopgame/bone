#ifndef BONE_RUNTIME_DOUBLE_H
#define BONE_RUNTIME_DOUBLE_H
#include "object.h"
struct bnFrame;
struct bnInterpreter;

/**
 * bnDouble is double.
 */
typedef struct bnDouble {
        bnObject base;
        double value;
} bnDouble;

/**
 * return new instance of bnDouble.
 * @param bone
 * @param name
 * @return
 */
bnDouble* bnNewDouble(struct bnInterpreter* bone, double value);

void bnStdDoubleFuncCall(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDoublePositive(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDoubleNegative(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDoubleChilda(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDoubleNot(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDoublePlus(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDoubleMinus(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDoubleMultiply(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDoubleDivide(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDoubleModulo(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDoubleBitAnd(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDoubleBitOr(struct bnInterpreter* bone, struct bnFrame* frame);

// void bnStdDoubleLogicAnd(struct bnInterpreter* bone, struct bnFrame* frame);

// void bnStdDoubleLogicOr(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDoubleExcOr(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDoubleLShift(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDoubleRShift(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDoubleGT(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDoubleGE(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDoubleLT(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDoubleLE(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDoubleEqual(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDoubleNotEqual(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDoubleToString(struct bnInterpreter* bone, struct bnFrame* frame);
#endif