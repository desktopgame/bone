#ifndef BONE_RUNTIME_STRING_H
#define BONE_RUNTIME_STRING_H
#include "interpreter.h"
#include "object.h"

/**
 * 新しい文字列インスタンスを生成して、参照を返します。
 * @param bone
 * @param name
 * @return
 */
bnReference bnNewString(bnInterpreter* bone, bnStringView value);

/**
 * 新しい文字列インスタンスを生成して、参照を返します。
 * @param bone
 * @param str
 * @return
 */
bnReference bnNewString2(bnInterpreter* bone, const char* str);

/**
 * 文字列プールで使用可能な値に変換します。
 * @param obj
 * @return
 */
bnStringView bnGetStringValue(bnObject* obj);
#endif