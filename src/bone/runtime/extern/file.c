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
        bnWriteExtern2(bone, "file.fopen",
                       bnNewLambdaFromCFunc(
                           bone, bnExtFileOpen, bone->pool, BN_C_ADD_PARAM,
                           "path", BN_C_ADD_PARAM, "mode", BN_C_ADD_RETURN,
                           "ret", BN_C_ADD_RETURN, "error", BN_C_ADD_EXIT));
        bnWriteExtern2(
            bone, "file.fputc",
            bnNewLambdaFromCFunc(bone, bnExtFilePutc, bone->pool,
                                 BN_C_ADD_PARAM, "self", BN_C_ADD_PARAM, "c",
                                 BN_C_ADD_RETURN, "error", BN_C_ADD_EXIT));
        bnWriteExtern2(
            bone, "file.fgetc",
            bnNewLambdaFromCFunc(bone, bnExtFileGetc, bone->pool,
                                 BN_C_ADD_PARAM, "self", BN_C_ADD_RETURN, "ret",
                                 BN_C_ADD_RETURN, "error", BN_C_ADD_EXIT));
        bnWriteExtern2(
            bone, "file.fclose",
            bnNewLambdaFromCFunc(bone, bnExtFileClose, bone->pool,
                                 BN_C_ADD_PARAM, "self", BN_C_ADD_EXIT));
        bnWriteExtern2(bone, "file.stdout", bnNewFile(bone, stdout));
        bnWriteExtern2(bone, "file.stderr", bnNewFile(bone, stderr));
        bnWriteExtern2(bone, "file.stdin", bnNewFile(bone, stdin));
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
        FILE* fp = fopen(bnView2Str(bone->pool, bnGetStringValue(a)),
                         bnView2Str(bone->pool, bnGetStringValue(b)));
        bnWriteVariable2(frame, bone->pool, "error",
                         bnGetFalse(bone->pool, frame));
        if (fp == NULL) {
                bnWriteVariable2(frame, bone->pool, "error",
                                 bnNewString2(bone, strerror(errno)));
                return;
        }
        bnWriteVariable2(frame, bone->pool, "ret", bnNewFile(bone, fp));
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
        bnWriteVariable2(frame, bone->pool, "error",
                         bnGetFalse(bone->pool, frame));
        FILE* fp = afile->fp;
        char c = bnGetCharValue(b);
        int code = fputc(c, fp);
        if (code == EOF) {
                bnWriteVariable2(frame, bone->pool, "error",
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
        bnWriteVariable2(frame, bone->pool, "error",
                         bnGetFalse(bone->pool, frame));
        FILE* fp = afile->fp;
        int c = fgetc(fp);
        fpos_t pos;
        if (c == EOF || feof(fp) || fgetpos(fp, &pos)) {
                bnWriteVariable2(frame, bone->pool, "error",
                                 bnGetTrue(bone->pool, frame));
                clearerr(fp);
                return;
        }
        bnWriteVariable2(frame, bone->pool, "ret", bnNewChar(bone, c));
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