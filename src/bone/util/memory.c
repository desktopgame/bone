#include "memory.h"
#include <string.h>
#include "string.h"

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

void* bnNonNullFunc(void* pdata, const char* filename, int lineno) {
        if (pdata == NULL) {
                fprintf(stderr, "pointer is null: <%d>%s\n", lineno, filename);
                abort();
        }
        return pdata;
}
