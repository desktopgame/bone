#ifndef BONE_RUNTIME_EXTERN_DIR_H
#define BONE_RUNTIME_EXTERN_DIR_H
#include "../any.h"

#define DIR_T ("bone.lang.dir")

void bnExternDir(struct bnInterpreter* bone);

void bnExtDirFiles(struct bnInterpreter* bone, struct bnFrame* frame);

void bnExtDirDirectories(struct bnInterpreter* bone, struct bnFrame* frame);

void bnExtDirDelete(struct bnInterpreter* bone, struct bnFrame* frame);

void bnExtDirCreate(struct bnInterpreter* bone, struct bnFrame* frame);
#endif