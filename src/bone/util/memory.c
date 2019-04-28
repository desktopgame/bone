#include "memory.h"

void* bnSafeMalloc(size_t size) {
        void* ret = malloc(size);
        if (ret == NULL) {
                abort();
        }
        return ret;
}

void* bnSafeRealloc(void* block, size_t newSize) {
        void* ret = realloc(block, newSize);
        if (ret == NULL) {
                abort();
        }
        return ret;
}

void bnSafeFree(void* block) { free(block); }

void* bnMallocFunc(size_t size, const char* filename, int lineno) {
        return bnSafeMalloc(size);
}

void* bnReallocFunc(void* block, size_t newSize, const char* filename,
                    int lineno) {
        return bnSafeRealloc(block, newSize);
}

void bnFreeFunc(void* block, const char* filename, int lineno) {
        bnSafeFree(block);
}