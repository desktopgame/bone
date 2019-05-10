#include "lambda.h"
#include "interpreter.h"

static void free_lambda(bnObject* obj);

bnLambda* bnNewLambda(bnInterpreter* bone, bnLambdaType type) {
        bnLambda* ret = BN_MALLOC(sizeof(bnLambda));
        bnInitObject(bone->heap, &ret->base, BN_OBJECT_LAMBDA);
        ret->base.freeFunc = free_lambda;
        ret->type = type;
        ret->parameters = NULL;
        ret->returns = NULL;
        ret->outer =
            g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, NULL);
        return ret;
}

bnLambda* bnNewLambdaFromCFunc(struct bnInterpreter* bone, bnNativeFunc func,
                               struct bnStringPool* pool, ...) {
        va_list ap;
        va_start(ap, func);
        bnLambda* ret = bnNewLambda(bone, BN_LAMBDA_NATIVE);
        ret->u.vFunc = func;
        while (1) {
                int val = va_arg(ap, int);
                if (val == BN_C_ADD_EXIT) {
                        break;
                }
                if (val == BN_C_ADD_PARAM) {
                        const char* name = va_arg(ap, const char*);
                        bnStringView view = bnIntern(pool, name);
                        ret->parameters = g_list_append(ret->parameters, view);
                } else if (val == BN_C_ADD_RETURN) {
                        const char* name = va_arg(ap, const char*);
                        bnStringView view = bnIntern(pool, name);
                        ret->returns = g_list_append(ret->returns, view);
                }
        }

        va_end(ap);
        return ret;
}

bool bnIsInstanceBaseLambda(struct bnStringPool* pool, bnLambda* self) {
        if (g_list_length(self->parameters) > 0 &&
            self->parameters->data == bnIntern(pool, "self")) {
                return true;
        }
        return false;
}

bool bnIsVariadicReturn(struct bnStringPool* pool, bnLambda* self) {
        if (g_list_length(self->returns) > 0 &&
            self->returns->data == bnIntern(pool, "...")) {
                return true;
        }
        return false;
}

static void free_lambda(bnObject* obj) {
        obj->freeFunc = NULL;
        bnLambda* lmb = obj;
        g_hash_table_destroy(lmb->outer);
        g_list_free(lmb->parameters);
        g_list_free(lmb->returns);
        bnDeleteObject(obj);
}