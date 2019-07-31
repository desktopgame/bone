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

void bnStdDebugShowInfo(struct bnInterpreter* bone, struct bnFrame* frame);
#endif

// Built-in

void bnStdSystemInclude(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemLoad(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemEval(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemObject(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemString(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemArray(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemExternVar(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemExternDef(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemPanic(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdSystemRecover(struct bnInterpreter* bone, struct bnFrame* frame);

// Object

void bnStdObjectFuncCall(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdObjectPositive(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdObjectNegative(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdObjectChilda(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdObjectNot(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdObjectPlus(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdObjectMinus(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdObjectMultiply(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdObjectDivide(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdObjectModulo(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdObjectBitAnd(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdObjectBitOr(struct bnInterpreter* bone, struct bnFrame* frame);

// void bnStdObjectLogicAnd(struct bnInterpreter* bone, struct bnFrame* frame);

// void bnStdObjectLogicOr(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdObjectExcOr(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdObjectLShift(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdObjectRShift(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdObjectGT(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdObjectGE(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdObjectLT(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdObjectLE(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdObjectEqual(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdObjectNotEqual(struct bnInterpreter* bone, struct bnFrame* frame);

// Char

void bnStdCharFuncCall(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdCharPositive(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdCharNegative(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdCharChilda(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdCharNot(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdCharPlus(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdCharMinus(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdCharMultiply(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdCharDivide(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdCharModulo(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdCharBitAnd(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdCharBitOr(struct bnInterpreter* bone, struct bnFrame* frame);

// void bnStdCharLogicAnd(struct bnInterpreter* bone, struct bnFrame* frame);

// void bnStdCharLogicOr(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdCharExcOr(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdCharLShift(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdCharRShift(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdCharGT(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdCharGE(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdCharLT(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdCharLE(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdCharEqual(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdCharNotEqual(struct bnInterpreter* bone, struct bnFrame* frame);

// Array

void bnStdArrayArraySet(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdArrayArrayGet(struct bnInterpreter* bone, struct bnFrame* frame);
#endif