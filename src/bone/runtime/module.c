#include "module.h"
#include "../bone.h"

#if HAVE_DLFCN
#include <dlfcn.h>
#endif

#if HAVE_WINDOWS
#include <windows.h>
#endif

#include <stdio.h>
#include "../util/memory.h"
static gchar* without_extension(gchar* src);

bnModule* bnNewModule(const char* path) {
        bnModule* ret = BN_MALLOC(sizeof(bnModule));
        ret->path = strdup(path);
        ret->handle = NULL;
#if HAVE_DLFCN
        ret->handle = dlopen(path, RTLD_LAZY);
        if (ret->handle == NULL) {
                fprintf(stderr, "%s\n", dlerror());
        }
#elif HAVE_WINDOWS
        ret->handle = LoadLibrary(path);
        if (ret->handle == NULL) {
                fprintf(stderr, "LoadLibrary: %lu\n", GetLastError());
        }
#endif
        return ret;
}

void* bnGetSymbol(bnModule* self, const char* name) {
        if (self->handle == NULL) {
                return NULL;
        }
        char funcName[100];
        gchar* base = without_extension(g_path_get_basename(self->path));
        sprintf(funcName, "%s_%s", base, name);
        void* sym = NULL;
#if HAVE_DLFCN
        unsigned char* ptr = funcName;
        if (g_str_has_prefix(funcName, "lib")) {
                ptr = ptr + (sizeof(char) * 3);
        }
        sym = dlsym(self->handle, ptr);
        if (sym == NULL) {
                fprintf(stderr, "%s\n", dlerror());
        }
#elif HAVE_WINDOWS
        sym = GetProcAddress(self->handle, funcName);
        if (sym == NULL) {
                fprintf(stderr, "GetProcAddress: %lu\n", GetLastError());
        }
#endif
        g_free(base);
        return sym;
}

void bnDeleteModule(bnModule* self) {
        if (self->handle != NULL) {
#if HAVE_DLFCN
                int status = dlclose(self->handle);
                if (status != 0) {
                        fprintf(stderr, "%s\n", dlerror());
                }
#elif HAVE_WINDOWS
                if (!FreeLibrary(self->handle)) {
                        fprintf(stderr, "FreeLibrary: %lu\n", GetLastError());
                }
#endif
        }
        BN_FREE(self->path);
        BN_FREE(self);
}

static gchar* without_extension(gchar* src) {
        int offset = 0;
        while (src[offset] != '\0') {
                if (src[offset] == '.') {
                        break;
                }
                offset++;
        }
        gchar* ret = g_malloc(sizeof(gchar) * (offset + 2));
        memset(ret, '\0', offset + 1);
        memcpy(ret, src, offset);
        g_free(src);
        return ret;
}