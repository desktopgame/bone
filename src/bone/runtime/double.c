#include "double.h"
#include <stdio.h>
#include "enviroment.h"
#include "frame.h"
#include "integer.h"
#include "interpreter.h"
#include "lambda.h"
#include "string.h"

bnDouble* bnNewDouble(bnInterpreter* bone, double value) {
        bnDouble* ret = BN_MALLOC(sizeof(bnDouble));
        bnInitObject(bone, &ret->base, BN_OBJECT_DOUBLE);
        ret->value = value;
        bnDefine(&ret->base, bnIntern(bone->pool, "toString"),
                 bnNewLambdaFromCFunc(bone, bnStdDoubleToString, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_RETURN,
                                      "ret", BN_C_ADD_EXIT));
        return ret;
}

void bnStdDoubleFuncCall(bnInterpreter* bone, bnFrame* frame) {}

void bnStdDoublePositive(bnInterpreter* bone, bnFrame* frame) {}

void bnStdDoubleNegative(bnInterpreter* bone, bnFrame* frame) {}

void bnStdDoubleChilda(bnInterpreter* bone, bnFrame* frame) {}

void bnStdDoubleNot(bnInterpreter* bone, bnFrame* frame) {}

void bnStdDoublePlus(bnInterpreter* bone, bnFrame* frame) {}

void bnStdDoubleMinus(bnInterpreter* bone, bnFrame* frame) {}

void bnStdDoubleMultiply(bnInterpreter* bone, bnFrame* frame) {}

void bnStdDoubleDivide(bnInterpreter* bone, bnFrame* frame) {}

void bnStdDoubleModulo(bnInterpreter* bone, bnFrame* frame) {}

void bnStdDoubleBitAnd(bnInterpreter* bone, bnFrame* frame) {}

void bnStdDoubleBitOr(bnInterpreter* bone, bnFrame* frame) {}

// void bnStdDoubleLogicAnd(bnInterpreter* bone, bnFrame* frame){}

// void bnStdDoubleLogicOr(bnInterpreter* bone, bnFrame* frame){}

void bnStdDoubleExcOr(bnInterpreter* bone, bnFrame* frame) {}

void bnStdDoubleLShift(bnInterpreter* bone, bnFrame* frame) {}

void bnStdDoubleRShift(bnInterpreter* bone, bnFrame* frame) {}

void bnStdDoubleGT(bnInterpreter* bone, bnFrame* frame) {}

void bnStdDoubleGE(bnInterpreter* bone, bnFrame* frame) {}

void bnStdDoubleLT(bnInterpreter* bone, bnFrame* frame) {}

void bnStdDoubleLE(bnInterpreter* bone, bnFrame* frame) {}

void bnStdDoubleEqual(bnInterpreter* bone, bnFrame* frame) {}

void bnStdDoubleNotEqual(bnInterpreter* bone, bnFrame* frame) {}

void bnStdDoubleToString(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE) {
                bnFormatThrow(bone, "internal error");
        }
        char buf[100];
        sprintf(buf, "%lf", ((bnDouble*)a)->value);
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewString(bone, bnIntern(bone->pool, buf)));
}
