#include "double.h"
#include <math.h>
#include <stdio.h>
#include "bool.h"
#include "enviroment.h"
#include "frame.h"
#include "heap.h"
#include "integer.h"
#include "interpreter.h"
#include "keyword.h"
#include "lambda.h"
#include "string.h"

#define _throw(bone, frame, fmt) (bnFormatThrow(bone, fmt))
#define message() ("should be parameter is double")

static void bnStdDoublePlus(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleMinus(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleMultiply(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleDivide(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleModulo(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleGT(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleGE(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleLT(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleLE(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleEqual(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleNotEqual(bnInterpreter* bone, bnFrame* frame);
static void bnStdDoubleToString(bnInterpreter* bone, bnFrame* frame);
/**
 * bnDouble is double.
 */
typedef struct bnDouble {
        bnObject base;
        double value;
} bnDouble;

bnReference bnNewDouble(bnInterpreter* bone, double value) {
        bnReference ref = bnAllocObject(bone->heap);
        bnDouble* ret = bnGetObject(bone->heap, ref);
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
        return ref;
}

void bnSetDoubleValue(bnObject* obj, double value) {
        ((bnDouble*)obj)->value = value;
}

double bnGetDoubleValue(bnObject* obj) { return ((bnDouble*)obj)->value; }

static void bnStdDoublePlus(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, message());
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        bnWriteVariable2(frame, bone->pool, "ret", bnNewDouble(bone, ai + bi));
}

static void bnStdDoubleMinus(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, message());
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        bnWriteVariable2(frame, bone->pool, "ret", bnNewDouble(bone, ai - bi));
}

static void bnStdDoubleMultiply(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, message());
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        bnWriteVariable2(frame, bone->pool, "ret", bnNewDouble(bone, ai * bi));
}

static void bnStdDoubleDivide(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, message());
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        bnWriteVariable2(frame, bone->pool, "ret", bnNewDouble(bone, ai / bi));
}

static void bnStdDoubleModulo(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, message());
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnNewDouble(bone, fmod(ai, bi)));
}

static void bnStdDoubleGT(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, message());
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnGetBool(bone->pool, frame, ai > bi));
}
static void bnStdDoubleGE(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, message());
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnGetBool(bone->pool, frame, ai >= bi));
}

static void bnStdDoubleLT(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, message());
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnGetBool(bone->pool, frame, ai < bi));
}
static void bnStdDoubleLE(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, message());
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnGetBool(bone->pool, frame, ai <= bi));
}
static void bnStdDoubleEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, message());
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnGetBool(bone->pool, frame, ai == bi));
}
static void bnStdDoubleNotEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE || b->type != BN_OBJECT_DOUBLE) {
                _throw(bone, frame, message());
        }
        double ai = ((bnDouble*)a)->value;
        double bi = ((bnDouble*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnGetBool(bone->pool, frame, ai == bi));
}

static void bnStdDoubleToString(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_DOUBLE) {
                bnFormatThrow(bone, message());
        }
        char buf[100];
        sprintf(buf, "%lf", ((bnDouble*)a)->value);
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnNewString(bone, bnIntern(bone->pool, buf)));
}
