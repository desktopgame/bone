#include "std.h"
#include "../bone.h"
#include "bool.h"
#include "frame.h"
#include "integer.h"
#include "interpreter.h"
#include "object.h"
#include "string.h"

// only in debug build
#if DEBUG
void bnStdDebugAssert(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_BOOL) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        bnBool* cond = a;
        if (!cond->value) {
                abort();
        }
}

void bnStdDebugDie(bnInterpreter* bone, bnFrame* frame) { abort(); }
#endif

// Built-in

void bnStdSystemObject(bnInterpreter* bone, bnFrame* frame) {
        g_hash_table_insert(frame->variableTable, bnIntern(bone->pool, "ret"),
                            bnNewObject());
}

void bnStdSystemPrint(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_STRING) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        bnString* str = a;
        const char* cstr = bnView2Str(bone->pool, str->value);
        printf(cstr);
        fflush(stdout);
}

void bnStdSystemPrintln(bnInterpreter* bone, bnFrame* frame) {
        bnStdSystemPrint(bone, frame);
        printf("\n");
}

// Integer

void bnStdIntegerFuncCall(bnInterpreter* bone, bnFrame* frame) {
        bnPanic(bone->__jmp, NULL, BN_JMP_CODE_EXCEPTION);
}

void bnStdIntegerPositive(bnInterpreter* bone, bnFrame* frame) {
        bnPanic(bone->__jmp, NULL, BN_JMP_CODE_EXCEPTION);
}

void bnStdIntegerNegative(bnInterpreter* bone, bnFrame* frame) {
        bnPanic(bone->__jmp, NULL, BN_JMP_CODE_EXCEPTION);
}

void bnStdIntegerChilda(bnInterpreter* bone, bnFrame* frame) {
        bnPanic(bone->__jmp, NULL, BN_JMP_CODE_EXCEPTION);
}

void bnStdIntegerNot(bnInterpreter* bone, bnFrame* frame) {
        bnPanic(bone->__jmp, NULL, BN_JMP_CODE_EXCEPTION);
}

void bnStdIntegerPlus(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai + bi));
}

void bnStdIntegerMinus(bnInterpreter* bone, bnFrame* frame) {}

void bnStdIntegerMultiply(bnInterpreter* bone, bnFrame* frame) {}

void bnStdIntegerDivide(bnInterpreter* bone, bnFrame* frame) {}

void bnStdIntegerModulo(bnInterpreter* bone, bnFrame* frame) {}

void bnStdIntegerBitAnd(bnInterpreter* bone, bnFrame* frame) {}

void bnStdIntegerBitOr(bnInterpreter* bone, bnFrame* frame) {}

// void bnStdIntegerLogicAnd(bnInterpreter* bone, bnFrame* frame){}

// void bnStdIntegerLogicOr(bnInterpreter* bone, bnFrame* frame){}

void bnStdIntegerExcOr(bnInterpreter* bone, bnFrame* frame) {}

void bnStdIntegerLShift(bnInterpreter* bone, bnFrame* frame) {}

void bnStdIntegerRShift(bnInterpreter* bone, bnFrame* frame) {}

void bnStdIntegerGT(bnInterpreter* bone, bnFrame* frame) {}

void bnStdIntegerGE(bnInterpreter* bone, bnFrame* frame) {}

void bnStdIntegerLT(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai < bi));
}

void bnStdIntegerLE(bnInterpreter* bone, bnFrame* frame) {}

void bnStdIntegerEqual(bnInterpreter* bone, bnFrame* frame) {}

void bnStdIntegerNotEqual(bnInterpreter* bone, bnFrame* frame) {}
