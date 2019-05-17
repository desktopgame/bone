#include "array.h"
#include "integer.h"
#include "interpreter.h"
#include "keyword.h"
#include "lambda.h"
#include "std.h"

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

static void free_array(bnObject* obj) {
        obj->freeFunc = NULL;
        bnArray* arr = obj;
        g_ptr_array_unref(arr->arr);
        bnDeleteObject(obj);
}