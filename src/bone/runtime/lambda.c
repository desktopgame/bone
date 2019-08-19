#include "lambda.h"
#include "heap.h"
#include "interpreter.h"
#include "storage.h"

static void free_lambda(bnStorage* storage, bnReference ref, bnObject* obj);

typedef struct bnLambdaBody {
        GHashTable* outer;
        GList* parameters;
        GList* returns;
        bnStringView filename;
        int lineno;
        union {
                bnEnviroment* vEnv;
                bnNativeFunc vFunc;
        } u;
} bnLambdaBody;

/**
 * bnLambda is function pointer in bone.
 */
typedef struct bnLambda {
        bnObject base;
        bnLambdaType type;
        bnLambdaBody* body;
} bnLambda;

bnReference bnNewLambdaFunc(struct bnInterpreter* bone, bnLambdaType type,
                            const char* filename, int lineno) {
        bnReference ref = bnAllocObject(bone->heap);
        bnLambda* ret = bnGetObject(bone->heap, ref);
        bnInitObject(bone, &ret->base, BN_OBJECT_LAMBDA);
        ret->base.freeFunc = free_lambda;
        ret->type = type;
        bnLambdaBody* body = BN_MALLOC(sizeof(bnLambdaBody));
        body->parameters = NULL;
        body->returns = NULL;
        body->filename = 0;
        body->lineno = -1;
        body->outer =
            g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, NULL);
        ret->body = body;
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
        ret->body->u.vFunc = func;
        ret->body->filename = bnIntern(bone->pool, filename);
        ret->body->lineno = lineno;
        while (1) {
                int val = va_arg(ap, int);
                if (val == BN_C_ADD_EXIT) {
                        break;
                }
                if (val == BN_C_ADD_PARAM) {
                        const char* name = va_arg(ap, const char*);
                        bnStringView view = bnIntern(pool, name);
                        ret->body->parameters = g_list_append(
                            ret->body->parameters, (gpointer)view);
                } else if (val == BN_C_ADD_RETURN) {
                        const char* name = va_arg(ap, const char*);
                        bnStringView view = bnIntern(pool, name);
                        ret->body->returns =
                            g_list_append(ret->body->returns, (gpointer)view);
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
        if (g_list_length(lmb->body->parameters) > 0 &&
            (bnStringView)lmb->body->parameters->data ==
                bnIntern(pool, "self")) {
                return true;
        }
#pragma clang diagnostic pop
        return false;
}

bool bnIsVariadicReturn(struct bnStringPool* pool, bnObject* self) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"
        bnLambda* lmb = (bnLambda*)self;
        if (g_list_length(lmb->body->returns) > 0 &&
            (bnStringView)lmb->body->returns->data == bnIntern(pool, "...")) {
                return true;
        }
#pragma clang diagnostic pop
        return false;
}

void bnSetEnviroment(bnObject* obj, bnEnviroment* env) {
        ((bnLambda*)obj)->body->u.vEnv = env;
}

bnEnviroment* bnGetEnviroment(bnObject* obj) {
        return ((bnLambda*)obj)->body->u.vEnv;
}

bnNativeFunc bnGetNativeFunc(bnObject* obj) {
        bnLambda* lambda = ((bnLambda*)obj);
        bnNativeFunc nf = lambda->body->u.vFunc;
        return nf;
}

void bnAddParameter(bnObject* obj, gpointer param) {
        ((bnLambda*)obj)->body->parameters =
            g_list_append(((bnLambda*)obj)->body->parameters, param);
}

GList* bnGetParameterList(bnObject* obj) {
        return ((bnLambda*)obj)->body->parameters;
}

void bnAddReturnValue(bnObject* obj, gpointer retval) {
        ((bnLambda*)obj)->body->returns =
            g_list_append(((bnLambda*)obj)->body->returns, retval);
}

GList* bnGetReturnValueList(bnObject* obj) {
        return ((bnLambda*)obj)->body->returns;
}

GHashTable* bnGetCapturedMap(bnObject* obj) {
        return ((bnLambda*)obj)->body->outer;
}

void bnSetLambdaFileName(bnObject* obj, bnStringView filename) {
        ((bnLambda*)obj)->body->filename = filename;
}

bnStringView bnGetLambdaFileName(bnObject* obj) {
        return ((bnLambda*)obj)->body->filename;
}

void bnSetLambdaLineNumber(bnObject* obj, int line) {
        ((bnLambda*)obj)->body->lineno = line;
}

int bnGetLambdaLineNumber(bnObject* obj) {
        return ((bnLambda*)obj)->body->lineno;
}

bnLambdaType bnGetLambdaType(bnObject* obj) { return ((bnLambda*)obj)->type; }

static void free_lambda(bnStorage* storage, bnReference ref, bnObject* obj) {
        obj->freeFunc = NULL;
        bnLambda* lmb = (bnLambda*)obj;
        g_hash_table_destroy(lmb->body->outer);
        g_list_free(lmb->body->parameters);
        g_list_free(lmb->body->returns);
        if (bnGetLambdaType((bnObject*)lmb) == BN_LAMBDA_SCRIPT) {
                bnDeleteEnviroment(lmb->body->u.vEnv);
        }
        BN_FREE(lmb->body);
        bnDeleteObject(storage, ref, obj);
}