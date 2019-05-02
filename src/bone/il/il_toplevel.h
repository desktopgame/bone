#ifndef BONE_IL_ILTOPLEVEL_H
#define BONE_IL_ILTOPLEVEL_H
#include "../bone.h"
struct bnEnviroment;
/**
 * bnILToplevel is structure of toplevel of source code.
 */
typedef struct bnILToplevel {
        GList* statements;
} bnILToplevel;

/**
 * return new instance of bnILToplevel.
 * @param cond
 * @return
 */
bnILToplevel* bnNewILTopLevel();

/**
 * free a bnILToplevel.
 * @param self
 */
void bnDeleteILTopLevel(bnILToplevel* self);

/**
 * @param fp
 * @param pool
 * @param self
 * @param depth
 */
void bnDumpILTopLevel(FILE* fp, struct bnStringPool* pool, bnILToplevel* self,
                      int depth);

/**
 * @param self
 * @param env
 */
void bnGenerateILTopLevel(bnILToplevel* self, struct bnEnviroment* env);
#endif