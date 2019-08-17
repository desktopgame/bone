#include "char.h"
#include "frame.h"
#include "heap.h"
#include "interpreter.h"
#include "keyword.h"
#include "lambda.h"

#define _throw(bone, frame, fmt) (bnFormatThrow(bone, fmt))
#define message() ("should be parameter is char")

static void bnStdCharEqual(bnInterpreter* bone, bnFrame* frame);
static void bnStdCharNotEqual(bnInterpreter* bone, bnFrame* frame);
/**
 * bnChar is char.
 */
typedef struct bnChar {
        bnObject base;
        char value;
} bnChar;

bnReference bnNewChar(bnInterpreter* bone, char value) {
        bnReference ref = bnAllocObject(bone->heap);
        bnChar* ret = bnGetObject(bone->heap, ref);
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
        return ref;
}

char bnGetCharValue(bnObject* obj) { return ((bnChar*)obj)->value; }

void bnSetCharValue(bnObject* obj, char c) { ((bnChar*)obj)->value = c; }

// Char

static void bnStdCharEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_CHAR || b->type != BN_OBJECT_CHAR) {
                _throw(bone, frame, message());
        }
        bnChar* aChar = (bnChar*)a;
        bnChar* bChar = (bnChar*)b;
        bnWriteVariable2(
            frame, bone->pool, "ret",
            bnGetBool(bone->pool, frame, aChar->value == bChar->value));
}

static void bnStdCharNotEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_CHAR || b->type != BN_OBJECT_CHAR) {
                _throw(bone, frame, message());
        }
        bnChar* aChar = (bnChar*)a;
        bnChar* bChar = (bnChar*)b;
        bnWriteVariable2(
            frame, bone->pool, "ret",
            bnGetBool(bone->pool, frame, aChar->value != bChar->value));
}
