#include "object.h"

void bnInitObject(bnObject* self) {
        self->table = g_hash_table_new(g_int_hash, g_int_equal);
        self->mark = false;
}

bnObject* bnNewObject() {
        bnObject* ret = BN_MALLOC(sizeof(bnObject));
        bnInitObject(ret);
        return ret;
}

void bnDefine(bnObject* self, const char* name, bnObject* value) {
        g_hash_table_insert(self->table, name, value);
}

void bnDeleteObject(bnObject* self) {}