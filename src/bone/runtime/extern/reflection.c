#include "reflection.h"
#include "../../bone.h"
#include "../array.h"
#include "../char.h"
#include "../frame.h"
#include "../integer.h"
#include "../interpreter.h"
#include "../lambda.h"
#include "../string.h"

void bnExternReflection(bnInterpreter* bone) {
        g_hash_table_replace(
            bone->externTable, bnIntern(bone->pool, "define"),
            bnNewLambdaFromCFunc(bone, bnExtReflectionDefine, bone->pool,
                                 BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM, "name",
                                 BN_C_ADD_PARAM, "value", BN_C_ADD_EXIT));
        g_hash_table_replace(
            bone->externTable, bnIntern(bone->pool, "undef"),
            bnNewLambdaFromCFunc(bone, bnExtReflectionUndef, bone->pool,
                                 BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM, "name",
                                 BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        g_hash_table_replace(
            bone->externTable, bnIntern(bone->pool, "defined"),
            bnNewLambdaFromCFunc(bone, bnExtReflectionDefined, bone->pool,
                                 BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM, "name",
                                 BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        g_hash_table_replace(
            bone->externTable, bnIntern(bone->pool, "expand"),
            bnNewLambdaFromCFunc(bone, bnExtReflectionExpand, bone->pool,
                                 BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM, "name",
                                 BN_C_ADD_RETURN, "value", BN_C_ADD_RETURN,
                                 "error", BN_C_ADD_EXIT));
        g_hash_table_replace(
            bone->externTable, bnIntern(bone->pool, "entries"),
            bnNewLambdaFromCFunc(bone, bnExtReflectionEntries, bone->pool,
                                 BN_C_ADD_PARAM, "self", BN_C_ADD_RETURN, "ret",
                                 BN_C_ADD_EXIT));
}

void bnExtReflectionDefine(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        bnObject* c = bnPopStack(frame->vStack);
        if (b->type != BN_OBJECT_STRING) {
                bnFormatThrow(bone, "shoud be `name` is string");
        }
        bnString* bStr = b;
        g_hash_table_replace(a->table, bStr->value, c);
}

void bnExtReflectionUndef(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (b->type != BN_OBJECT_STRING) {
                bnFormatThrow(bone, "shoud be `name` is string");
        }
        bnString* bStr = b;
        gboolean removed = g_hash_table_remove(a->table, bStr->value);
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, removed));
}

void bnExtReflectionDefined(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (b->type != BN_OBJECT_STRING) {
                bnFormatThrow(bone, "shoud be `name` is string");
        }
        bnString* bStr = b;
        gboolean contains = g_hash_table_contains(a->table, bStr->value);
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, contains));
}

void bnExtReflectionExpand(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (b->type != BN_OBJECT_STRING) {
                bnFormatThrow(bone, "shoud be `name` is string");
        }
        bnString* bStr = b;
        gboolean contains = g_hash_table_contains(a->table, bStr->value);
        g_hash_table_replace(frame->variableTable,
                             bnIntern(bone->pool, "error"),
                             bnGetBool(bone->pool, frame, !contains));
        if (contains) {
                g_hash_table_replace(
                    frame->variableTable, bnIntern(bone->pool, "value"),
                    g_hash_table_lookup(a->table, bStr->value));
        }
}

void bnExtReflectionEntries(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
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
        bnLambda* arrayFunc = g_hash_table_lookup(
            frame->variableTable, bnIntern(bone->pool, "array"));
        bnPushStack(frame->vStack, bnNewInteger(bone, g_list_length(entries)));
        bnFrame* sub = bnFuncCall(arrayFunc, bone, frame, 1);
        bnDeleteFrame(sub);
        bnArray* arrayInst = bnPopStack(frame->vStack);
        GList* iter = entries;
        for (int i = 0; i < arrayInst->arr->len; i++) {
                g_ptr_array_index(arrayInst->arr, i) =
                    bnNewString(bone, iter->data);
                iter = iter->next;
        }
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             arrayInst);
        g_list_free(entries);
}
