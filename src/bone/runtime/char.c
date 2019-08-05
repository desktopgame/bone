#include "char.h"
#include "frame.h"
#include "interpreter.h"
#include "keyword.h"
#include "lambda.h"

#define _throw(bone, frame, fmt) (bnFormatThrow(bone, fmt))
#define message() ("should be parameter is char")

static void bnStdCharFuncCall(bnInterpreter* bone, bnFrame* frame);
static void bnStdCharPositive(bnInterpreter* bone, bnFrame* frame);
static void bnStdCharNegative(bnInterpreter* bone, bnFrame* frame);
static void bnStdCharChilda(bnInterpreter* bone, bnFrame* frame);
static void bnStdCharNot(bnInterpreter* bone, bnFrame* frame);
static void bnStdCharPlus(bnInterpreter* bone, bnFrame* frame);
static void bnStdCharMinus(bnInterpreter* bone, bnFrame* frame);
static void bnStdCharMultiply(bnInterpreter* bone, bnFrame* frame);
static void bnStdCharDivide(bnInterpreter* bone, bnFrame* frame);
static void bnStdCharModulo(bnInterpreter* bone, bnFrame* frame);
static void bnStdCharBitAnd(bnInterpreter* bone, bnFrame* frame);
static void bnStdCharBitOr(bnInterpreter* bone, bnFrame* frame);
// static void bnStdCharLogicAnd(bnInterpreter* bone, bnFrame* frame);
// static void bnStdCharLogicOr(bnInterpreter* bone, bnFrame* frame);
static void bnStdCharExcOr(bnInterpreter* bone, bnFrame* frame);
static void bnStdCharLShift(bnInterpreter* bone, bnFrame* frame);
static void bnStdCharRShift(bnInterpreter* bone, bnFrame* frame);
static void bnStdCharGT(bnInterpreter* bone, bnFrame* frame);
static void bnStdCharGE(bnInterpreter* bone, bnFrame* frame);
static void bnStdCharLT(bnInterpreter* bone, bnFrame* frame);
static void bnStdCharLE(bnInterpreter* bone, bnFrame* frame);
static void bnStdCharEqual(bnInterpreter* bone, bnFrame* frame);
static void bnStdCharNotEqual(bnInterpreter* bone, bnFrame* frame);

bnChar* bnNewChar(bnInterpreter* bone, char value) {
        bnChar* ret = BN_MALLOC(sizeof(bnChar));
        bnInitObject(bone, &ret->base, BN_OBJECT_CHAR);
        ret->value = value;
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_EQUAL),
                 bnNewLambdaFromCFunc(bone, bnStdCharEqual, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_NOTEQUAL),
                 bnNewLambdaFromCFunc(bone, bnStdCharNotEqual, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        return ret;
}

// Char

static void bnStdCharFuncCall(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdCharPositive(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdCharNegative(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdCharChilda(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdCharNot(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdCharPlus(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdCharMinus(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdCharMultiply(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdCharDivide(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdCharModulo(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdCharBitAnd(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdCharBitOr(bnInterpreter* bone, bnFrame* frame) {}

// static void bnStdCharLogicAnd(bnInterpreter* bone, bnFrame* frame){}

// static void bnStdCharLogicOr(bnInterpreter* bone, bnFrame* frame){}

static void bnStdCharExcOr(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdCharLShift(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdCharRShift(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdCharGT(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdCharGE(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdCharLT(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdCharLE(bnInterpreter* bone, bnFrame* frame) {}

static void bnStdCharEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_CHAR || b->type != BN_OBJECT_CHAR) {
                _throw(bone, frame, message());
        }
        bnChar* aChar = a;
        bnChar* bChar = b;
        g_hash_table_replace(
            frame->variableTable, bnIntern(bone->pool, "ret"),
            bnGetBool(bone->pool, frame, aChar->value == bChar->value));
}

static void bnStdCharNotEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_CHAR || b->type != BN_OBJECT_CHAR) {
                _throw(bone, frame, message());
        }
        bnChar* aChar = a;
        bnChar* bChar = b;
        g_hash_table_replace(
            frame->variableTable, bnIntern(bone->pool, "ret"),
            bnGetBool(bone->pool, frame, aChar->value != bChar->value));
}
