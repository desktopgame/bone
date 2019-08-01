#include "double.h"
#include <math.h>
#include <stdio.h>
#include "bool.h"
#include "enviroment.h"
#include "frame.h"
#include "integer.h"
#include "interpreter.h"
#include "keyword.h"
#include "lambda.h"
#include "string.h"

#define _throw(bone, frame, fmt) (bnFormatThrow(bone, fmt))

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

        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_PLUS),
                 bnNewLambdaFromCFunc(bone, bnStdDoublePlus, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_MINUS),
                 bnNewLambdaFromCFunc(bone, bnStdDoubleMinus, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_MULTIPLY),
                 bnNewLambdaFromCFunc(bone, bnStdDoubleMultiply, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_DIVIDE),
                 bnNewLambdaFromCFunc(bone, bnStdDoubleDivide, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_MODULO),
                 bnNewLambdaFromCFunc(bone, bnStdDoubleModulo, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_GT),
                 bnNewLambdaFromCFunc(bone, bnStdDoubleGT, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_GE),
                 bnNewLambdaFromCFunc(bone, bnStdDoubleGE, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_LT),
                 bnNewLambdaFromCFunc(bone, bnStdDoubleLT, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_LE),
                 bnNewLambdaFromCFunc(bone, bnStdDoubleLE, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_EQUAL),
                 bnNewLambdaFromCFunc(bone, bnStdDoubleEqual, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_NOTEQUAL),
                 bnNewLambdaFromCFunc(bone, bnStdDoubleNotEqual, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, "toString"),
                 bnNewLambdaFromCFunc(bone, bnStdDoubleToString, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_RETURN,
                                      "ret", BN_C_ADD_EXIT));
        return ret;
}

static void bnStdDoubleFuncCall(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoublePositive(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleNegative(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleChilda(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleNot(bnInterpreter* bone, bnFrame* frame);

static void bnStdDoublePlus(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, "internal error");
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewDouble(bone, ai + bi));
}

static void bnStdDoubleMinus(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, "internal error");
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewDouble(bone, ai - bi));
}

static void bnStdDoubleMultiply(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, "internal error");
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewDouble(bone, ai * bi));
}

static void bnStdDoubleDivide(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, "internal error");
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewDouble(bone, ai / bi));
}

static void bnStdDoubleModulo(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, "internal error");
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewDouble(bone, fmod(ai, bi)));
}
static void bnStdDoubleBitAnd(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleBitOr(bnInterpreter* bone, bnFrame* frame);
// static void bnStdDoubleLogicAnd(bnInterpreter* bone, bnFrame* frame);
// static void bnStdDoubleLogicOr(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleExcOr(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleLShift(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleRShift(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleGT(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, "internal error");
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai > bi));
}
static void bnStdDoubleGE(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, "internal error");
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai >= bi));
}

static void bnStdDoubleLT(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, "internal error");
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai < bi));
}
static void bnStdDoubleLE(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, "internal error");
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai <= bi));
}
static void bnStdDoubleEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, "internal error");
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai == bi));
}
static void bnStdDoubleNotEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, "internal error");
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai == bi));
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
