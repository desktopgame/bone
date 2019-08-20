#include "string.h"
#include "char.h"
#include "frame.h"
#include "heap.h"
#include "integer.h"
#include "keyword.h"
#include "lambda.h"

#define _throw(bone, frame, fmt) (bnFormatThrow(bone, fmt))
#define message() ("should be parameter is string")

static void bnStdStringEqual(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringNotEqual(bnInterpreter* bone, bnFrame* frame);
static void bnStdStringAt(bnInterpreter* bone, bnFrame* frame);

/**
 * bnString is bone string.
 */
typedef struct bnString {
        bnObject base;
        bnStringView value;
} bnString;

bnReference bnNewString(bnInterpreter* bone, bnStringView value) {
        bnReference ref = bnAllocObject(bone->heap);
        bnString* ret = bnGetObject(bone->heap, ref);
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
        return ref;
}

bnReference bnNewString2(bnInterpreter* bone, const char* str) {
        return bnNewString(bone, bnIntern(bone->pool, str));
}

bnStringView bnGetStringValue(bnObject* obj) { return ((bnString*)obj)->value; }

// private

static void bnStdStringEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnObject* b = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (a->type != BN_OBJECT_STRING || b->type != BN_OBJECT_STRING) {
                _throw(bone, frame, message());
        }
        bnStringView ai = ((bnString*)a)->value;
        bnStringView bi = ((bnString*)b)->value;
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnGetBool(bone->pool, frame, ai == bi));
}

static void bnStdStringNotEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnObject* b = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (a->type != BN_OBJECT_STRING || b->type != BN_OBJECT_STRING) {
                _throw(bone, frame, message());
        }
        bnStringView ai = ((bnString*)a)->value;
        bnStringView bi = ((bnString*)b)->value;
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnGetBool(bone->pool, frame, ai != bi));
}

static void bnStdStringAt(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnObject* b = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (a->type != BN_OBJECT_STRING) {
                _throw(bone, frame, "should be `self` is string");
        }
        if (b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "should be `index` is string");
        }
        bnStringView ai = ((bnString*)a)->value;
        const char* astr = bnView2Str(bone->pool, ai);
        int astrlen = strlen(astr);
        int bv = bnGetIntegerValue(b);
        if (bv < 0 || bv >= astrlen) {
                bnFormatThrow(bone, "over index in bounds: %d~%d[%d]", 0,
                              astrlen, bv);
        }
        bnWriteVariable2(frame, bone->pool, "ret", bnNewChar(bone, astr[bv]));
}
