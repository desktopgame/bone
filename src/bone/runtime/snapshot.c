#include "snapshot.h"
#include "../util/memory.h"

bnSnapShot* bnNewSnapShot(int pc) {
        bnSnapShot* ret = BN_MALLOC(sizeof(bnSnapShot));
        ret->pc = pc;
        ret->table =
            g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, NULL);
        return ret;
}

void bnDeleteSnapShot(bnSnapShot* self) {
        g_hash_table_destroy(self->table);
        BN_FREE(self);
}