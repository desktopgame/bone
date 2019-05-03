#include "lambda.h"

bnLambda* bnNewLambda(bnLambdaType type) {
        bnLambda* ret = BN_MALLOC(sizeof(bnLambda));
        bnInitObject(&ret->base, BN_OBJECT_LAMBDA);
        ret->type = type;
        ret->instanceBase = false;
        ret->returns = NULL;
        ret->outer =
            g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, NULL);
        return ret;
}

bnLambda* bnNewLambdaFromCFunc(bnNativeFunc func, struct bnStringPool* pool,
                               ...) {
        va_list ap;
        va_start(ap, func);
        bnLambda* ret = bnNewLambda(BN_LAMBDA_NATIVE);
        ret->u.vFunc = func;
        while (1) {
                const char* name = va_arg(ap, const char*);
                if (name == NULL) {
                        break;
                }
                bnStringView view = bnIntern(pool, name);
                ret->returns = g_list_append(ret->returns, view);
        }
        va_end(ap);
        return ret;
}

void bnDeleteLambda(bnLambda* self) {}