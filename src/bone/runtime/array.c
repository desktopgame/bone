#include "array.h"
#include "interpreter.h"

bnArray* bnNewArray(bnInterpreter* bone, int size) {
        bnArray* ret = BN_MALLOC(sizeof(bnArray));
        bnInitObject(&ret->base, BN_OBJECT_ARRAY);
        ret->arr = g_ptr_array_new();
        ret->size = size;
        for (int i = 0; i < size; i++) {
                g_ptr_array_add(ret->arr, NULL);
        }
        return ret;
}

void bnDeleteArray(bnArray* self) {}