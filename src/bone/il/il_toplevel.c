#include "il_toplevel.h"
#include "../runtime/enviroment.h"
#include "compile_cache.h"
#include "il_stmt_all.h"

bnILToplevel* bnNewILTopLevel() {
        bnILToplevel* ret = BN_MALLOC(sizeof(bnILToplevel));
        ret->Xstatements = g_ptr_array_new_full(2, bnDeleteILStatement);
        return ret;
}

void bnDeleteILTopLevel(bnILToplevel* self) {
        g_ptr_array_free(self->Xstatements, TRUE);
        BN_FREE(self);
}

void bnDumpILTopLevel(FILE* fp, struct bnStringPool* pool, bnILToplevel* self,
                      int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "TopLevel\n");
        for (int i = 0; i < self->Xstatements->len; i++) {
                bnILStatement* ilstmt = g_ptr_array_index(self->Xstatements, i);
                bnDumpILStatement(fp, pool, ilstmt, depth + 1);
        }
}

void bnGenerateILTopLevel(struct bnInterpreter* bone, bnILToplevel* self,
                          bnEnviroment* env) {
        bnCompileCache* ccache = bnNewCompileCache();
        for (int i = 0; i < self->Xstatements->len; i++) {
                bnILStatement* ilstmt = g_ptr_array_index(self->Xstatements, i);
                bnGenerateILStatement(bone, ilstmt, env, ccache);
        }
        bnDeleteCompileCache(ccache);
}