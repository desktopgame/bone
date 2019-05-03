#ifndef BONE_RUNTIME_STD_H
#define BONE_RUNTIME_STD_H
struct bnFrame;
struct bnInterpreter;

void bnStdSystemPrint(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemPrintln(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdIntegerFuncCall(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdIntegerPositive(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdIntegerNegative(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdIntegerChilda(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdIntegerNot(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdIntegerPlus(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdIntegerMinus(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdIntegerMultiply(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdIntegerDivide(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdIntegerModulo(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdIntegerBitAnd(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdIntegerBitOr(struct bnInterpreter* bone, struct bnFrame* frame);

// void bnStdIntegerLogicAnd(struct bnInterpreter* bone, struct bnFrame* frame);

// void bnStdIntegerLogicOr(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdIntegerExcOr(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdIntegerLShift(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdIntegerRShift(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdIntegerGT(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdIntegerGE(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdIntegerLT(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdIntegerLE(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdIntegerEqual(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdIntegerNotEqual(struct bnInterpreter* bone, struct bnFrame* frame);

#endif