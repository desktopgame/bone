#include "integer.h"
#include "frame.h"
#include "keyword.h"
#include "lambda.h"
#include "string.h"

#define _throw(bone, frame, fmt) (bnFormatThrow(bone, fmt))
#define message() ("should be parameter is int")

// Integer
static void bnStdIntegerNegative(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerPlus(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerMinus(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerMultiply(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerDivide(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerModulo(bnInterpreter* bone, bnFrame* frame);
// static void bnStdIntegerLogicAnd(bnInterpreter* bone, bnFrame* frame);
// static void bnStdIntegerLogicOr(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerLShift(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerRShift(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerGT(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerGE(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerLT(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerLE(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerEqual(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerNotEqual(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerToString(bnInterpreter* bone, bnFrame* frame);
/**
 * bnInteger is bone integer.
 */
typedef struct bnInteger {
        bnObject base;
        int value;
} bnInteger;

bnObject* bnNewInteger(bnInterpreter* bone, int value) {
        bnInteger* ret = BN_MALLOC(sizeof(bnInteger));
        bnInitObject(bone, &ret->base, BN_OBJECT_INTEGER);
        ret->value = value;
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_PLUS),
                 bnNewLambdaFromCFunc(bone, bnStdIntegerPlus, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_MINUS),
                 bnNewLambdaFromCFunc(bone, bnStdIntegerMinus, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_MULTIPLY),
                 bnNewLambdaFromCFunc(bone, bnStdIntegerMultiply, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_DIVIDE),
                 bnNewLambdaFromCFunc(bone, bnStdIntegerDivide, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_MODULO),
                 bnNewLambdaFromCFunc(bone, bnStdIntegerModulo, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_LT),
                 bnNewLambdaFromCFunc(bone, bnStdIntegerLT, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_LE),
                 bnNewLambdaFromCFunc(bone, bnStdIntegerLE, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_GT),
                 bnNewLambdaFromCFunc(bone, bnStdIntegerGT, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_GE),
                 bnNewLambdaFromCFunc(bone, bnStdIntegerGE, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_EQUAL),
                 bnNewLambdaFromCFunc(bone, bnStdIntegerEqual, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_NOTEQUAL),
                 bnNewLambdaFromCFunc(bone, bnStdIntegerNotEqual, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_NEGATIVE),
                 bnNewLambdaFromCFunc(bone, bnStdIntegerNegative, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_RETURN,
                                      "ret", BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_LSHIFT),
                 bnNewLambdaFromCFunc(bone, bnStdIntegerLShift, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_RSHIFT),
                 bnNewLambdaFromCFunc(bone, bnStdIntegerRShift, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, "toString"),
                 bnNewLambdaFromCFunc(bone, bnStdIntegerToString, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_RETURN,
                                      "ret", BN_C_ADD_EXIT));
        return (bnObject*)ret;
}

int bnGetIntegerValue(bnObject* obj) { return ((bnInteger*)obj)->value; }
void bnSetIntegerValue(bnObject* obj, int value) {
        ((bnInteger*)obj)->value = value;
}

// private

static void bnStdIntegerNegative(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, message());
        }
        int ai = ((bnInteger*)a)->value;
        bnWriteVariable2(frame, bone->pool, "ret", bnNewInteger(bone, -ai));
}

static void bnStdIntegerPlus(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, message());
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        bnWriteVariable2(frame, bone->pool, "ret", bnNewInteger(bone, ai + bi));
}

static void bnStdIntegerMinus(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, message());
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        bnWriteVariable2(frame, bone->pool, "ret", bnNewInteger(bone, ai - bi));
}

static void bnStdIntegerMultiply(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, message());
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        bnWriteVariable2(frame, bone->pool, "ret", bnNewInteger(bone, ai * bi));
}

static void bnStdIntegerDivide(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, message());
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        bnWriteVariable2(frame, bone->pool, "ret", bnNewInteger(bone, ai / bi));
}

static void bnStdIntegerModulo(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, message());
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        bnWriteVariable2(frame, bone->pool, "ret", bnNewInteger(bone, ai % bi));
}

// static void bnStdIntegerLogicAnd(bnInterpreter* bone, bnFrame* frame){}

// static void bnStdIntegerLogicOr(bnInterpreter* bone, bnFrame* frame){}

static void bnStdIntegerLShift(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnFormatThrow(bone, message());
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnNewInteger(bone, ai << bi));
}

static void bnStdIntegerRShift(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                bnFormatThrow(bone, message());
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnNewInteger(bone, ai >> bi));
}

static void bnStdIntegerGT(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, message());
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi))
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnGetBool(bone->pool, frame, ai > bi));
}

static void bnStdIntegerGE(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, message());
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnGetBool(bone->pool, frame, ai >= bi));
}

static void bnStdIntegerLT(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, message());
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnGetBool(bone->pool, frame, ai < bi));
}

static void bnStdIntegerLE(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, message());
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnGetBool(bone->pool, frame, ai <= bi));
}

static void bnStdIntegerEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, message());
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnGetBool(bone->pool, frame, ai == bi));
}

static void bnStdIntegerNotEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, message());
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnGetBool(bone->pool, frame, ai != bi));
}

static void bnStdIntegerToString(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, message());
        }
        char buf[100];
        sprintf(buf, "%d", ((bnInteger*)a)->value);
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnNewString(bone, bnIntern(bone->pool, buf)));
}
