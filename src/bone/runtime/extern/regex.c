#include "regex.h"
#include "../../glib.h"
#include "../any.h"
#include "../frame.h"
#include "../heap.h"
#include "../integer.h"
#include "../interpreter.h"
#include "../lambda.h"
#include "../string.h"

static bnReference bnNewRegex(bnInterpreter* bone, const char* pattern,
                              int compile_options, int match_options,
                              GError** err);
static void ext_compile(bnInterpreter* bone, bnFrame* frame);
static void ext_replace(bnInterpreter* bone, bnFrame* frame);
static void ext_match(bnInterpreter* bone, bnFrame* frame);

typedef struct bnRegex {
        bnAny base;
        GRegex* regex;
} bnRegex;

void bnExternRegex(bnInterpreter* bone) {
        // GRegexCompileFlagsを参照可能に
        bnWriteExtern2(bone, "regex.compile_flags.G_REGEX_CASELESS",
                       bnNewInteger(bone, G_REGEX_CASELESS));
        bnWriteExtern2(bone, "regex.compile_flags.G_REGEX_MULTILINE",
                       bnNewInteger(bone, G_REGEX_MULTILINE));
        bnWriteExtern2(bone, "regex.compile_flags.G_REGEX_DOTALL",
                       bnNewInteger(bone, G_REGEX_DOTALL));
        bnWriteExtern2(bone, "regex.compile_flags.G_REGEX_EXTENDED",
                       bnNewInteger(bone, G_REGEX_EXTENDED));
        bnWriteExtern2(bone, "regex.compile_flags.G_REGEX_ANCHORED",
                       bnNewInteger(bone, G_REGEX_ANCHORED));
        bnWriteExtern2(bone, "regex.compile_flags.G_REGEX_DOLLAR_ENDONLY",
                       bnNewInteger(bone, G_REGEX_DOLLAR_ENDONLY));
        bnWriteExtern2(bone, "regex.compile_flags.G_REGEX_UNGREEDY",
                       bnNewInteger(bone, G_REGEX_UNGREEDY));
        bnWriteExtern2(bone, "regex.compile_flags.G_REGEX_RAW",
                       bnNewInteger(bone, G_REGEX_RAW));
        bnWriteExtern2(bone, "regex.compile_flags.G_REGEX_NO_AUTO_CAPTURE",
                       bnNewInteger(bone, G_REGEX_NO_AUTO_CAPTURE));
        bnWriteExtern2(bone, "regex.compile_flags.G_REGEX_OPTIMIZE",
                       bnNewInteger(bone, G_REGEX_OPTIMIZE));
        bnWriteExtern2(bone, "regex.compile_flags.G_REGEX_FIRSTLINE",
                       bnNewInteger(bone, G_REGEX_FIRSTLINE));
        bnWriteExtern2(bone, "regex.compile_flags.G_REGEX_DUPNAMES",
                       bnNewInteger(bone, G_REGEX_DUPNAMES));
        bnWriteExtern2(bone, "regex.compile_flags.G_REGEX_NEWLINE_CR",
                       bnNewInteger(bone, G_REGEX_NEWLINE_CR));
        bnWriteExtern2(bone, "regex.compile_flags.G_REGEX_NEWLINE_LF",
                       bnNewInteger(bone, G_REGEX_NEWLINE_LF));
        bnWriteExtern2(bone, "regex.compile_flags.G_REGEX_NEWLINE_CRLF",
                       bnNewInteger(bone, G_REGEX_NEWLINE_CRLF));
        bnWriteExtern2(bone, "regex.compile_flags.G_REGEX_NEWLINE_ANYCRLF",
                       bnNewInteger(bone, G_REGEX_NEWLINE_ANYCRLF));
        bnWriteExtern2(bone, "regex.compile_flags.G_REGEX_BSR_ANYCRLF",
                       bnNewInteger(bone, G_REGEX_BSR_ANYCRLF));
        bnWriteExtern2(bone, "regex.compile_flags.G_REGEX_JAVASCRIPT_COMPAT",
                       bnNewInteger(bone, G_REGEX_JAVASCRIPT_COMPAT));
        // GRegexMatchFlagsを参照可能に
        bnWriteExtern2(bone, "regex.match_flags.G_REGEX_MATCH_ANCHORED",
                       bnNewInteger(bone, G_REGEX_MATCH_ANCHORED));
        bnWriteExtern2(bone, "regex.match_flags.G_REGEX_MATCH_NOTBOL",
                       bnNewInteger(bone, G_REGEX_MATCH_NOTBOL));
        bnWriteExtern2(bone, "regex.match_flags.G_REGEX_MATCH_NOTEOL",
                       bnNewInteger(bone, G_REGEX_MATCH_NOTEOL));
        bnWriteExtern2(bone, "regex.match_flags.G_REGEX_MATCH_NOTEMPTY",
                       bnNewInteger(bone, G_REGEX_MATCH_NOTEMPTY));
        bnWriteExtern2(bone, "regex.match_flags.G_REGEX_MATCH_PARTIAL",
                       bnNewInteger(bone, G_REGEX_MATCH_PARTIAL));
        bnWriteExtern2(bone, "regex.match_flags.G_REGEX_MATCH_NEWLINE_CR",
                       bnNewInteger(bone, G_REGEX_MATCH_NEWLINE_CR));
        bnWriteExtern2(bone, "regex.match_flags.G_REGEX_MATCH_NEWLINE_LF",
                       bnNewInteger(bone, G_REGEX_MATCH_NEWLINE_LF));
        bnWriteExtern2(bone, "regex.match_flags.G_REGEX_MATCH_NEWLINE_CRLF",
                       bnNewInteger(bone, G_REGEX_MATCH_NEWLINE_CRLF));
        bnWriteExtern2(bone, "regex.match_flags.G_REGEX_MATCH_NEWLINE_ANY",
                       bnNewInteger(bone, G_REGEX_MATCH_NEWLINE_ANY));
        bnWriteExtern2(bone, "regex.match_flags.G_REGEX_MATCH_NEWLINE_ANYCRLF",
                       bnNewInteger(bone, G_REGEX_MATCH_NEWLINE_ANYCRLF));
        bnWriteExtern2(bone, "regex.match_flags.G_REGEX_MATCH_BSR_ANYCRLF",
                       bnNewInteger(bone, G_REGEX_MATCH_BSR_ANYCRLF));
        bnWriteExtern2(bone, "regex.match_flags.G_REGEX_MATCH_BSR_ANY",
                       bnNewInteger(bone, G_REGEX_MATCH_BSR_ANY));
        bnWriteExtern2(bone, "regex.match_flags.G_REGEX_MATCH_PARTIAL_SOFT",
                       bnNewInteger(bone, G_REGEX_MATCH_PARTIAL_SOFT));
        bnWriteExtern2(bone, "regex.match_flags.G_REGEX_MATCH_PARTIAL_HARD",
                       bnNewInteger(bone, G_REGEX_MATCH_PARTIAL_HARD));
        bnWriteExtern2(bone, "regex.match_flags.G_REGEX_MATCH_NOTEMPTY_ATSTART",
                       bnNewInteger(bone, G_REGEX_MATCH_NOTEMPTY_ATSTART));
        //正規表現
        bnWriteExtern2(bone, "regex.compile",
                       bnNewLambdaFromCFunc(
                           bone, ext_compile, bone->pool, BN_C_ADD_PARAM,
                           "pattern", BN_C_ADD_PARAM, "compile_options",
                           BN_C_ADD_PARAM, "match_options", BN_C_ADD_RETURN,
                           "ret", BN_C_ADD_RETURN, "error", BN_C_ADD_EXIT));

        bnWriteExtern2(bone, "regex.replace",
                       bnNewLambdaFromCFunc(
                           bone, ext_replace, bone->pool, BN_C_ADD_PARAM,
                           "self", BN_C_ADD_PARAM, "source", BN_C_ADD_PARAM,
                           "start", BN_C_ADD_PARAM, "replacement",
                           BN_C_ADD_PARAM, "match_options", BN_C_ADD_RETURN,
                           "ret", BN_C_ADD_RETURN, "error", BN_C_ADD_EXIT));
        bnWriteExtern2(
            bone, "regex.match",
            bnNewLambdaFromCFunc(
                bone, ext_match, bone->pool, BN_C_ADD_PARAM, "self",
                BN_C_ADD_PARAM, "source", BN_C_ADD_PARAM, "match_options",
                BN_C_ADD_PARAM, "func", BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
}

static bnReference bnNewRegex(bnInterpreter* bone, const char* pattern,
                              int compile_options, int match_options,
                              GError** err) {
        GRegex* gregex =
            g_regex_new(pattern, compile_options, match_options, err);
        if (gregex == NULL) {
                return NULL;
        }
        bnReference ref = bnAllocObject(bone->heap);
        bnRegex* regex = bnGetObject(bone->heap, ref);
        bnInitAny(bone, &regex->base, REGEX_T);
        regex->regex = gregex;
        return ref;
}

static void ext_compile(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnObject* b = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnObject* c = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (a->type != BN_OBJECT_STRING) {
                bnFormatThrow(bone, "should be `pattern` is string");
        }
        if (b->type != BN_OBJECT_INTEGER) {
                bnFormatThrow(bone, "should be `compile_option` is integer");
        }
        if (c->type != BN_OBJECT_INTEGER) {
                bnFormatThrow(bone, "should be `match_option` is integer");
        }
        const char* pattern = bnView2Str(bone->pool, bnGetStringValue(a));
        int compile_options = bnGetIntegerValue(b);
        int match_options = bnGetIntegerValue(c);
        GError* err = NULL;
        bnReference ref =
            bnNewRegex(bone, pattern, compile_options, match_options, &err);
        bnWriteVariable2(frame, bone->pool, "error",
                         bnGetFalse(bone->pool, frame));
        if (ref == NULL) {
                bnWriteVariable2(frame, bone->pool, "error",
                                 bnNewString2(bone, "illegal pattern"));
                bnWriteVariable2(frame, bone->pool, "ret",
                                 bnGetFalse(bone->pool, frame));
                return;
        }
        bnWriteVariable2(frame, bone->pool, "ret", ref);
}

static void ext_replace(bnInterpreter* bone, bnFrame* frame) {
        // self
        // source
        // start
        // replacement
        // match_options
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnObject* b = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnObject* c = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnObject* d = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnObject* e = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (a->type != BN_OBJECT_ANY || !bnMatchType(bone, a, REGEX_T)) {
                bnFormatThrow(bone, "should be `self` is regex");
        }
        if (b->type != BN_OBJECT_STRING) {
                bnFormatThrow(bone, "should be `source` is string");
        }
        if (c->type != BN_OBJECT_INTEGER) {
                bnFormatThrow(bone, "should be `start` is integer");
        }
        if (d->type != BN_OBJECT_STRING) {
                bnFormatThrow(bone, "should be `replacement` is string");
        }
        if (e->type != BN_OBJECT_INTEGER) {
                bnFormatThrow(bone, "should be `match_options` is integer");
        }
        bnRegex* regex = (bnRegex*)a;
        const char* source = bnView2Str(bone->pool, bnGetStringValue(b));
        int start = bnGetIntegerValue(c);
        const char* replacement = bnView2Str(bone->pool, bnGetStringValue(d));
        int match_options = bnGetIntegerValue(e);
        GError* err = NULL;
        bnWriteVariable2(frame, bone->pool, "error",
                         bnGetFalse(bone->pool, frame));
        gchar* result = g_regex_replace(regex->regex, source, -1, start,
                                        replacement, match_options, &err);
        if (result == NULL || err != NULL) {
                bnWriteVariable2(frame, bone->pool, "error",
                                 bnNewString2(bone, err->message));
                bnWriteVariable2(frame, bone->pool, "ret",
                                 bnGetFalse(bone->pool, frame));
                return;
        }
        bnWriteVariable2(frame, bone->pool, "ret", bnNewString2(bone, result));
        g_free(result);
}

static void ext_match(bnInterpreter* bone, bnFrame* frame) {
        // self
        // source
        // match_options
        // func
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnObject* b = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnObject* c = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnReference dref = bnPopStack(frame->vStack);
        bnObject* d = bnGetObject(bone->heap, dref);

        if (a->type != BN_OBJECT_ANY || !bnMatchType(bone, a, REGEX_T)) {
                bnFormatThrow(bone, "should be `self` is regex");
        }
        if (b->type != BN_OBJECT_STRING) {
                bnFormatThrow(bone, "should be `source` is string");
        }
        if (c->type != BN_OBJECT_INTEGER) {
                bnFormatThrow(bone, "should be `match_options` is integer");
        }
        if (d->type != BN_OBJECT_LAMBDA) {
                bnFormatThrow(bone, "should be `func` is lambda");
        }
        bnRegex* regex = (bnRegex*)a;
        const char* source = bnView2Str(bone->pool, bnGetStringValue(b));
        int match_options = bnGetIntegerValue(c);
        GMatchInfo* info;
        gboolean matches =
            g_regex_match(regex->regex, source, match_options, &info);
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnGetBool(bone->pool, frame, matches));
        while (g_match_info_matches(info)) {
                gchar* word = g_match_info_fetch(info, 0);
                int start, end;
                g_match_info_fetch_pos(info, 0, &start, &end);
                bnPushStack(frame->vStack, bnNewString2(bone, word));
                bnPushStack(frame->vStack, bnNewInteger(bone, start));
                bnPushStack(frame->vStack, bnNewInteger(bone, end));
                bnFrame* sub = bnFuncCall(dref, bone, frame, 3);
                bnDeleteFrame(sub);
                g_free(word);
                g_match_info_next(info, NULL);
        }
        g_match_info_free(info);
}