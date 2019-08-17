#include "lambda.h"
#include "heap.h"
#include "interpreter.h"
#include "storage.h"

static void free_lambda(bnStorage* storage, bnReference ref, bnObject* obj);

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

bnReference bnNewLambdaFunc(struct bnInterpreter* bone, bnLambdaType type,
                            const char* filename, int lineno) {
        bnReference ref = bnAllocObject(bone->heap);
        bnLambda* ret = bnGetObject(bone->heap, ref);
        bnInitObject(bone, &ret->base, BN_OBJECT_LAMBDA);
        ret->base.freeFunc = free_lambda;
        ret->type = type;
        ret->parameters = NULL;
        ret->returns = NULL;
        ret->filename = 0;
        ret->lineno = -1;
        ret->outer =
            g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, NULL);
        return ref;
}

bnReference bnNewLambdaFromCFuncFunc(struct bnInterpreter* bone,
                                     bnNativeFunc func,
                                     struct bnStringPool* pool,
                                     const char* filename, int lineno, ...) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
        va_list ap;
        va_start(ap, lineno);
        bnReference ref =
            bnNewLambdaFunc(bone, BN_LAMBDA_NATIVE, filename, lineno);
        bnLambda* ret = bnGetObject(bone->heap, ref);
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
                        ret->parameters =
                            g_list_append(ret->parameters, (gpointer)view);
                } else if (val == BN_C_ADD_RETURN) {
                        const char* name = va_arg(ap, const char*);
                        bnStringView view = bnIntern(pool, name);
                        ret->returns =
                            g_list_append(ret->returns, (gpointer)view);
                }
        }

        va_end(ap);
#pragma clang diagnostic pop
        return ref;
}

bool bnIsInstanceBaseLambda(struct bnStringPool* pool, bnObject* self) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
        bnLambda* lmb = (bnLambda*)self;
        if (g_list_length(lmb->parameters) > 0 &&
            (bnStringView)lmb->parameters->data == bnIntern(pool, "self")) {
                return true;
        }
#pragma clang diagnostic pop
        return false;
}

bool bnIsVariadicReturn(struct bnStringPool* pool, bnObject* self) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
        bnLambda* lmb = (bnLambda*)self;
        if (g_list_length(lmb->returns) > 0 &&
            (bnStringView)lmb->returns->data == bnIntern(pool, "...")) {
                return true;
        }
#pragma clang diagnostic pop
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

static void free_lambda(bnStorage* storage, bnReference ref, bnObject* obj) {
        obj->freeFunc = NULL;
        bnLambda* lmb = (bnLambda*)obj;
        g_hash_table_destroy(lmb->outer);
        g_list_free(lmb->parameters);
        g_list_free(lmb->returns);
        if (bnGetLambdaType((bnObject*)lmb) == BN_LAMBDA_SCRIPT) {
                bnDeleteEnviroment(lmb->u.vEnv);
        }
        bnDeleteObject(storage, ref, obj);
}