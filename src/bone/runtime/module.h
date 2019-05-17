#ifndef BONE_RUNTIME_MODULE_H
#define BONE_RUNTIME_MODULE_H
#include "../glib.h"

typedef struct bnModule {
        char* path;
        void* handle;
} bnModule;

bnModule* bnNewModule(const char* path);

void* bnGetSymbol(bnModule* self, const char* name);

void bnDeleteModule(bnModule* self);
#endif