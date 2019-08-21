#include "string.h"
#include "array.h"
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
static void bnStdStringPlus(bnInterpreter* bone, bnFrame* frame);

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
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_PLUS),
                 bnNewLambdaFromCFunc(bone, bnStdStringPlus, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "other", BN_C_ADD_RETURN, "ret",
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

static void bnStdStringPlus(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnObject* b = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (a->type != BN_OBJECT_STRING) {
                _throw(bone, frame, "should be `self` is string");
        }
        if (b->type != BN_OBJECT_STRING) {
                _throw(bone, frame, "should be `other` is string");
        }
        //文字列をくっつける
        bnStringView ai = ((bnString*)a)->value;
        bnStringView bi = ((bnString*)b)->value;
        const char* astr = bnView2Str(bone->pool, ai);
        const char* bstr = bnView2Str(bone->pool, bi);
        int alen = strlen(astr);
        int blen = strlen(bstr);
        char newstr[alen + blen + 1];
        sprintf(newstr, "%s%s", astr, bstr);
        //配列を作成
        bnReference aryFunc = bnReadVariable2(frame, bone->pool, "array");
        bnPushStack(frame->vStack, bnNewInteger(bone, alen + blen));
        bnFrame* sub = bnFuncCall(aryFunc, bone, frame, 1);
        bnReference aryInst = bnStaging(bone->heap, bnReturnValue(frame));
        bnDeleteFrame(sub);
        //文字で埋める
        bnFillString(bone, newstr, aryInst);
        //文字を作成
        bnReference strFunc = bnReadVariable2(frame, bone->pool, "string");
        bnPushStack(frame->vStack, aryInst);
        sub = bnFuncCall(strFunc, bone, frame, 1);
        bnReference strInst = bnStaging(bone->heap, bnReturnValue(frame));
        bnDeleteFrame(sub);
        bnWriteVariable2(frame, bone->pool, "ret", strInst);
}