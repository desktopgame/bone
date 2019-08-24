#ifndef BONE_RUNTIME_EXTERN_SYSTEM_H
#define BONE_RUNTIME_EXTERN_SYSTEM_H
struct bnFrame;
struct bnInterpreter;

/**
 * ユーティリティ関数をインタプリタへエクスポートします。
 * @param bone
 */
void bnExternSystem(struct bnInterpreter* bone);

#endif