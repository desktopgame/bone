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

static void bnExtDirFiles(struct bnInterpreter* bone, struct bnFrame* frame);
static void bnExtDirDirectories(struct bnInterpreter* bone,
                                struct bnFrame* frame);
static void bnExtDirDelete(struct bnInterpreter* bone, struct bnFrame* frame);
static void bnExtDirCreate(struct bnInterpreter* bone, struct bnFrame* frame);
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

static void bnExtDirFiles(bnInterpreter* bone, bnFrame* frame) {
        collect_files(bone, frame, true);
}

static void bnExtDirDirectories(bnInterpreter* bone, bnFrame* frame) {
        collect_files(bone, frame, false);
}

static void bnExtDirDelete(bnInterpreter* bone, bnFrame* frame) {
        bnPopStringArg(bone, frame, path);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-function-declaration"
        g_rmdir(bnView2Str(bone->pool, bnGetStringValue(pathObj)));
#pragma clang diagnostic pop
}

static void bnExtDirCreate(bnInterpreter* bone, bnFrame* frame) {
        bnPopStringArg(bone, frame, path);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-function-declaration"
        g_mkdir(bnView2Str(bone->pool, bnGetStringValue(pathObj)), 0755);
#pragma clang diagnostic pop
}

static void collect_files(bnInterpreter* bone, bnFrame* frame, bool fileOnly) {
        bnReference aryFuncRef = bnReadVariable2(frame, bone->pool, "array");
        bnReference strFuncRef = bnReadVariable2(frame, bone->pool, "string");
        bnPopStringArg(bone, frame, path);
        const char* dirname = bnView2Str(bone->pool, bnGetStringValue(pathObj));
        GDir* dp = g_dir_open(dirname, 0, NULL);
        if (dp == NULL) {
                bnFormatThrow(bone, "can't open directory");
        }
        const gchar* name = NULL;
        GList* files = NULL;
        //ディレクトリ内の全てのファイルを確認する
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wparentheses"
        while (name = g_dir_read_name(dp)) {
                gchar* path = g_build_filename(dirname, name, NULL);
                gboolean is_dir = g_file_test(path, G_FILE_TEST_IS_DIR);
                int pathlen = strlen(path);
                if ((!is_dir && fileOnly) || (is_dir && !fileOnly)) {
                        //ファイル名を格納するための配列を作成
                        bnPushStack(frame->vStack, bnNewInteger(bone, pathlen));
                        bnFrame* sub = bnFuncCall(aryFuncRef, bone, frame, 1);
                        if (sub->panic) {
                                frame->panic = sub->panic;
                                return;
                        }
                        bnReference chary =
                            bnStaging(bone->heap, bnPopStack(frame->vStack));
                        bnFillString(bone, path, chary);
                        //配列を文字で埋める
                        bnPushStack(frame->vStack, chary);
                        bnDeleteFrame(sub);
                        //文字配列から文字列を作成する
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
        //全てのパス文字列を格納する配列を作成する
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