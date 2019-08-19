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
#include "../heap.h"
#include "../integer.h"
#include "../interpreter.h"
#include "../lambda.h"
#include "../object.h"
#include "../string.h"
#include "../vm.h"

static void bnExtSystemExit(struct bnInterpreter* bone, struct bnFrame* frame);
static void bnExtSystemAbort(struct bnInterpreter* bone, struct bnFrame* frame);
static void bnExtSystemSystem(struct bnInterpreter* bone,
                              struct bnFrame* frame);

void bnExternSystem(bnInterpreter* bone) {
        bnWriteExtern2(
            bone, "exit",
            bnNewLambdaFromCFunc(bone, bnExtSystemExit, bone->pool,
                                 BN_C_ADD_PARAM, "status", BN_C_ADD_EXIT));
        bnWriteExtern2(bone, "abort",
                       bnNewLambdaFromCFunc(bone, bnExtSystemAbort, bone->pool,
                                            BN_C_ADD_EXIT));
        bnWriteExtern2(bone, "system",
                       bnNewLambdaFromCFunc(
                           bone, bnExtSystemSystem, bone->pool, BN_C_ADD_PARAM,
                           "args", BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
}

static void bnExtSystemExit(bnInterpreter* bone, bnFrame* frame) {
        bnObject* statusObj =
            bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (statusObj->type != BN_OBJECT_INTEGER) {
                bnThrow(bone,
                        bnNewString2(bone, "should be `status` is integer"),
                        BN_JMP_CODE_EXCEPTION);
        }
        exit(bnGetIntegerValue(statusObj));
}

static void bnExtSystemAbort(bnInterpreter* bone, bnFrame* frame) { abort(); }

static void bnExtSystemSystem(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (a->type != BN_OBJECT_ARRAY) {
                bnThrow(
                    bone,
                    bnNewString(bone, bnIntern(bone->pool,
                                               "should be `args` is array")),
                    BN_JMP_CODE_EXCEPTION);
        }
        bnObject* args = a;
        GString* gbuf = g_string_new("");
        for (int i = 0; i < bnGetArrayLength(args); i++) {
                bnReference eRef = bnGetArrayElementAt(args, i);
                bnObject* e = bnGetObject(bone->heap, eRef);
                if (e->type != BN_OBJECT_STRING) {
                        bnFormatThrow(bone, "should be [%d] i string", i);
                }
                g_string_append(gbuf,
                                bnView2Str(bone->pool, bnGetStringValue(e)));
                g_string_append(gbuf, " ");
        }
        g_string_erase(gbuf, gbuf->len - 1, 1);
        int code = system(gbuf->str);
        g_string_free(gbuf, TRUE);
        bnWriteVariable2(frame, bone->pool, "ret", bnNewInteger(bone, code));
}