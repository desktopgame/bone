#ifndef BONE_MEMORY_H
#define BONE_MEMORY_H
#include <stdio.h>
#include <stdlib.h>

#if DEBUG
#define BN_MALLOC(size) (bnMallocFunc(size, __FILE__, __LINE__))
#define BN_REALLOC(block, size) (bnReallocFunc(block, size, __FILE__, __LINE__))
#define BN_FREE(block) (bnFreeFunc(block, __FILE__, __LINE__))
#else
#define BN_MALLOC(size) (bnSafeMalloc(size))
#define BN_REALLOC(block, size) (bnSafeRealloc(block, size))
#define BN_FREE(block) bnSafeFree(block)
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