#ifndef BONE_RUNTIME_EXTERN_STRING_BUFFER_H
#define BONE_RUNTIME_EXTERN_STRING_BUFFER_H
struct bnInterpreter;

#define STRING_BUFFER_T ("bone.lang.string_buffer")

/**
 * 文字列バッファのための関数をインタプリタをエクスポートします。
 * @param bone
 */
void bnExternStringBuffer(struct bnInterpreter* bone);

#endif