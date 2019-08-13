#include "lambda.h"
#include "interpreter.h"

static void free_lambda(bnObject* obj);

/**
 * bnLambda is function pointer in bone.
 */
typedef struct bnLambda {
        bnObject base;
        bnLambdaType type;
        GHashTable* outer;
        GList* parameters;
        GList* returns;
        bnStringView filename;
        int lineno;
        union {
                bnEnviroment* vEnv;
                bnNativeFunc vFunc;
        } u;
} bnLambda;

bnObject* bnNewLambdaFunc(struct bnInterpreter* bone, bnLambdaType type,
                          const char* filename, int lineno) {
        bnLambda* ret = bnMallocFunc(sizeof(bnLambda), filename, lineno);
        bnInitObject(bone, &ret->base, BN_OBJECT_LAMBDA);
        ret->base.freeFunc = free_lambda;
        ret->type = type;
        ret->parameters = NULL;
        ret->returns = NULL;
        ret->filename = 0;
        ret->lineno = -1;
        ret->outer =
            g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, NULL);
        return ret;
}

bnObject* bnNewLambdaFromCFuncFunc(struct bnInterpreter* bone,
                                   bnNativeFunc func, struct bnStringPool* pool,
                                   const char* filename, int lineno, ...) {
        va_list ap;
        va_start(ap, lineno);
        bnLambda* ret =
            bnNewLambdaFunc(bone, BN_LAMBDA_NATIVE, filename, lineno);
        ret->u.vFunc = func;
        ret->filename = bnIntern(bone->pool, filename);
        ret->lineno = lineno;
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

bool bnIsInstanceBaseLambda(struct bnStringPool* pool, bnObject* self) {
        bnLambda* lmb = (bnLambda*)self;
        if (g_list_length(lmb->parameters) > 0 &&
            lmb->parameters->data == bnIntern(pool, "self")) {
                return true;
        }
        return false;
}

bool bnIsVariadicReturn(struct bnStringPool* pool, bnObject* self) {
        bnLambda* lmb = (bnLambda*)self;
        if (g_list_length(lmb->returns) > 0 &&
            lmb->returns->data == bnIntern(pool, "...")) {
                return true;
        }
        return false;
}

void bnSetEnviroment(bnObject* obj, bnEnviroment* env) {
        ((bnLambda*)obj)->u.vEnv = env;
}

bnEnviroment* bnGetEnviroment(bnObject* obj) {
        return ((bnLambda*)obj)->u.vEnv;
}

bnNativeFunc bnGetNativeFunc(bnObject* obj) {
        return ((bnLambda*)obj)->u.vFunc;
}

void bnAddParameter(bnObject* obj, gpointer param) {
        ((bnLambda*)obj)->parameters =
            g_list_append(((bnLambda*)obj)->parameters, param);
}

GList* bnGetParameterList(bnObject* obj) {
        return ((bnLambda*)obj)->parameters;
}

void bnAddReturnValue(bnObject* obj, gpointer retval) {
        ((bnLambda*)obj)->returns =
            g_list_append(((bnLambda*)obj)->returns, retval);
}

GList* bnGetReturnValueList(bnObject* obj) { return ((bnLambda*)obj)->returns; }

GHashTable* bnGetCapturedMap(bnObject* obj) { return ((bnLambda*)obj)->outer; }

void bnSetLambdaFileName(bnObject* obj, bnStringView filename) {
        ((bnLambda*)obj)->filename = filename;
}

bnStringView bnGetLambdaFileName(bnObject* obj) {
        return ((bnLambda*)obj)->filename;
}

void bnSetLambdaLineNumber(bnObject* obj, int line) {
        ((bnLambda*)obj)->lineno = line;
}

int bnGetLambdaLineNumber(bnObject* obj) { return ((bnLambda*)obj)->lineno; }

bnLambdaType bnGetLambdaType(bnObject* obj) { return ((bnLambda*)obj)->type; }

static void free_lambda(bnObject* obj) {
        obj->freeFunc = NULL;
        bnLambda* lmb = obj;
        g_hash_table_destroy(lmb->outer);
        g_list_free(lmb->parameters);
        g_list_free(lmb->returns);
        if (bnGetLambdaType(lmb) == BN_LAMBDA_SCRIPT) {
                bnDeleteEnviroment(lmb->u.vEnv);
        }
        bnDeleteObject(obj);
}