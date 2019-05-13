#ifndef BONE_RUNTIME_EXTERN_H
#define BONE_RUNTIME_EXTERN_H
struct bnFrame;
struct bnInterpreter;

void bnExtSystemExit(struct bnInterpreter* bone, struct bnFrame* frame);

void bnExtSystemAbort(struct bnInterpreter* bone, struct bnFrame* frame);

void bnExtSystemSystem(struct bnInterpreter* bone, struct bnFrame* frame);
#endif