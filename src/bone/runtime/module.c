#include "module.h"
#include <dlfcn.h>
#include <stdio.h>
#include "../util/memory.h"
static gchar* without_extension(gchar* src);

bnModule* bnNewModule(const char* path) {
        bnModule* ret = BN_MALLOC(sizeof(bnModule));
        ret->path = strdup(path);
        ret->handle = NULL;
#if __APPLE__
        ret->handle = dlopen(path, RTLD_LAZY);
#endif
        return ret;
}

void* bnGetSymbol(bnModule* self, const char* name) {
        if (self->handle == NULL) {
                return NULL;
        }
        char destroyFuncName[100];
        gchar* base = without_extension(g_path_get_basename(self->path));
        sprintf(destroyFuncName, "%s_%s", base, name);
        void* sym = NULL;
#if __APPLE__
        sym = dlsym(self->handle, destroyFuncName);
#endif
        g_free(base);
        return sym;
}

void bnDeleteModule(bnModule* self) {
        if (self->handle != NULL) {
#if __APPLE__
                dlclose(self->handle);
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
        gchar* ret = BN_MALLOC(sizeof(gchar) * (offset + 1));
        memset(ret, '\0', offset + 1);
        memcpy(ret, src, offset);
        g_free(src);
        return ret;
}