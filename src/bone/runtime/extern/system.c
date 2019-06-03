#include "system.h"
#include <assert.h>
#include "../../bone.h"
#include "../../parse/ast2il.h"
#include "../../parse/parser.h"
#include "../../util/fmt.h"
#include "../array.h"
#include "../bool.h"
#include "../enviroment.h"
#include "../frame.h"
#include "../integer.h"
#include "../interpreter.h"
#include "../lambda.h"
#include "../object.h"
#include "../string.h"
#include "../vm.h"

void bnExternSystem(bnInterpreter* bone) {
        g_hash_table_replace(
            bone->externTable, bnIntern(bone->pool, "exit"),
            bnNewLambdaFromCFunc(bone, bnExtSystemExit, bone->pool,
                                 BN_C_ADD_PARAM, "status", BN_C_ADD_EXIT));
        g_hash_table_replace(bone->externTable, bnIntern(bone->pool, "abort"),
                             bnNewLambdaFromCFunc(bone, bnExtSystemAbort,
                                                  bone->pool, BN_C_ADD_EXIT));
        g_hash_table_replace(
            bone->externTable, bnIntern(bone->pool, "system"),
            bnNewLambdaFromCFunc(bone, bnExtSystemSystem, bone->pool,
                                 BN_C_ADD_PARAM, "args", BN_C_ADD_RETURN, "ret",
                                 BN_C_ADD_EXIT));
}

void bnExtSystemExit(bnInterpreter* bone, bnFrame* frame) {
        bnObject* statusObj = bnPopStack(frame->vStack);
        if (statusObj->type != BN_OBJECT_INTEGER) {
                bnThrow(bone,
                        bnNewString2(bone, "should be `status` is integer"),
                        BN_JMP_CODE_EXCEPTION);
        }
        bnInteger* statusInt = statusObj;
        exit(statusInt->value);
}

void bnExtSystemAbort(bnInterpreter* bone, bnFrame* frame) { abort(); }

void bnExtSystemSystem(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_ARRAY) {
                bnThrow(
                    bone,
                    bnNewString(bone, bnIntern(bone->pool,
                                               "should be `args` is array")),
                    BN_JMP_CODE_EXCEPTION);
        }
        bnArray* args = a;
        GString* gbuf = g_string_new("");
        for (int i = 0; i < args->arr->len; i++) {
                bnObject* e = g_ptr_array_index(args->arr, i);
                if (e->type != BN_OBJECT_STRING) {
                        bnFormatThrow(bone, "should be [%d] i string", i);
                }
                g_string_append(gbuf,
                                bnView2Str(bone->pool, ((bnString*)e)->value));
                g_string_append(gbuf, " ");
        }
        g_string_erase(gbuf, gbuf->len - 1, 1);
        int code = system(gbuf->str);
        g_string_free(gbuf, TRUE);
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, code));
}