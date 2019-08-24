#ifndef BONE_RUNTIME_EXTERN_DIR_H
#define BONE_RUNTIME_EXTERN_DIR_H
#include "../any.h"

#define DIR_T ("bone.lang.dir")

/**
 * ディレクトリ操作のための関数をインタプリタへエクスポートします。
 * @param bone
 */
void bnExternDir(struct bnInterpreter* bone);

#endif