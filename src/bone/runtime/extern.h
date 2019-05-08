#ifndef BONE_RUNTIME_EXTERN_H
#define BONE_RUNTIME_EXTERN_H
struct bnFrame;
struct bnInterpreter;

void bnExtExit(struct bnInterpreter* bone, struct bnFrame* frame);

void bnExtAbort(struct bnInterpreter* bone, struct bnFrame* frame);
#endif