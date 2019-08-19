#ifndef BONE_MEMORY_H
#define BONE_MEMORY_H
#include <stdio.h>
#include <stdlib.h>

#if _MSC_VER
#include <crtdbg.h>
#if DEBUG
#define BN_MALLOC(size)                                                      \
        (bnNonNullFunc(_malloc_dbg(size, _NORMAL_BLOCK, __FILE__, __LINE__), \
                       __FILE__, __LINE__))
#define BN_REALLOC(block, size)                                           \
        (bnNonNullFunc(                                                   \
            _realloc_dbg(block, size, _NORMAL_BLOCK, __FILE__, __LINE__), \
            __FILE__, __LINE__))
#define BN_FREE(block) (_free_dbg(block, _NORMAL_BLOCK))
#else
#define BN_MALLOC(size) (bnSafeMalloc(size))
#define BN_REALLOC(block, size) (bnSafeRealloc(block, size))
#define BN_FREE(block) bnSafeFree(block)
#endif
#else
#define BN_MALLOC(size) (bnSafeMalloc(size))
#define BN_REALLOC(block, size) (bnSafeRealloc(block, size))
#define BN_FREE(block) bnSafeFree(block)
#endif

#if _MSC_VER
#define BN_CHECK_MEM() _ASSERTE(_CrtCheckMemory())
#else
#define BN_CHECK_MEM() ((void)0)
#endif

/**
 * 指定のサイズだけメモリを確保して返します。
 * 確保できなかった場合にはabort()します。
 * @param size
 * @return
 */
void* bnSafeMalloc(size_t size);

/**
 * 指定のサイズでメモリを再確保します。
 * 確保できなかった場合にはabort()します。
 * @param block
 * @param newSize
 * @return
 */
void* bnSafeRealloc(void* block, size_t newSize);

/**
 * 指定のメモリブロックを解放します。
 * @param block
 */
void bnSafeFree(void* block);

/**
 * 指定のメモリブロックがNULLであるかどうかアサーションします。
 * NULLでなければメモリブロックをそのまま返します。
 * @param pdata
 * @param filename
 * @param lineno
 * @return
 */
void* bnNonNullFunc(void* pdata, const char* filename, int lineno);

/**
 * @param pdata
 * @return
 */
#define bnNonNull(pdata) (bnNonNullFunc(pdata, __FILE__, __LINE__))
#endif