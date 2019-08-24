#ifndef BONE_RUNTIME_EXTERN_REFLECTION_H
#define BONE_RUNTIME_EXTERN_REFLECTION_H
struct bnInterpreter;
struct bnFrame;

/**
 * リフレクションのための関数をインタプリタをエクスポートします。
 * @param bone
 */
void bnExternReflection(struct bnInterpreter* bone);

#endif