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
        g_hash_table_replace(
            bone->externTable, bnIntern(bone->pool, "dir.files"),
            bnNewLambdaFromCFunc(bone, bnExtDirFiles, bone->pool,
                                 BN_C_ADD_PARAM, "path", BN_C_ADD_RETURN, "ret",
                                 BN_C_ADD_EXIT));
        g_hash_table_replace(
            bone->externTable, bnIntern(bone->pool, "dir.directories"),
            bnNewLambdaFromCFunc(bone, bnExtDirDirectories, bone->pool,
                                 BN_C_ADD_PARAM, "path", BN_C_ADD_RETURN, "ret",
                                 BN_C_ADD_EXIT));
        g_hash_table_replace(
            bone->externTable, bnIntern(bone->pool, "dir.delete"),
            bnNewLambdaFromCFunc(bone, bnExtDirDelete, bone->pool,
                                 BN_C_ADD_PARAM, "path", BN_C_ADD_RETURN,
                                 "error", BN_C_ADD_EXIT));
        g_hash_table_replace(
            bone->externTable, bnIntern(bone->pool, "dir.create"),
            bnNewLambdaFromCFunc(bone, bnExtDirCreate, bone->pool,
                                 BN_C_ADD_PARAM, "path", BN_C_ADD_RETURN,
                                 "error", BN_C_ADD_EXIT));
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
        bnString* aStr = a;
        g_rmdir(bnView2Str(bone->pool, aStr->value));
}

void bnExtDirCreate(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_STRING) {
                bnFormatThrow(bone, "should be `path` is string");
        }
        bnString* aStr = a;
        g_mkdir(bnView2Str(bone->pool, aStr->value), 0755);
}

static void collect_files(bnInterpreter* bone, bnFrame* frame, bool fileOnly) {
        bnObject* aryFunc = g_hash_table_lookup(frame->variableTable,
                                                bnIntern(bone->pool, "array"));
        bnObject* strFunc = g_hash_table_lookup(frame->variableTable,
                                                bnIntern(bone->pool, "string"));
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_STRING) {
                bnFormatThrow(bone, "should be `path` is string");
        }
        bnString* aStr = a;
        const char* dirname = bnView2Str(bone->pool, aStr->value);
        GDir* dp = g_dir_open(dirname, 0, NULL);
        if (dp == NULL) {
                bnFormatThrow(bone, "can't open directory");
        }
        const gchar* name = NULL;
        GList* files = NULL;
        // check files
        while (name = g_dir_read_name(dp)) {
                gchar* path = g_build_filename(dirname, name, NULL);
                gboolean is_dir = g_file_test(path, G_FILE_TEST_IS_DIR);
                int pathlen = strlen(path);
                if ((!is_dir && fileOnly) || (is_dir && !fileOnly)) {
                        // create new char array
                        bnPushStack(frame->vStack, bnNewInteger(bone, pathlen));
                        bnFrame* sub = bnFuncCall(aryFunc, bone, frame, 1);
                        if (sub->panic) {
                                frame->panic = sub->panic;
                                return;
                        }
                        bnArray* chary =
                            bnStaging(bone->heap, bnPopStack(frame->vStack));
                        bnFillString(bone, path, chary);
                        // create string
                        bnPushStack(frame->vStack, chary);
                        bnDeleteFrame(sub);
                        sub = bnFuncCall(strFunc, bone, frame, 1);
                        if (sub->panic) {
                                frame->panic = sub->panic;
                                return;
                        }
                        bnString* bnstr =
                            bnStaging(bone->heap, bnPopStack(frame->vStack));
                        files = g_list_append(files, bnstr);
                        bnDeleteFrame(sub);
                }
                g_free(path);
        }
        g_dir_close(dp);
        // create new array
        bnPushStack(frame->vStack, bnNewInteger(bone, g_list_length(files)));
        bnFrame* sub = bnFuncCall(aryFunc, bone, frame, 1);
        if (sub->panic) {
                frame->panic = sub->panic;
                return;
        }
        bnArray* ary = bnStaging(bone->heap, bnPopStack(frame->vStack));
        GList* filesIter = files;
        for (int i = 0; i < ary->size; i++) {
                g_ptr_array_index(ary->arr, i) = filesIter->data;
                filesIter = filesIter->next;
        }
        g_list_free(files);
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             ary);
        bnDeleteFrame(sub);
}