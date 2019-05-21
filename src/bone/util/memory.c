#include "memory.h"
#include <string.h>
#include "string.h"

typedef struct bnMemInfo {
        size_t size;
        void* area;
        const char* filename;
        int lineno;
        struct bnMemInfo* prev;
        struct bnMemInfo* next;
} bnMemInfo;

#define BN_MEM_OVER_SIZE (4)
#define BN_MEM_OVER_DATA (0xCD)
#define BN_MEM_CLEAR (0xCC)

static bnMemInfo* new_mem_info(size_t size, const char* filename, int lineno);
static size_t fixed_size(size_t size);
static void* get_user_data(bnMemInfo* self);
static bnMemInfo* find_info(void* block);
static bnMemInfo* root = NULL;

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
#if DEBUG
        if (size == 0) {
                return NULL;
        }
        bnCheckMemoryBounds();
        bnMemInfo* last = NULL;
        if (root == NULL) {
                root = last = new_mem_info(size, filename, lineno);
        } else {
                bnMemInfo* iter = root;
                while (1) {
                        if (iter->next == NULL) {
                                iter->next = last =
                                    new_mem_info(size, filename, lineno);
                                iter->next->prev = iter;
                                break;
                        }
                        iter = iter->next;
                }
        }
        return get_user_data(last);
#else
        return bnSafeMalloc(size);
#endif
}

void* bnReallocFunc(void* block, size_t newSize, const char* filename,
                    int lineno) {
#if DEBUG
        bnMemInfo* a = find_info(block);
        if (a == NULL) {
                return bnSafeRealloc(block, newSize);
        }
        if (a->size == newSize) {
                return get_user_data(a);
        }
        a->filename = filename;
        a->lineno = lineno;
        a->area = bnSafeRealloc(a->area, fixed_size(newSize));
        char* data = a->area;
        memset(data, BN_MEM_OVER_DATA, BN_MEM_OVER_SIZE);
        memset(data + newSize, BN_MEM_OVER_DATA, BN_MEM_OVER_SIZE);
        a->size = newSize;
        return get_user_data(a);
#else
        return bnSafeRealloc(block, newSize);
#endif
}

void bnFreeFunc(void* block, const char* filename, int lineno) {
#if DEBUG
        bnCheckMemoryBounds();
        bnMemInfo* a = find_info(block);
        if (a == NULL) {
                bnSafeFree(block);
                return;
        }
        if (a->prev != NULL) {
                if (a->next != NULL) {
                        a->prev->next = a->next;
                } else {
                        a->prev->next = NULL;
                }
        }
        if (a->next != NULL) {
                if (a->prev != NULL) {
                        a->next->prev = a->prev;
                } else {
                        a->next->prev = NULL;
                }
        }
        if (a == root) {
                root = a->next;
                if (root != NULL) {
                        root->prev = NULL;
                }
        }
        bnSafeFree(a->area);
        bnSafeFree(a);
#else
        bnSafeFree(block);
#endif
}

void bnCheckMemoryBounds() {
        bnMemInfo* iter = root;
        while (iter != NULL) {
                unsigned char* data = iter->area;
                for (int i = 0; i < BN_MEM_OVER_SIZE; i++) {
                        unsigned char a = data[i];
                        unsigned char b =
                            data[iter->size + i + BN_MEM_OVER_SIZE];
                        if (a != BN_MEM_OVER_DATA || b != BN_MEM_OVER_DATA) {
                                abort();
                        }
                }
                iter = iter->next;
        }
}

void bnDumpMemoryLeaks(FILE* fp) {
        bnCheckMemoryBounds();
        bnMemInfo* iter = root;
        int items = 0;
        while (iter != NULL) {
                fprintf(fp, "%s <%d> %p\n",
                        iter->filename + bnLastPathComponent(iter->filename),
                        iter->lineno, iter->area);
                iter = iter->next;
                items++;
        }
        fprintf(fp, "memory leaks: %d\n", items);
}

static bnMemInfo* new_mem_info(size_t size, const char* filename, int lineno) {
        bnMemInfo* ret = bnSafeMalloc(sizeof(bnMemInfo));
        char* data = bnSafeMalloc(fixed_size(size));
        void* vdata = data;
        memset(data, BN_MEM_CLEAR, fixed_size(size));
        memset(data, BN_MEM_OVER_DATA, BN_MEM_OVER_SIZE);
        memset(data + size + BN_MEM_OVER_SIZE, BN_MEM_OVER_DATA,
               BN_MEM_OVER_SIZE);
        ret->size = size;
        ret->area = data;
        ret->filename = filename;
        ret->lineno = lineno;
        ret->prev = NULL;
        ret->next = NULL;
        return ret;
}

static size_t fixed_size(size_t size) { return size + (BN_MEM_OVER_SIZE * 2); }

static void* get_user_data(bnMemInfo* self) {
        char* data = self->area;
        return data + BN_MEM_OVER_SIZE;
}

static bnMemInfo* find_info(void* block) {
        bnMemInfo* iter = root;
        while (iter != NULL) {
                char* data = iter->area;
                if (block == (data + BN_MEM_OVER_SIZE)) {
                        return iter;
                }
                iter = iter->next;
        }
        return NULL;
}