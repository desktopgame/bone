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
#define BN_MALLOC(size) (bnMallocFunc(size, __FILE__, __LINE__))
#define BN_REALLOC(block, size) (bnReallocFunc(block, size, __FILE__, __LINE__))
#define BN_FREE(block) (bnFreeFunc(block, __FILE__, __LINE__))
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
 * return memory of allocated by malloc.
 * abort if return NULL from malloc
 * @param size
 * @return
 */
void* bnSafeMalloc(size_t size);

/**
 * return memory of allocated by realloc.
 * abort if return NULL from realloc
 * @param block
 * @param newSize
 * @return
 */
void* bnSafeRealloc(void* block, size_t newSize);

/**
 * free of memory by free.
 * declared function for future extension
 * @param block
 */
void bnSafeFree(void* block);

/**
 * return a pdata.
 * exit if pdata equals a null
 * @param pdata
 * @param filename
 * @param lineno
 * @return pdata
 */
void* bnNonNullFunc(void* pdata, const char* filename, int lineno);
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