#include "string.h"
#include "char.h"
#include "frame.h"
#include "integer.h"
#include "keyword.h"
#include "lambda.h"

#define _throw(bone, frame, fmt) (bnFormatThrow(bone, fmt))
#define message() ("should be parameter is string")

static void bnStdStringFuncCall(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringPositive(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringNegative(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringChilda(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringNot(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringPlus(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringMinus(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringMultiply(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringDivide(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringModulo(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringBitAnd(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringBitOr(bnInterpreter* bone, bnFrame* frame);
// static void bnStdStringLogicAnd(bnInterpreter* bone, bnFrame* frame);
// static void bnStdStringLogicOr(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringExcOr(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringLShift(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringRShift(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringGT(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringGE(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringLT(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringLE(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringEqual(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringNotEqual(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringAt(bnInterpreter* bone, bnFrame* frame);

bnString* bnNewString(bnInterpreter* bone, bnStringView value) {
        bnString* ret = BN_MALLOC(sizeof(bnString));
        bnInitObject(bone, &ret->base, BN_OBJECT_STRING);
        ret->value = value;
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_EQUAL),
                 bnNewLambdaFromCFunc(bone, bnStdStringEqual, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_NOTEQUAL),
                 bnNewLambdaFromCFunc(bone, bnStdStringNotEqual, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, "length"),
                 bnNewInteger(bone, strlen(bnView2Str(bone->pool, value))));
        bnDefine(&ret->base, bnIntern(bone->pool, "at"),
                 bnNewLambdaFromCFunc(bone, bnStdStringAt, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "index", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        return ret;
}

bnString* bnNewString2(bnInterpreter* bone, const char* str) {
        return bnNewString(bone, bnIntern(bone->pool, str));
}

// String

static void bnStdStringFuncCall(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdStringPositive(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdStringNegative(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdStringChilda(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdStringNot(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdStringPlus(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdStringMinus(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdStringMultiply(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdStringDivide(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdStringModulo(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdStringBitAnd(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdStringBitOr(bnInterpreter* bone, bnFrame* frame) {}

// static void bnStdStringLogicAnd(bnInterpreter* bone, bnFrame* frame){}

// static void bnStdStringLogicOr(bnInterpreter* bone, bnFrame* frame){}

static void bnStdStringExcOr(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdStringLShift(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdStringRShift(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdStringGT(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdStringGE(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdStringLT(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdStringLE(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdStringEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_STRING || b->type != BN_OBJECT_STRING) {
                _throw(bone, frame, message());
        }
        bnStringView ai = ((bnString*)a)->value;
        bnStringView bi = ((bnString*)b)->value;
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai == bi));
}

static void bnStdStringNotEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_STRING || b->type != BN_OBJECT_STRING) {
                _throw(bone, frame, message());
        }
        bnStringView ai = ((bnString*)a)->value;
        bnStringView bi = ((bnString*)b)->value;
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai != bi));
}

static void bnStdStringAt(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_STRING) {
                _throw(bone, frame, "should be `self` is string");
        }
        if (b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "should be `index` is string");
        }
        bnStringView ai = ((bnString*)a)->value;
        bnInteger* bi = b;
        const char* astr = bnView2Str(bone->pool, ai);
        int astrlen = strlen(astr);
        if (bi->value < 0 || bi->value >= astrlen) {
                bnFormatThrow(bone, "over index in bounds: %d~%d[%d]", 0,
                              astrlen, bi->value);
        }
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewChar(bone, astr[bi->value]));
}
