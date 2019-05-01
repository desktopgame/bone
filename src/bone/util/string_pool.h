#ifndef BONE_UTIL_STRINGPOOL_H
#define BONE_UTIL_STRINGPOOL_H

#define BN_NULL_STRING (NULL)
#define BN_EMPTY_STRING ("")

#define BN_NULL_VIEW (0)
#define BN_EMPTY_VIEW (1)

/**
 * encoded string.
 */
typedef unsigned int bnStringView;

/**
 * bnStringPool is pool of string.
 * string pool policy
 * -- duplicate string of key always
 * -- string of return value should NOT be dispose!
 * -- should NOT be share instance between threads!
 */
struct bnStringPool;

/**
 * return new instance of bnStringPool.
 * @param value
 * @return
 */
struct bnStringPool* bnNewStringPool();

/**
 * encode string to unsigned integer.
 * return old value, if already encoded.
 * @param self
 * @param str
 * @return
 */
bnStringView bnIntern(struct bnStringPool* self, const char* str);

/**
 * return of encoded integer from string.
 * terminate by assert if not already encoded.
 * @param self
 * @param str
 * @return
 */
bnStringView bnStr2View(struct bnStringPool* self, const char* str);

/**
 * return of string from encoded integer.
 * terminate by assert if not already encoded
 * @param self
 * @param view
 * @return
 */
const char* bnView2Str(struct bnStringPool* self, bnStringView view);

/**
 * free a bnStringPool.
 * @param self
 */
void bnDeleteStringPool(struct bnStringPool* self);
#endif