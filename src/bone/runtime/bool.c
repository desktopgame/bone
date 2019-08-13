#include "bool.h"
#include "frame.h"
#include "interpreter.h"
#include "keyword.h"
#include "lambda.h"

#define _throw(bone, frame, fmt) (bnFormatThrow(bone, fmt))
#define message() ("should be parameter is bool")

static void bnStdBoolNot(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolBitAnd(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolBitOr(bnInterpreter* bone, bnFrame* frame);

static void bnStdBoolEqual(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolNotEqual(bnInterpreter* bone, bnFrame* frame);

/**
 * bnBool is bool.
 */
typedef struct bnBool {
        bnObject base;
        bool value;
        struct bnBool* r;
} bnBool;

bnObject* bnNewBool(bnInterpreter* bone, bool value) {
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
        return (bnObject*)ret;
}

void bnSetFlipValue(bnObject* t, bnObject* f) {
        bnBool* bt = (bnBool*)t;
        bnBool* bf = (bnBool*)f;
        bt->r = bf;
        bf->r = bt;
}

bnObject* bnGetFlipValue(bnObject* obj) { return ((bnBool*)obj)->r; }

bool bnGetBoolValue(bnObject* obj) { return ((bnBool*)obj)->value; }
// Bool

static void bnStdBoolNot(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, "should be `self` is bool");
        }
        bnBool* b = a;
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             b->r);
}

static void bnStdBoolBitAnd(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, message());
        }
        if (b->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, message());
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
                _throw(bone, frame, message());
        }
        if (b->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, message());
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

static void bnStdBoolEqual(bnInterpreter* bone, bnFrame* frame);

static void bnStdBoolNotEqual(bnInterpreter* bone, bnFrame* frame);
