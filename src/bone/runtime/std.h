#ifndef BONE_RUNTIME_STD_H
#define BONE_RUNTIME_STD_H
struct bnFrame;
struct bnInterpreter;

void bnIntegerFuncCall(struct bnInterpreter* bone, struct bnFrame* frame);

void bnIntegerPositive(struct bnInterpreter* bone, struct bnFrame* frame);

void bnIntegerNegative(struct bnInterpreter* bone, struct bnFrame* frame);

void bnIntegerChilda(struct bnInterpreter* bone, struct bnFrame* frame);

void bnIntegerNot(struct bnInterpreter* bone, struct bnFrame* frame);

void bnIntegerPlus(struct bnInterpreter* bone, struct bnFrame* frame);

void bnIntegerMinus(struct bnInterpreter* bone, struct bnFrame* frame);

void bnIntegerMultiply(struct bnInterpreter* bone, struct bnFrame* frame);

void bnIntegerDivide(struct bnInterpreter* bone, struct bnFrame* frame);

void bnIntegerModulo(struct bnInterpreter* bone, struct bnFrame* frame);

void bnIntegerBitAnd(struct bnInterpreter* bone, struct bnFrame* frame);

void bnIntegerBitOr(struct bnInterpreter* bone, struct bnFrame* frame);

// void bnIntegerLogicAnd(struct bnInterpreter* bone, struct bnFrame* frame);

// void bnIntegerLogicOr(struct bnInterpreter* bone, struct bnFrame* frame);

void bnIntegerExcOr(struct bnInterpreter* bone, struct bnFrame* frame);

void bnIntegerLShift(struct bnInterpreter* bone, struct bnFrame* frame);

void bnIntegerRShift(struct bnInterpreter* bone, struct bnFrame* frame);

void bnIntegerGT(struct bnInterpreter* bone, struct bnFrame* frame);

void bnIntegerGE(struct bnInterpreter* bone, struct bnFrame* frame);

void bnIntegerLT(struct bnInterpreter* bone, struct bnFrame* frame);

void bnIntegerLE(struct bnInterpreter* bone, struct bnFrame* frame);

void bnIntegerEqual(struct bnInterpreter* bone, struct bnFrame* frame);

void bnIntegerNotEqual(struct bnInterpreter* bone, struct bnFrame* frame);

#endif