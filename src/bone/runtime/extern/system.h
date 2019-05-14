#ifndef BONE_RUNTIME_EXTERN_SYSTEM_H
#define BONE_RUNTIME_EXTERN_SYSTEM_H
struct bnFrame;
struct bnInterpreter;

void bnExternSystem(struct bnInterpreter* bone);

void bnExtSystemExit(struct bnInterpreter* bone, struct bnFrame* frame);

void bnExtSystemAbort(struct bnInterpreter* bone, struct bnFrame* frame);

void bnExtSystemSystem(struct bnInterpreter* bone, struct bnFrame* frame);
#endif