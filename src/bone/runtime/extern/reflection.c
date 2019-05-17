#include "reflection.h"
#include "../../bone.h"
#include "../char.h"
#include "../frame.h"
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
