#ifndef BONE_IL_ILTOPLEVEL_H
#define BONE_IL_ILTOPLEVEL_H
#include "../bone.h"
/**
 * bnILToplevel is structure of toplevel of source code.
 */
typedef struct bnILToplevel {
        GList* statementList;
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
 * @param self
 * @param depth
 */
void bnDumpILTopLevel(FILE* fp, bnILToplevel* self, int depth);
#endif