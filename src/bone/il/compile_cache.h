#ifndef BONE_IL_COMPILECACHE_H
#define BONE_IL_COMPILECACHE_H
#include "../util/stack.h"

/**
 * bnCompileCache is cache of compile.
 */
typedef struct bnCompileCache {
        bnStack* whileStartStack;
        bnStack* whileEndStack;
} bnCompileCache;

/**
 * @return
 */
bnCompileCache* bnNewCompileCache();

/**
 * @param self
 */
void bnDeleteCompileCache(bnCompileCache* self);
#endif