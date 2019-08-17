#include "dir.h"
#include <errno.h>
#include "../../bone.h"
#include "../../glib.h"
#include "../array.h"
#include "../char.h"
#include "../frame.h"
#include "../heap.h"
#include "../integer.h"
#include "../interpreter.h"
#include "../lambda.h"
#include "../string.h"

static void collect_files(bnInterpreter* bone, bnFrame* frame, bool fileOnly);

void bnExternDir(bnInterpreter* bone) {
        bnWriteExtern2(bone, "dir.files",
                       bnNewLambdaFromCFunc(
                           bone, bnExtDirFiles, bone->pool, BN_C_ADD_PARAM,
                           "path", BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        bnWriteExtern2(
            bone, "dir.directories",
            bnNewLambdaFromCFunc(bone, bnExtDirDirectories, bone->pool,
                                 BN_C_ADD_PARAM, "path", BN_C_ADD_RETURN, "ret",
                                 BN_C_ADD_EXIT));
        bnWriteExtern2(bone, "dir.delete",
                       bnNewLambdaFromCFunc(
                           bone, bnExtDirDelete, bone->pool, BN_C_ADD_PARAM,
                           "path", BN_C_ADD_RETURN, "error", BN_C_ADD_EXIT));
        bnWriteExtern2(bone, "dir.create",
                       bnNewLambdaFromCFunc(
                           bone, bnExtDirCreate, bone->pool, BN_C_ADD_PARAM,
                           "path", BN_C_ADD_RETURN, "error", BN_C_ADD_EXIT));
}

void bnExtDirFiles(bnInterpreter* bone, bnFrame* frame) {
        collect_files(bone, frame, true);
}

void bnExtDirDirectories(bnInterpreter* bone, bnFrame* frame) {
        collect_files(bone, frame, false);
}

void bnExtDirDelete(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_STRING) {
                bnFormatThrow(bone, "should be `path` is string");
        }
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-function-declaration"
        g_rmdir(bnView2Str(bone->pool, bnGetStringValue(a)));
#pragma clang diagnostic pop
}

void bnExtDirCreate(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_STRING) {
                bnFormatThrow(bone, "should be `path` is string");
        }
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-function-declaration"
        g_mkdir(bnView2Str(bone->pool, bnGetStringValue(a)), 0755);
#pragma clang diagnostic pop
}

static void collect_files(bnInterpreter* bone, bnFrame* frame, bool fileOnly) {
        bnReference aryFuncRef = bnReadVariable2(frame, bone->pool, "array");
        bnReference strFuncRef = bnReadVariable2(frame, bone->pool, "string");
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_STRING) {
                bnFormatThrow(bone, "should be `path` is string");
        }
        const char* dirname = bnView2Str(bone->pool, bnGetStringValue(a));
        GDir* dp = g_dir_open(dirname, 0, NULL);
        if (dp == NULL) {
                bnFormatThrow(bone, "can't open directory");
        }
        const gchar* name = NULL;
        GList* files = NULL;
        // check files
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wparentheses"
        while (name = g_dir_read_name(dp)) {
                gchar* path = g_build_filename(dirname, name, NULL);
                gboolean is_dir = g_file_test(path, G_FILE_TEST_IS_DIR);
                int pathlen = strlen(path);
                if ((!is_dir && fileOnly) || (is_dir && !fileOnly)) {
                        // create new char array
                        bnPushStack(frame->vStack, bnNewInteger(bone, pathlen));
                        bnFrame* sub = bnFuncCall(aryFuncRef, bone, frame, 1);
                        if (sub->panic) {
                                frame->panic = sub->panic;
                                return;
                        }
                        bnReference chary =
                            bnStaging(bone->heap, bnPopStack(frame->vStack));
                        bnFillString(bone, path, chary);
                        // create string
                        bnPushStack(frame->vStack, chary);
                        bnDeleteFrame(sub);
                        sub = bnFuncCall(strFuncRef, bone, frame, 1);
                        if (sub->panic) {
                                frame->panic = sub->panic;
                                return;
                        }
                        bnReference bnstr =
                            bnStaging(bone->heap, bnPopStack(frame->vStack));
                        files = g_list_append(files, bnstr);
                        bnDeleteFrame(sub);
                }
                g_free(path);
        }
#pragma clang diagnostic pop
        g_dir_close(dp);
        // create new array
        bnPushStack(frame->vStack, bnNewInteger(bone, g_list_length(files)));
        bnFrame* sub = bnFuncCall(aryFuncRef, bone, frame, 1);
        if (sub->panic) {
                frame->panic = sub->panic;
                return;
        }
        bnReference aryRef = bnStaging(bone->heap, bnPopStack(frame->vStack));
        bnObject* ary = bnGetObject(bone->heap, aryRef);
        GList* filesIter = files;
        for (int i = 0; i < bnGetArrayLength(ary); i++) {
                bnSetArrayElementAt(ary, i, (bnReference)filesIter->data);
                filesIter = filesIter->next;
        }
        g_list_free(files);
        bnWriteVariable2(frame, bone->pool, "ret", aryRef);
        bnDeleteFrame(sub);
}