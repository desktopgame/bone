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

// Bool

void bnStdBoolFuncCall(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolPositive(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolNegative(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolChilda(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolNot(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolPlus(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolMinus(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolMultiply(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolDivide(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolModulo(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolBitAnd(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolBitOr(bnInterpreter* bone, bnFrame* frame);

// void bnStdBoolLogicAnd(struct bnInterpreter* bone, struct bnFrame* frame);

// void bnStdBoolLogicOr(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolExcOr(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolLShift(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolRShift(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolGT(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolGE(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolLT(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolLE(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolEqual(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolNotEqual(bnInterpreter* bone, bnFrame* frame);

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

void bnStdIntegerMinus(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai - bi));
}

void bnStdIntegerMultiply(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai * bi));
}

void bnStdIntegerDivide(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai / bi));
}

void bnStdIntegerModulo(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai % bi));
}

void bnStdIntegerBitAnd(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai & bi));
}

void bnStdIntegerBitOr(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai | bi));
}

// void bnStdIntegerLogicAnd(bnInterpreter* bone, bnFrame* frame){}

// void bnStdIntegerLogicOr(bnInterpreter* bone, bnFrame* frame){}

void bnStdIntegerExcOr(bnInterpreter* bone, bnFrame* frame) {}

void bnStdIntegerLShift(bnInterpreter* bone, bnFrame* frame) {}

void bnStdIntegerRShift(bnInterpreter* bone, bnFrame* frame) {}

void bnStdIntegerGT(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai > bi));
}

void bnStdIntegerGE(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai >= bi));
}

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

void bnStdIntegerLE(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai <= bi));
}

void bnStdIntegerEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai == bi));
}

void bnStdIntegerNotEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai != bi));
}

void bnStdIntegerToString(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER) {
                bnPanic(bone, NULL, BN_JMP_CODE_EXCEPTION);
        }
        char buf[100];
        sprintf(buf, "%d", ((bnInteger*)a)->value);
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewString(bone, bnIntern(bone->pool, buf)));
}