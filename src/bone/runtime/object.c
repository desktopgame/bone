#include "object.h"

void bnInitObject(bnObject* self, bnObjectType type) {
        self->table = g_hash_table_new(g_int_hash, g_int_equal);
        self->mark = false;
        self->type = type;
}

bnObject* bnNewObject() {
        bnObject* ret = BN_MALLOC(sizeof(bnObject));
        bnInitObject(ret, BN_OBJECT_PROTO);
        return ret;
}

void bnDefine(bnObject* self, bnStringView name, bnObject* value) {
        g_hash_table_insert(self->table, (int)name, value);
}

void bnDeleteObject(bnObject* self) {}