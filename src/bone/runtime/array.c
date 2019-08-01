#include "array.h"
#include "char.h"
#include "integer.h"
#include "interpreter.h"
#include "keyword.h"
#include "lambda.h"
#include "frame.h"

#define _throw(bone, frame, fmt) (bnFormatThrow(bone, fmt))

static void bnStdArrayArraySet(bnInterpreter* bone, bnFrame* frame);
static void bnStdArrayArrayGet(bnInterpreter* bone, bnFrame* frame);
static void free_array(bnObject* obj);

bnArray* bnNewArray(bnInterpreter* bone, int size) {
        bnArray* ret = BN_MALLOC(sizeof(bnArray));
        bnInitObject(bone, &ret->base, BN_OBJECT_ARRAY);
        ret->base.freeFunc = free_array;
        ret->arr = g_ptr_array_new();
        ret->size = size;
        g_ptr_array_set_free_func(ret->arr, NULL);
        for (int i = 0; i < size; i++) {
                g_ptr_array_add(ret->arr, bnNewObject(bone));
        }
        bnDefine(&ret->base, bnIntern(bone->pool, "length"),
                 bnNewInteger(bone, size));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_ARRAY_SET),
                 bnNewLambdaFromCFunc(bone, bnStdArrayArraySet, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "index", BN_C_ADD_PARAM, "value",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, BN_KWD_ARRAY_GET),
                 bnNewLambdaFromCFunc(bone, bnStdArrayArrayGet, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "index", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, "set"),
                 bnNewLambdaFromCFunc(bone, bnStdArrayArraySet, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "index", BN_C_ADD_PARAM, "value",
                                      BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        bnDefine(&ret->base, bnIntern(bone->pool, "get"),
                 bnNewLambdaFromCFunc(bone, bnStdArrayArrayGet, bone->pool,
                                      BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM,
                                      "index", BN_C_ADD_RETURN, "ret",
                                      BN_C_ADD_EXIT));
        return ret;
}

void bnFillString(bnInterpreter* bone, const char* str, bnArray* ary) {
        for (int i = 0; i < ary->size; i++) {
                g_ptr_array_index(ary->arr, i) = bnNewChar(bone, str[i]);
        }
}



// Array
static void bnStdArrayArraySet(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        bnObject* c = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_ARRAY) {
                _throw(bone, frame, "should be `self` is array");
        }
        if (b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "should be `index` is integer");
        }
        bnArray* arr = a;
        bnInteger* idx = b;
        bnInteger* val = c;
        g_ptr_array_index(arr->arr, idx->value) = c;
}

static void bnStdArrayArrayGet(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_ARRAY) {
                _throw(bone, frame, "should be `self` is array");
        }
        if (b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "should be `index` is integer");
        }
        bnArray* arr = a;
        bnInteger* idx = b;
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             g_ptr_array_index(arr->arr, idx->value));
}

static void free_array(bnObject* obj) {
        obj->freeFunc = NULL;
        bnArray* arr = obj;
        g_ptr_array_unref(arr->arr);
        bnDeleteObject(obj);
}