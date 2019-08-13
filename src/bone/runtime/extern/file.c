#include "file.h"
#include <errno.h>
#include "../../bone.h"
#include "../char.h"
#include "../frame.h"
#include "../interpreter.h"
#include "../lambda.h"
#include "../string.h"

typedef struct bnFile {
        bnAny base;
        FILE* fp;
} bnFile;

bnObject* bnNewFile(bnInterpreter* bone, FILE* fp) {
        bnFile* ret = BN_MALLOC(sizeof(bnFile));
        bnInitAny(bone, &ret->base, FILE_T);
        ret->fp = fp;
        return (bnObject*)ret;
}

void bnExternFile(bnInterpreter* bone) {
        g_hash_table_replace(
            bone->externTable, bnIntern(bone->pool, "file.fopen"),
            bnNewLambdaFromCFunc(bone, bnExtFileOpen, bone->pool,
                                 BN_C_ADD_PARAM, "path", BN_C_ADD_PARAM, "mode",
                                 BN_C_ADD_RETURN, "ret", BN_C_ADD_RETURN,
                                 "error", BN_C_ADD_EXIT));
        g_hash_table_replace(
            bone->externTable, bnIntern(bone->pool, "file.fputc"),
            bnNewLambdaFromCFunc(bone, bnExtFilePutc, bone->pool,
                                 BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM, "c",
                                 BN_C_ADD_RETURN, "error", BN_C_ADD_EXIT));
        g_hash_table_replace(
            bone->externTable, bnIntern(bone->pool, "file.fgetc"),
            bnNewLambdaFromCFunc(bone, bnExtFileGetc, bone->pool,
                                 BN_C_ADD_PARAM, "self", BN_C_ADD_RETURN, "ret",
                                 BN_C_ADD_RETURN, "error", BN_C_ADD_EXIT));
        g_hash_table_replace(
            bone->externTable, bnIntern(bone->pool, "file.fclose"),
            bnNewLambdaFromCFunc(bone, bnExtFileClose, bone->pool,
                                 BN_C_ADD_PARAM, "self", BN_C_ADD_EXIT));
        g_hash_table_replace(bone->externTable,
                             bnIntern(bone->pool, "file.stdout"),
                             bnNewFile(bone, stdout));
        g_hash_table_replace(bone->externTable,
                             bnIntern(bone->pool, "file.stderr"),
                             bnNewFile(bone, stderr));
        g_hash_table_replace(bone->externTable,
                             bnIntern(bone->pool, "file.stdin"),
                             bnNewFile(bone, stdin));
}

void bnExtFileOpen(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_STRING) {
                bnFormatThrow(bone, "should be `path` is string");
        }
        if (b->type != BN_OBJECT_STRING) {
                bnFormatThrow(bone, "should be `mode` is string");
        }
        bnString* astr = a;
        bnString* bstr = b;
        FILE* fp = fopen(bnView2Str(bone->pool, astr->value),
                         bnView2Str(bone->pool, bstr->value));
        g_hash_table_replace(frame->variableTable,
                             bnIntern(bone->pool, "error"),
                             bnGetFalse(bone->pool, frame));
        if (fp == NULL) {
                g_hash_table_replace(frame->variableTable,
                                     bnIntern(bone->pool, "error"),
                                     bnNewString2(bone, strerror(errno)));
                return;
        }
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewFile(bone, fp));
}

void bnExtFilePutc(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_ANY || !bnMatchType(bone, a, FILE_T)) {
                bnFormatThrow(bone, "should be `self` is file");
        }
        if (b->type != BN_OBJECT_CHAR) {
                bnFormatThrow(bone, "should be `c` is char");
        }
        bnFile* afile = ((bnFile*)a);
        if (afile->fp == NULL) {
                bnFormatThrow(bone, "file is already closed");
        }
        g_hash_table_replace(frame->variableTable,
                             bnIntern(bone->pool, "error"),
                             bnGetFalse(bone->pool, frame));
        FILE* fp = afile->fp;
        char c = ((bnChar*)b)->value;
        int code = fputc(c, fp);
        if (code == EOF) {
                g_hash_table_replace(frame->variableTable,
                                     bnIntern(bone->pool, "error"),
                                     bnGetTrue(bone->pool, frame));
        }
}

void bnExtFileGetc(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_ANY || !bnMatchType(bone, a, FILE_T)) {
                bnFormatThrow(bone, "should be `self` is file");
        }
        bnFile* afile = ((bnFile*)a);
        if (afile->fp == NULL) {
                bnFormatThrow(bone, "file is already closed");
        }
        g_hash_table_replace(frame->variableTable,
                             bnIntern(bone->pool, "error"),
                             bnGetFalse(bone->pool, frame));
        FILE* fp = afile->fp;
        int c = fgetc(fp);
        fpos_t pos;
        if (c == EOF || feof(fp) || fgetpos(fp, &pos)) {
                g_hash_table_replace(frame->variableTable,
                                     bnIntern(bone->pool, "error"),
                                     bnGetTrue(bone->pool, frame));
                clearerr(fp);
                return;
        }
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewChar(bone, c));
}

void bnExtFileClose(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_ANY || !bnMatchType(bone, a, FILE_T)) {
                bnFormatThrow(bone, "should be `self` is file");
        }
        bnFile* afile = a;
        if (afile->fp != NULL) {
                fclose(afile->fp);
                afile->fp = NULL;
        }
}