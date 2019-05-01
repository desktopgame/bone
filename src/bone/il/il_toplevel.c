#include "il_toplevel.h"
#include "il_stmt_all.h"

bnILToplevel* bnNewILTopLevel() {
        bnILToplevel* ret = BN_MALLOC(sizeof(bnILToplevel));
        ret->statements = NULL;
        return ret;
}

void bnDeleteILTopLevel(bnILToplevel* self) {
        g_list_free_full(self->statements, bnDeleteILStatement);
        BN_FREE(self);
}

void bnDumpILTopLevel(FILE* fp, bnILToplevel* self, int depth) {
        bnFindent(fp, depth);
        fprintf(fp, "TopLevel\n");
        GList* iter = self->statements;
        while (iter != NULL) {
                bnILStatement* ilstmt = iter->data;
                bnDumpILStatement(fp, ilstmt, depth + 1);
                iter = iter->next;
        }
}