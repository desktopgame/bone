#ifndef BONE_RUNTIME_EXTERN_REFLECTION_H
#define BONE_RUNTIME_EXTERN_REFLECTION_H
struct bnInterpreter;
struct bnFrame;

void bnExternReflection(struct bnInterpreter* bone);

void bnExtReflectionDefine(struct bnInterpreter* bone, struct bnFrame* frame);

void bnExtReflectionUndef(struct bnInterpreter* bone, struct bnFrame* frame);

void bnExtReflectionDefined(struct bnInterpreter* bone, struct bnFrame* frame);

void bnExtReflectionExpand(struct bnInterpreter* bone, struct bnFrame* frame);

void bnExtReflectionEntries(struct bnInterpreter* bone, struct bnFrame* frame);

#endif