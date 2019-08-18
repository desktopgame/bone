#include "bool.h"
#include "frame.h"
#include "heap.h"
#include "integer.h"
#include "interpreter.h"
#include "keyword.h"
#include "lambda.h"

#define _throw(bone, frame, fmt) (bnFormatThrow(bone, fmt))
#define message() ("should be parameter is bool")

static void bnStdBoolNot(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolBitAnd(bnInterpreter* bone, bnFrame* frame);
static void bnStdBoolBitOr(bnInterpreter* bone, bnFrame* frame);

/**
 * bnBool is bool.
 */
typedef struct bnBool {
        bnObject base;
        bool value;
        bnReference r;
} bnBool;

bnReference bnNewBool(bnInterpreter* bone, bool value) {
        bnReference ref = bnAllocObject(bone->heap);
        bnBool* ret = bnGetObject(bone->heap, ref);
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
        return ref;
}

void bnSetFlipValue(struct bnHeap* heap, bnReference t, bnReference f) {
        bnBool* tobj = bnGetObject(heap, t);
        bnBool* fobj = bnGetObject(heap, f);
        tobj->r = f;
        fobj->r = t;
}

bnReference bnGetFlipValue(bnObject* obj) { return (((bnBool*)obj)->r); }

bool bnGetBoolValue(bnObject* obj) {
        if (obj->type == BN_OBJECT_BOOL) {
                return ((bnBool*)obj)->value;
        } else if (obj->type == BN_OBJECT_INTEGER) {
                return bnGetIntegerValue(obj) != 0;
        }
        return true;
}
// Bool

static void bnStdBoolNot(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (a->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, "should be `self` is bool");
        }
        bnBool* b = (bnBool*)a;
        bnWriteVariable2(frame, bone->pool, "ret", b->r);
}

static void bnStdBoolBitAnd(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnObject* b = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (a->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, message());
        }
        if (b->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, message());
        }
        bnBool* boolA = (bnBool*)a;
        bnBool* boolB = (bnBool*)b;
        bnReference c =
            bnReadVariable2(frame, bone->pool,
                            (boolA->value & boolB->value) ? "true" : "false");
        bnWriteVariable2(frame, bone->pool, "ret", c);
}

static void bnStdBoolBitOr(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnObject* b = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (a->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, message());
        }
        if (b->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, message());
        }
        bnBool* boolA = (bnBool*)a;
        bnBool* boolB = (bnBool*)b;
        bnReference c =
            bnReadVariable2(frame, bone->pool,
                            (boolA->value | boolB->value) ? "true" : "false");
        bnWriteVariable2(frame, bone->pool, "ret", c);
}