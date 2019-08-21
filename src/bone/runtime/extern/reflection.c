#include "reflection.h"
#include "../../bone.h"
#include "../array.h"
#include "../char.h"
#include "../frame.h"
#include "../heap.h"
#include "../integer.h"
#include "../interpreter.h"
#include "../lambda.h"
#include "../string.h"

static void bnExtReflectionDefine(struct bnInterpreter* bone,
                                  struct bnFrame* frame);
static void bnExtReflectionUndef(struct bnInterpreter* bone,
                                 struct bnFrame* frame);
static void bnExtReflectionDefined(struct bnInterpreter* bone,
                                   struct bnFrame* frame);
static void bnExtReflectionExpand(struct bnInterpreter* bone,
                                  struct bnFrame* frame);
static void bnExtReflectionEntries(struct bnInterpreter* bone,
                                   struct bnFrame* frame);
static void bnExtReflectionExport(struct bnInterpreter* bone,
                                  struct bnFrame* frame);

void bnExternReflection(bnInterpreter* bone) {
        bnWriteExtern2(
            bone, "define",
            bnNewLambdaFromCFunc(bone, bnExtReflectionDefine, bone->pool,
                                 BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM, "name",
                                 BN_C_ADD_PARAM, "value", BN_C_ADD_EXIT));
        bnWriteExtern2(
            bone, "undef",
            bnNewLambdaFromCFunc(bone, bnExtReflectionUndef, bone->pool,
                                 BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM, "name",
                                 BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        bnWriteExtern2(
            bone, "defined",
            bnNewLambdaFromCFunc(bone, bnExtReflectionDefined, bone->pool,
                                 BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM, "name",
                                 BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        bnWriteExtern2(
            bone, "expand",
            bnNewLambdaFromCFunc(bone, bnExtReflectionExpand, bone->pool,
                                 BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM, "name",
                                 BN_C_ADD_RETURN, "ret", BN_C_ADD_RETURN,
                                 "error", BN_C_ADD_EXIT));
        bnWriteExtern2(
            bone, "entries",
            bnNewLambdaFromCFunc(bone, bnExtReflectionEntries, bone->pool,
                                 BN_C_ADD_PARAM, "self", BN_C_ADD_RETURN, "ret",
                                 BN_C_ADD_EXIT));
        bnWriteExtern2(
            bone, "export",
            bnNewLambdaFromCFunc(bone, bnExtReflectionExport, bone->pool,
                                 BN_C_ADD_PARAM, "self", BN_C_ADD_RETURN, "...",
                                 BN_C_ADD_EXIT));
}

static void bnExtReflectionDefine(bnInterpreter* bone, bnFrame* frame) {
        bnReference a = bnPopStack(frame->vStack);
        bnPopStringArg(bone, frame, name);
        bnReference c = bnPopStack(frame->vStack);
        bnObject* aObj = bnGetObject(bone->heap, a);
        bnDefine(aObj, bnGetStringValue(nameObj), c);
}

static void bnExtReflectionUndef(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnPopStringArg(bone, frame, name);
        bool removed = bnUndef(a, bnGetStringValue(nameObj));
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnGetBool(bone->pool, frame, removed));
}

static void bnExtReflectionDefined(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnPopStringArg(bone, frame, name);
        bool contains = bnDefined(a, bnGetStringValue(nameObj));
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnGetBool(bone->pool, frame, contains));
}

static void bnExtReflectionExpand(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnPopStringArg(bone, frame, name);
        bool contains = bnDefined(a, bnGetStringValue(nameObj));
        bnWriteVariable2(frame, bone->pool, "error",
                         bnGetBool(bone->pool, frame, !contains));
        if (contains) {
                bnWriteVariable2(frame, bone->pool, "ret",
                                 bnLookup(a, bnGetStringValue(nameObj)));
        }
}

static void bnExtReflectionEntries(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        GList* entries = NULL;
        GHashTableIter hashIter;
        gpointer k, v;
        g_hash_table_iter_init(&hashIter, a->table);
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                const char* kstr = bnView2Str(bone->pool, (bnStringView)k);
                if (*kstr == '$') {
                        continue;
                }
                entries = g_list_append(entries, k);
        }
        bnReference arrayFuncRef = bnReadVariable2(frame, bone->pool, "array");
        bnPushStack(frame->vStack, bnNewInteger(bone, g_list_length(entries)));
        bnFrame* sub = bnFuncCall(arrayFuncRef, bone, frame, 1);
        bnDeleteFrame(sub);
        bnReference arrayInstRef = bnPopStack(frame->vStack);
        bnObject* arrayInst = bnGetObject(bone->heap, arrayInstRef);
        GList* iter = entries;
        for (int i = 0; i < bnGetArrayLength(arrayInst); i++) {
                bnSetArrayElementAt(
                    arrayInst, i, bnNewString(bone, (bnStringView)iter->data));
                iter = iter->next;
        }
        bnWriteVariable2(frame, bone->pool, "ret", arrayInstRef);
        g_list_free(entries);
}

static void bnExtReflectionExport(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        GHashTableIter hashIter;
        g_hash_table_iter_init(&hashIter, a->table);
        gpointer k, v;
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                bnStringView view = (bnStringView)k;
                const char* str = bnView2Str(bone->pool, view);
                if (*str == '$') {
                        continue;
                }
                bnWriteVariable(frame, view, v);
        }
}
