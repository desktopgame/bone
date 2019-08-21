#include "file.h"
#include <errno.h>
#include "../../bone.h"
#include "../char.h"
#include "../frame.h"
#include "../heap.h"
#include "../interpreter.h"
#include "../lambda.h"
#include "../string.h"

typedef struct bnFile {
        bnAny base;
        FILE* fp;
} bnFile;

static bnReference bnNewFile(struct bnInterpreter* bone, FILE* fp);
static void bnExtFileOpen(struct bnInterpreter* bone, struct bnFrame* frame);
static void bnExtFilePutc(struct bnInterpreter* bone, struct bnFrame* frame);
static void bnExtFileGetc(struct bnInterpreter* bone, struct bnFrame* frame);
static void bnExtFileClose(struct bnInterpreter* bone, struct bnFrame* frame);

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

static bnReference bnNewFile(bnInterpreter* bone, FILE* fp) {
        bnReference ref = bnAllocObject(bone->heap);
        bnFile* ret = bnGetObject(bone->heap, ref);
        bnInitAny(bone, &ret->base, FILE_T);
        ret->fp = fp;
        return ref;
}

static void bnExtFileOpen(bnInterpreter* bone, bnFrame* frame) {
        bnPopStringArg(bone, frame, path);
        bnPopStringArg(bone, frame, mode);
        FILE* fp = fopen(bnView2Str(bone->pool, bnGetStringValue(pathObj)),
                         bnView2Str(bone->pool, bnGetStringValue(modeObj)));
        bnWriteVariable2(frame, bone->pool, "error",
                         bnGetFalse(bone->pool, frame));
        if (fp == NULL) {
                bnWriteVariable2(frame, bone->pool, "error",
                                 bnNewString2(bone, strerror(errno)));
                return;
        }
        bnWriteVariable2(frame, bone->pool, "ret", bnNewFile(bone, fp));
}

static void bnExtFilePutc(bnInterpreter* bone, bnFrame* frame) {
        bnPopAnyArg(bone, frame, self, FILE_T);
        bnPopCharArg(bone, frame, c);
        bnFile* afile = ((bnFile*)selfObj);
        if (afile->fp == NULL) {
                bnFormatThrow(bone, "file is already closed");
        }
        bnWriteVariable2(frame, bone->pool, "error",
                         bnGetFalse(bone->pool, frame));
        FILE* fp = afile->fp;
        int code = fputc(bnGetCharValue(cObj), fp);
        if (code == EOF) {
                bnWriteVariable2(frame, bone->pool, "error",
                                 bnGetTrue(bone->pool, frame));
        }
}

static void bnExtFileGetc(bnInterpreter* bone, bnFrame* frame) {
        bnPopAnyArg(bone, frame, self, FILE_T);
        bnFile* afile = ((bnFile*)selfObj);
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

static void bnExtFileClose(bnInterpreter* bone, bnFrame* frame) {
        bnPopAnyArg(bone, frame, self, FILE_T);
        bnFile* afile = (bnFile*)selfObj;
        if (afile->fp != NULL) {
                fclose(afile->fp);
                afile->fp = NULL;
        }
}