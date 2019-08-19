#ifndef BONE_RUNTIME_EXTERN_FILE_H
#define BONE_RUNTIME_EXTERN_FILE_H
#include <stdio.h>
#include "../any.h"

#define FILE_T ("bone.lang.file")

/**
 * ファイル操作のための関数をインタプリタへエクスポートします。
 * @param bone
 */
void bnExternFile(struct bnInterpreter* bone);

#endif