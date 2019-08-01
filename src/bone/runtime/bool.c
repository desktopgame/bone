#include "bool.h"
#include "frame.h"
#include "interpreter.h"
#include "keyword.h"
#include "lambda.h"

#define _throw(bone, frame, fmt) (bnFormatThrow(bone, fmt))

static void bnStdBoolFuncCall(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolPositive(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolNegative(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolChilda(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolNot(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolPlus(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolMinus(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolMultiply(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolDivide(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolModulo(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolBitAnd(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolBitOr(bnInterpreter* bone, bnFrame* frame);
// static void bnStdBoolLogicAnd(bnInterpreter* bone, bnFrame* frame);
// static void bnStdBoolLogicOr(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolExcOr(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolLShift(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolRShift(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolGT(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolGE(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolLT(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolLE(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolEqual(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolNotEqual(bnInterpreter* bone, bnFrame* frame);

bnBool* bnNewBool(bnInterpreter* bone, bool value) {
        bnBool* ret = BN_MALLOC(sizeof(bnBool));
        bnInitObject(bone, &ret->base, BN_OBJECT_BOOL);
        ret->value = value;
        ret->r = NULL;
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_NOT),
                 bnNewLambdaFromCFunc(bone, bnStdBoolNot, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_RETURN,
                                      "ret", BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_BIT_OR),
                 bnNewLambdaFromCFunc(bone, bnStdBoolBitOr, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_BIT_AND),
                 bnNewLambdaFromCFunc(bone, bnStdBoolBitAnd, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        return ret;
}
// Bool

static void bnStdBoolFuncCall(bnInterpreter* bone, bnFrame* frame);

static void bnStdBoolPositive(bnInterpreter* bone, bnFrame* frame);

static void bnStdBoolNegative(bnInterpreter* bone, bnFrame* frame);

static void bnStdBoolChilda(bnInterpreter* bone, bnFrame* frame);

static void bnStdBoolNot(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, "should be `self` is bool");
        }
        bnBool* b = a;
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             b->r);
}

static void bnStdBoolPlus(bnInterpreter* bone, bnFrame* frame);

static void bnStdBoolMinus(bnInterpreter* bone, bnFrame* frame);

static void bnStdBoolMultiply(bnInterpreter* bone, bnFrame* frame);

static void bnStdBoolDivide(bnInterpreter* bone, bnFrame* frame);

static void bnStdBoolModulo(bnInterpreter* bone, bnFrame* frame);

static void bnStdBoolBitAnd(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, "should be `self` is bool");
        }
        if (b->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, "should be argument is bool");
        }
        bnBool* boolA = a;
        bnBool* boolB = b;
        bnObject* c = g_hash_table_lookup(
            frame->variableTable,
            bnIntern(bone->pool,
                     (boolA->value & boolB->value) ? "true" : "false"));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             c);
}

static void bnStdBoolBitOr(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, "should be `self` is bool");
        }
        if (b->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, "should be argument is bool");
        }
        bnBool* boolA = a;
        bnBool* boolB = b;
        bnObject* c = g_hash_table_lookup(
            frame->variableTable,
            bnIntern(bone->pool,
                     (boolA->value | boolB->value) ? "true" : "false"));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             c);
}

// static void bnStdBoolLogicAnd(struct bnInterpreter* bone, struct bnFrame*
// frame);

// static void bnStdBoolLogicOr(struct bnInterpreter* bone, struct bnFrame*
// frame);

static void bnStdBoolExcOr(bnInterpreter* bone, bnFrame* frame);

static void bnStdBoolLShift(bnInterpreter* bone, bnFrame* frame);

static void bnStdBoolRShift(bnInterpreter* bone, bnFrame* frame);

static void bnStdBoolGT(bnInterpreter* bone, bnFrame* frame);

static void bnStdBoolGE(bnInterpreter* bone, bnFrame* frame);

static void bnStdBoolLT(bnInterpreter* bone, bnFrame* frame);

static void bnStdBoolLE(bnInterpreter* bone, bnFrame* frame);

static void bnStdBoolEqual(bnInterpreter* bone, bnFrame* frame);

static void bnStdBoolNotEqual(bnInterpreter* bone, bnFrame* frame);
