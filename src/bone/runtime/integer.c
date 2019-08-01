#include "integer.h"
#include "keyword.h"
#include "lambda.h"
#include "frame.h"

#define _throw(bone, frame, fmt) (bnFormatThrow(bone, fmt))

// Integer
static void bnStdIntegerFuncCall(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerPositive(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerNegative(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerChilda(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerNot(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerPlus(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerMinus(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerMultiply(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerDivide(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerModulo(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerBitAnd(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerBitOr(bnInterpreter* bone, bnFrame* frame);
// static void bnStdIntegerLogicAnd(bnInterpreter* bone, bnFrame* frame);
// static void bnStdIntegerLogicOr(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerExcOr(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerLShift(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerRShift(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerGT(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerGE(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerLT(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerLE(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerEqual(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerNotEqual(bnInterpreter* bone, bnFrame* frame);
static void bnStdIntegerToString(bnInterpreter* bone, bnFrame* frame);


bnInteger* bnNewInteger(bnInterpreter* bone, int value) {
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
        return ret;
}
// private
static void bnStdIntegerFuncCall(bnInterpreter* bone, bnFrame* frame) {
        _throw(bone, frame, "internal error");
}

static void bnStdIntegerPositive(bnInterpreter* bone, bnFrame* frame) {
        _throw(bone, frame, "internal error");
}

static void bnStdIntegerNegative(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "should be `self` is integer");
        }
        int ai = ((bnInteger*)a)->value;
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, -ai));
}

static void bnStdIntegerChilda(bnInterpreter* bone, bnFrame* frame) {
        _throw(bone, frame, "internal error");
}

static void bnStdIntegerNot(bnInterpreter* bone, bnFrame* frame) {
        _throw(bone, frame, "internal error");
}

static void bnStdIntegerPlus(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai + bi));
}

static void bnStdIntegerMinus(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai - bi));
}

static void bnStdIntegerMultiply(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai * bi));
}

static void bnStdIntegerDivide(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai / bi));
}

static void bnStdIntegerModulo(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai % bi));
}

static void bnStdIntegerBitAnd(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai & bi));
}

static void bnStdIntegerBitOr(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai | bi));
}

// static void bnStdIntegerLogicAnd(bnInterpreter* bone, bnFrame* frame){}

// static void bnStdIntegerLogicOr(bnInterpreter* bone, bnFrame* frame){}

static void bnStdIntegerExcOr(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdIntegerLShift(bnInterpreter* bone, bnFrame* frame) {
	bnObject* a = bnPopStack(frame->vStack);
	bnObject* b = bnPopStack(frame->vStack);
	if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
		bnFormatThrow(bone, "Integer#<< is required integer argument");
	}
	int ai = ((bnInteger*)a)->value;
	int bi = ((bnInteger*)b)->value;
	g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
		bnNewInteger(bone, ai << bi));
}

static void bnStdIntegerRShift(bnInterpreter* bone, bnFrame* frame) {
	bnObject* a = bnPopStack(frame->vStack);
	bnObject* b = bnPopStack(frame->vStack);
	if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
		bnFormatThrow(bone, "Integer#<< is required integer argument");
	}
	int ai = ((bnInteger*)a)->value;
	int bi = ((bnInteger*)b)->value;
	g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
		bnNewInteger(bone, ai >> bi));
}

static void bnStdIntegerGT(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai > bi));
}

static void bnStdIntegerGE(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai >= bi));
}

static void bnStdIntegerLT(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai < bi));
}

static void bnStdIntegerLE(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai <= bi));
}

static void bnStdIntegerEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai == bi));
}

static void bnStdIntegerNotEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai != bi));
}

static void bnStdIntegerToString(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        char buf[100];
        sprintf(buf, "%d", ((bnInteger*)a)->value);
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewString(bone, bnIntern(bone->pool, buf)));
}
