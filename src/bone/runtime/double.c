#include "double.h"
#include <stdio.h>
#include "enviroment.h"
#include "frame.h"
#include "integer.h"
#include "interpreter.h"
#include "lambda.h"
#include "string.h"


static void bnStdDoubleFuncCall(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoublePositive(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleNegative(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleChilda(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleNot(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoublePlus(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleMinus(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleMultiply(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleDivide(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleModulo(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleBitAnd(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleBitOr(bnInterpreter* bone, bnFrame* frame);
// static void bnStdDoubleLogicAnd(bnInterpreter* bone, bnFrame* frame);
// static void bnStdDoubleLogicOr(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleExcOr(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleLShift(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleRShift(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleGT(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleGE(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleLT(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleLE(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleEqual(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleNotEqual(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleToString(bnInterpreter* bone, bnFrame* frame);

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

static void bnStdDoubleToString(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE) {
                bnFormatThrow(bone, "internal error");
        }
        char buf[100];
        sprintf(buf, "%lf", ((bnDouble*)a)->value);
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewString(bone, bnIntern(bone->pool, buf)));
}
