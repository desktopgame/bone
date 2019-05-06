#ifndef BONE_RUNTIME_STD_H
#define BONE_RUNTIME_STD_H
struct bnFrame;
struct bnInterpreter;

// only in debug build
#if DEBUG
void bnStdDebugAssert(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDebugDie(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDebugPrint(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDebugPrintln(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDebugBreak(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdDebugDumpTable(struct bnInterpreter* bone, struct bnFrame* frame);
#endif

// Built-in

void bnStdSystemInclude(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemLoad(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemObject(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemArray(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemRecover(struct bnInterpreter* bone, struct bnFrame* frame);

// Bool

void bnStdBoolFuncCall(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolPositive(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolNegative(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolChilda(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolNot(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolPlus(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolMinus(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolMultiply(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolDivide(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolModulo(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolBitAnd(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolBitOr(struct bnInterpreter* bone, struct bnFrame* frame);

// void bnStdBoolLogicAnd(struct bnInterpreter* bone, struct bnFrame* frame);

// void bnStdBoolLogicOr(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolExcOr(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolLShift(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolRShift(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolGT(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolGE(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolLT(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolLE(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolEqual(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolNotEqual(struct bnInterpreter* bone, struct bnFrame* frame);

// Integer
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

void bnStdIntegerToString(struct bnInterpreter* bone, struct bnFrame* frame);

// String

void bnStdStringFuncCall(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdStringPositive(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdStringNegative(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdStringChilda(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdStringNot(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdStringPlus(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdStringMinus(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdStringMultiply(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdStringDivide(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdStringModulo(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdStringBitAnd(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdStringBitOr(struct bnInterpreter* bone, struct bnFrame* frame);

// void bnStdStringLogicAnd(struct bnInterpreter* bone, struct bnFrame* frame);

// void bnStdStringLogicOr(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdStringExcOr(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdStringLShift(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdStringRShift(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdStringGT(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdStringGE(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdStringLT(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdStringLE(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdStringEqual(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdStringNotEqual(struct bnInterpreter* bone, struct bnFrame* frame);

// Array

void bnStdArrayArraySet(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdArrayArrayGet(struct bnInterpreter* bone, struct bnFrame* frame);
#endif