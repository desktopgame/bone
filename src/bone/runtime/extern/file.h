#ifndef BONE_RUNTIME_EXTERN_FILE_H
#define BONE_RUNTIME_EXTERN_FILE_H
#include <stdio.h>
#include "../any.h"

#define FILE_T ("bone.lang.file")

bnObject* bnNewFile(struct bnInterpreter* bone, FILE* fp);

void bnExternFile(struct bnInterpreter* bone);

void bnExtFileOpen(struct bnInterpreter* bone, struct bnFrame* frame);

void bnExtFilePutc(struct bnInterpreter* bone, struct bnFrame* frame);

void bnExtFileGetc(struct bnInterpreter* bone, struct bnFrame* frame);

void bnExtFileClose(struct bnInterpreter* bone, struct bnFrame* frame);
#endif