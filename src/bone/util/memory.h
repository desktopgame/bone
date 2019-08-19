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
#if DEBUG
#if defined(NGLOBAL)
#define BN_MALLOC(size) (bnSafeMalloc(size))
#define BN_REALLOC(block, size) (bnSafeRealloc(block, size))
#define BN_FREE(block) bnSafeFree(block)
#else
#define BN_MALLOC(size) (bnMallocFunc(size, __FILE__, __LINE__))
#define BN_REALLOC(block, size) (bnReallocFunc(block, size, __FILE__, __LINE__))
#define BN_FREE(block) (bnFreeFunc(block, __FILE__, __LINE__))
#endif
#else
#define BN_MALLOC(size) (bnSafeMalloc(size))
#define BN_REALLOC(block, size) (bnSafeRealloc(block, size))
#define BN_FREE(block) bnSafeFree(block)
#endif
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

#if _MSC_VER && DEBUG

#define bnMallocFunc(size) \
        (_malloc_dbg(size, _NORMAL_BLOCK, __FILE__, __LINE__))
#define bnReallocFunc(block, size) \
        (_realloc_dbg(block, size, _NORMAL_BLOCK, __FILE__, __LINE__))
#define bnFreeFunc(block) (_free_dbg(block, _NORMAL_BLOCK))
#else

/**
 * include debug information to allocated memory
 * in general, called by macro
 * @param size
 * @param filename
 * @param lineno
 * @return
 */
void* bnMallocFunc(size_t size, const char* filename, int lineno);

/**
 * include debug information to allocated memory
 * in general, called by macro
 * @param block
 * @param newSize
 * @param filename
 * @param lineno
 * @return
 */
void* bnReallocFunc(void* block, size_t newSize, const char* filename,
                    int lineno);
/**
 * free of memory by free.
 * declared function for future extension
 * @param block
 * @param filename
 * @param lineno
 */
void bnFreeFunc(void* block, const char* filename, int lineno);
#endif

/**
 * check a overrun in memory
 */
void bnCheckMemoryBounds();

/**
 * dump a still freed memory.
 * @param fp
 */
void bnDumpMemoryLeaks(FILE* fp);
#endif