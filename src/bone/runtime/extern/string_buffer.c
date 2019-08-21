#include "string_buffer.h"
#include "../../glib.h"
#include "../any.h"
#include "../char.h"
#include "../frame.h"
#include "../heap.h"
#include "../integer.h"
#include "../interpreter.h"
#include "../lambda.h"
#include "../string.h"

static void ext_new_string_buffer(bnInterpreter* bone, bnFrame* frame);
static void ext_append_c(bnInterpreter* bone, bnFrame* frame);
static void ext_append_str(bnInterpreter* bone, bnFrame* frame);
static void ext_erase(bnInterpreter* bone, bnFrame* frame);
static void ext_length(bnInterpreter* bone, bnFrame* frame);
static void ext_set(bnInterpreter* bone, bnFrame* frame);
static void ext_get(bnInterpreter* bone, bnFrame* frame);
static void ext_to_string(bnInterpreter* bone, bnFrame* frame);

typedef struct bnStringBuffer {
        bnAny base;
        GString* str;
} bnStringBuffer;

void bnExternStringBuffer(bnInterpreter* bone) {
        bnWriteExtern2(
            bone, "string_buffer.new",
            bnNewLambdaFromCFunc(bone, ext_new_string_buffer, bone->pool,
                                 BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        bnWriteExtern2(
            bone, "string_buffer.append_c",
            bnNewLambdaFromCFunc(bone, ext_append_c, bone->pool, BN_C_ADD_PARAM,
                                 "self", BN_C_ADD_PARAM, "c", BN_C_ADD_RETURN,
                                 "ret", BN_C_ADD_EXIT));
        bnWriteExtern2(
            bone, "string_buffer.append_str",
            bnNewLambdaFromCFunc(bone, ext_append_str, bone->pool,
                                 BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM, "str",
                                 BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        bnWriteExtern2(bone, "string_buffer.erase",
                       bnNewLambdaFromCFunc(
                           bone, ext_erase, bone->pool, BN_C_ADD_PARAM, "self",
                           BN_C_ADD_PARAM, "offset", BN_C_ADD_PARAM, "length",
                           BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        bnWriteExtern2(bone, "string_buffer.length",
                       bnNewLambdaFromCFunc(
                           bone, ext_length, bone->pool, BN_C_ADD_PARAM, "self",
                           BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        bnWriteExtern2(bone, "string_buffer.set",
                       bnNewLambdaFromCFunc(
                           bone, ext_set, bone->pool, BN_C_ADD_PARAM, "self",
                           BN_C_ADD_PARAM, "index", BN_C_ADD_PARAM, "c",
                           BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        bnWriteExtern2(
            bone, "string_buffer.get",
            bnNewLambdaFromCFunc(bone, ext_get, bone->pool, BN_C_ADD_PARAM,
                                 "self", BN_C_ADD_PARAM, "index",
                                 BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        bnWriteExtern2(bone, "string_buffer.to_string",
                       bnNewLambdaFromCFunc(
                           bone, ext_to_string, bone->pool, BN_C_ADD_PARAM,
                           "self", BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
}

static bnReference bnNewStringBuffer(bnInterpreter* bone) {
        bnReference ref = bnAllocObject(bone->heap);
        bnStringBuffer* buff = bnGetObject(bone->heap, ref);
        bnInitAny(bone, &buff->base, STRING_BUFFER_T);
        buff->str = g_string_new(NULL);
        return ref;
}

static void ext_new_string_buffer(bnInterpreter* bone, bnFrame* frame) {
        bnWriteVariable2(frame, bone->pool, "ret", bnNewStringBuffer(bone));
}

static void ext_append_c(bnInterpreter* bone, bnFrame* frame) {
        bnReference aref = bnPopStack(frame->vStack);
        bnObject* a = bnGetObject(bone->heap, aref);
        bnObject* b = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (a->type != BN_OBJECT_ANY ||
            !bnMatchType(bone, a, STRING_BUFFER_T)) {
                bnFormatThrow(bone, "should be `self` is string_buffer");
        }
        if (b->type != BN_OBJECT_CHAR) {
                bnFormatThrow(bone, "should be `c` is char");
        }
        bnStringBuffer* abuf = (bnStringBuffer*)a;
        bnGStringAppendC(abuf->str, bnGetCharValue(b));
        bnWriteVariable2(frame, bone->pool, "ret", aref);
}

static void ext_append_str(bnInterpreter* bone, bnFrame* frame) {
        bnReference aref = bnPopStack(frame->vStack);
        bnObject* a = bnGetObject(bone->heap, aref);
        bnObject* b = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (a->type != BN_OBJECT_ANY ||
            !bnMatchType(bone, a, STRING_BUFFER_T)) {
                bnFormatThrow(bone, "should be `self` is string_buffer");
        }
        if (b->type != BN_OBJECT_STRING) {
                bnFormatThrow(bone, "should be `str` is string");
        }
        bnStringBuffer* abuf = (bnStringBuffer*)a;
        g_string_append(abuf->str, bnView2Str(bone->pool, bnGetStringValue(b)));
        bnWriteVariable2(frame, bone->pool, "ret", aref);
}

static void ext_erase(bnInterpreter* bone, bnFrame* frame) {
        bnReference aref = bnPopStack(frame->vStack);
        bnObject* a = bnGetObject(bone->heap, aref);
        bnObject* b = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnObject* c = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (a->type != BN_OBJECT_ANY ||
            !bnMatchType(bone, a, STRING_BUFFER_T)) {
                bnFormatThrow(bone, "should be `self` is string_buffer");
        }
        if (b->type != BN_OBJECT_INTEGER) {
                bnFormatThrow(bone, "should be `offset` is integer");
        }
        if (c->type != BN_OBJECT_INTEGER) {
                bnFormatThrow(bone, "should be `length` is integer");
        }
        //境界をチェック
        bnStringBuffer* abuf = (bnStringBuffer*)a;
        int offset = bnGetIntegerValue(b);
        int length = bnGetIntegerValue(c);
        if (offset < 0) {
                bnFormatThrow(bone, "should be `offset` is greater than zero");
        }
        if (offset + length >= abuf->str->len) {
                bnFormatThrow(bone,
                              "should be `offset` + `length` is less than "
                              "length of string");
        }
        g_string_erase(abuf->str, offset, length);
        bnWriteVariable2(frame, bone->pool, "ret", aref);
}

static void ext_length(bnInterpreter* bone, bnFrame* frame) {
        bnReference aref = bnPopStack(frame->vStack);
        bnObject* a = bnGetObject(bone->heap, aref);
        if (a->type != BN_OBJECT_ANY ||
            !bnMatchType(bone, a, STRING_BUFFER_T)) {
                bnFormatThrow(bone, "should be `self` is string_buffer");
        }
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnNewInteger(bone, ((bnStringBuffer*)a)->str->len));
}

static void ext_set(bnInterpreter* bone, bnFrame* frame) {
        bnReference aref = bnPopStack(frame->vStack);
        bnObject* a = bnGetObject(bone->heap, aref);
        bnObject* b = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnObject* c = bnGetObject(bone->heap, bnPopStack(frame->vStack));

        if (a->type != BN_OBJECT_ANY ||
            !bnMatchType(bone, a, STRING_BUFFER_T)) {
                bnFormatThrow(bone, "should be `self` is string_buffer");
        }
        if (b->type != BN_OBJECT_INTEGER) {
                bnFormatThrow(bone, "should be `index` is integer");
        }
        if (c->type != BN_OBJECT_INTEGER) {
                bnFormatThrow(bone, "should be `c` is integer");
        }
        int bint = bnGetIntegerValue(b);
        if (bint < 0 || bint >= ((bnStringBuffer*)a)->str->len) {
                bnFormatThrow(bone, "internal error");
        }
        ((bnStringBuffer*)a)->str->str[bint] = bnGetCharValue(c);
}

static void ext_get(bnInterpreter* bone, bnFrame* frame) {
        bnReference aref = bnPopStack(frame->vStack);
        bnObject* a = bnGetObject(bone->heap, aref);
        bnObject* b = bnGetObject(bone->heap, bnPopStack(frame->vStack));

        if (a->type != BN_OBJECT_ANY ||
            !bnMatchType(bone, a, STRING_BUFFER_T)) {
                bnFormatThrow(bone, "should be `self` is string_buffer");
        }
        if (b->type != BN_OBJECT_INTEGER) {
                bnFormatThrow(bone, "should be `index` is integer");
        }
        int bint = bnGetIntegerValue(b);
        if (bint < 0 || bint >= ((bnStringBuffer*)a)->str->len) {
                bnFormatThrow(bone, "internal error");
        }
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnNewChar(bone, ((bnStringBuffer*)a)->str->str[bint]));
}

static void ext_to_string(bnInterpreter* bone, bnFrame* frame) {
        bnReference aref = bnPopStack(frame->vStack);
        bnObject* a = bnGetObject(bone->heap, aref);
        if (a->type != BN_OBJECT_ANY ||
            !bnMatchType(bone, a, STRING_BUFFER_T)) {
                bnFormatThrow(bone, "should be `self` is string_buffer");
        }
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnNewString2(bone, ((bnStringBuffer*)a)->str->str));
}