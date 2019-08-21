#ifndef BONE_RUNTIME_EXTERN_REGEX_H
#define BONE_RUNTIME_EXTERN_REGEX_H
struct bnInterpreter;
struct bnFrame;

#define REGEX_T ("bone.lang.regex")
#define MATCH_INFO_T ("bone.lang.match_info")

/**
 * 正規表現のための関数をインタプリタをエクスポートします。
 * @param bone
 */
void bnExternRegex(struct bnInterpreter* bone);

#endif