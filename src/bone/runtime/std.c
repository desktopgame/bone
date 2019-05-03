#include "std.h"
#include "../bone.h"
#include "frame.h"
#include "integer.h"
#include "interpreter.h"
#include "object.h"

void bnIntegerFuncCall(bnInterpreter* bone, bnFrame* frame) {
        bnPanic(bone->__jmp, NULL, BN_JMP_CODE_EXCEPTION);
}

void bnIntegerPositive(bnInterpreter* bone, bnFrame* frame) {
        bnPanic(bone->__jmp, NULL, BN_JMP_CODE_EXCEPTION);
}

void bnIntegerNegative(bnInterpreter* bone, bnFrame* frame) {
        bnPanic(bone->__jmp, NULL, BN_JMP_CODE_EXCEPTION);
}

void bnIntegerChilda(bnInterpreter* bone, bnFrame* frame) {
        bnPanic(bone->__jmp, NULL, BN_JMP_CODE_EXCEPTION);
}

void bnIntegerNot(bnInterpreter* bone, bnFrame* frame) {
        bnPanic(bone->__jmp, NULL, BN_JMP_CODE_EXCEPTION);
}

void bnIntegerPlus(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
}

void bnIntegerMinus(bnInterpreter* bone, bnFrame* frame) {}

void bnIntegerMultiply(bnInterpreter* bone, bnFrame* frame) {}

void bnIntegerDivide(bnInterpreter* bone, bnFrame* frame) {}

void bnIntegerModulo(bnInterpreter* bone, bnFrame* frame) {}

void bnIntegerBitAnd(bnInterpreter* bone, bnFrame* frame) {}

void bnIntegerBitOr(bnInterpreter* bone, bnFrame* frame) {}

// void bnIntegerLogicAnd(bnInterpreter* bone, bnFrame* frame){}

// void bnIntegerLogicOr(bnInterpreter* bone, bnFrame* frame){}

void bnIntegerExcOr(bnInterpreter* bone, bnFrame* frame) {}

void bnIntegerLShift(bnInterpreter* bone, bnFrame* frame) {}

void bnIntegerRShift(bnInterpreter* bone, bnFrame* frame) {}

void bnIntegerGT(bnInterpreter* bone, bnFrame* frame) {}

void bnIntegerGE(bnInterpreter* bone, bnFrame* frame) {}

void bnIntegerLT(bnInterpreter* bone, bnFrame* frame) {}

void bnIntegerLE(bnInterpreter* bone, bnFrame* frame) {}

void bnIntegerEqual(bnInterpreter* bone, bnFrame* frame) {}

void bnIntegerNotEqual(bnInterpreter* bone, bnFrame* frame) {}
