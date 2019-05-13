#include "std.h"

#ifdef __APPLE__
#include <signal.h>
#define CAN_BREAK (1)
#endif

#include <assert.h>
#include "../bone.h"
#include "../parse/ast2il.h"
#include "../parse/parser.h"
#include "../util/fmt.h"
#include "array.h"
#include "bool.h"
#include "enviroment.h"
#include "frame.h"
#include "integer.h"
#include "interpreter.h"
#include "lambda.h"
#include "object.h"
#include "string.h"
#include "vm.h"

static void _throw(bnInterpreter* bone, bnFrame* frame, const char* str);
static bool file_exists(const char* path);

// only in debug build
#if DEBUG
void bnStdDebugAssert(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, "internal error");
        }
        bnBool* cond = a;
        if (!cond->value) {
                abort();
        }
}

void bnStdDebugDie(bnInterpreter* bone, bnFrame* frame) { abort(); }

void bnStdDebugPrint(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_STRING) {
                _throw(bone, frame, "internal error");
        }
        bnString* str = a;
        const char* cstr = bnView2Str(bone->pool, str->value);
        printf("  [%s]", cstr);
        fflush(stdout);
}

void bnStdDebugPrintln(bnInterpreter* bone, bnFrame* frame) {
        bnStdDebugPrint(bone, frame);
        printf("\n");
}

void bnStdDebugBreak(bnInterpreter* bone, bnFrame* frame) {
#if CAN_BREAK
        raise(SIGINT);
#else
        // please set break point at here
        abort();
#endif
}

void bnStdDebugDumpTable(bnInterpreter* bone, bnFrame* frame) {
        GHashTableIter hashIter;
        g_hash_table_iter_init(&hashIter, frame->variableTable);
        gpointer k, v;
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                const char* kStr = (const char*)bnView2Str(bone->pool, k);
                fprintf(stdout, "%s:", kStr);
                bnPrintObject(stdout, bone, v);
                fprintf(stdout, "\n");
        }
}

static void showInfo(bnInterpreter* bone, bnObject* a, int depth) {
        if (depth > 4) {
                return;
        }
        GHashTableIter hashIter;
        gpointer k, v;
        g_hash_table_iter_init(&hashIter, a->table);
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                const char* strk = bnView2Str(bone->pool, k);
                for (int i = 0; i < depth; i++) fprintf(BN_STDOUT, "    ");
                // bnFindent(BN_STDOUT, depth);
                fprintf(BN_STDOUT, "%s", strk);
                fprintf(BN_STDOUT, "[");
                bnPrintObject(BN_STDOUT, bone, v);
                fprintf(BN_STDOUT, "]");
                fprintf(BN_STDOUT, "\n");
                if (a != v) {
                        showInfo(bone, v, depth + 1);
                }
        }
}

void bnStdDebugShowInfo(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        showInfo(bone, a, 0);
}
#endif

// Built-in

void bnStdSystemInclude(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_STRING) {
                _throw(bone, frame, "should be path is string");
        }
        bnStringView pathView = ((bnString*)a)->value;
        const char* pathStr = bnView2Str(bone->pool, pathView);
        if (!file_exists(pathStr)) {
                bnFormatThrow(bone, "`%s` is not found", pathStr);
        }
        // parse file
        bnAST* ast = bnParseFile(bone->pool, pathStr);
        if (ast == NULL) {
                _throw(bone, frame, "internal error");
        }
        bnILToplevel* iltop = bnAST2IL(ast);
        if (iltop == NULL) {
                _throw(bone, frame, "internal error");
        }
        // gen code
        bnFrame* sub = bnSubFrame(frame);
        bnEnviroment* env = bnNewEnviroment();
        bnGenerateILTopLevel(bone, iltop, env);
        bnInjectFrame(frame->variableTable, sub);
        bnExecute(bone, env, sub);
        bnInjectFrame(sub->variableTable, frame);
        bnDeleteAST(ast);
        bnDeleteILTopLevel(iltop);
        bnDeleteEnviroment(env);
        bnDeleteFrame(sub);
}

void bnStdSystemLoad(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_STRING) {
                _throw(bone, frame, "should be path is string");
        }
        bnStringView pathView = ((bnString*)a)->value;
        const char* pathStr = bnView2Str(bone->pool, pathView);
        if (!file_exists(pathStr)) {
                bnFormatThrow(bone, "`%s` is not found", pathStr);
        }
        // parse file
        bnAST* ast = bnParseFile(bone->pool, pathStr);
        if (ast == NULL) {
                _throw(bone, frame, "internal error");
        }
        bnILToplevel* iltop = bnAST2IL(ast);
        if (iltop == NULL) {
                _throw(bone, frame, "internal error");
        }
        // gen code
        bnFrame* sub = bnSubFrame(frame);
        bnEnviroment* env = bnNewEnviroment();
        bnGenerateILTopLevel(bone, iltop, env);
        bnWriteDefaults(bone, sub, bone->pool);
        // get default hash
        GList* keys = NULL;
        GHashTableIter hashIter;
        gpointer k, v;
        g_hash_table_iter_init(&hashIter, sub->variableTable);
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                keys = g_list_append(keys, v);
        }
        bnExecute(bone, env, sub);
        assert(frame->panic == NULL);
        // remove default
        GList* listIter = keys;
        while (listIter != NULL) {
                g_hash_table_remove(sub->variableTable, listIter->data);
                listIter = listIter->next;
        }
        g_list_free(keys);
        bnInjectFrame(sub->variableTable, frame);
        bnDeleteAST(ast);
        bnDeleteILTopLevel(iltop);
        bnDeleteEnviroment(env);
        bnDeleteFrame(sub);
}

void bnStdSystemObject(bnInterpreter* bone, bnFrame* frame) {
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewObject(bone->heap));
}

void bnStdSystemArray(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewArray(bone, ((bnInteger*)a)->value));
}

void bnStdSystemExternVar(bnInterpreter* bone, bnFrame* frame) {
        bnObject* name = bnPopStack(frame->vStack);
        if (name->type != BN_OBJECT_STRING) {
                _throw(bone, frame, "internal error");
        }
        gpointer v =
            g_hash_table_lookup(bone->externTable, ((bnString*)name)->value);
        bnObject* obj = v;
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             obj);
}

void bnStdSystemExternDef(bnInterpreter* bone, bnFrame* frame) {
        bnObject* name = bnPopStack(frame->vStack);
        bnObject* params = bnPopStack(frame->vStack);
        bnObject* returns = bnPopStack(frame->vStack);
        if (name->type != BN_OBJECT_STRING) {
                _throw(bone, frame, "internal error");
        }
        // find from extern table
        gpointer v =
            g_hash_table_lookup(bone->externTable, ((bnString*)name)->value);
        bnObject* obj = v;
        if (obj->type != BN_OBJECT_LAMBDA) {
                _throw(bone, frame, "internal error");
        }
        // check parameters
        bnLambda* lambda = obj;
        bnArray* paraArr = params;
        if (paraArr->arr->len != g_list_length(lambda->parameters)) {
                _throw(bone, frame, "internal error");
        }
        // check returns
        bnArray* retuArr = returns;
        if (retuArr->arr->len != g_list_length(lambda->returns)) {
                _throw(bone, frame, "internal error");
        }
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             obj);
}

void bnStdSystemPanic(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnThrow(bone, a, BN_JMP_CODE_EXCEPTION);
}

void bnStdSystemRecover(bnInterpreter* bone, bnFrame* frame) {
        g_hash_table_replace(
            frame->variableTable, bnIntern(bone->pool, "ret"),
            g_hash_table_lookup(frame->variableTable,
                                bnIntern(bone->pool, "false")));
        if (frame->prev->panic != NULL) {
                g_hash_table_replace(frame->variableTable,
                                     bnIntern(bone->pool, "ret"),
                                     frame->prev->panic);
                frame->prev->panic = NULL;
        }
}
// Bool

void bnStdBoolFuncCall(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolPositive(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolNegative(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolChilda(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolNot(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, "internal error");
        }
        bnBool* b = a;
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             b->r);
}

void bnStdBoolPlus(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolMinus(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolMultiply(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolDivide(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolModulo(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolBitAnd(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolBitOr(bnInterpreter* bone, bnFrame* frame);

// void bnStdBoolLogicAnd(struct bnInterpreter* bone, struct bnFrame* frame);

// void bnStdBoolLogicOr(struct bnInterpreter* bone, struct bnFrame* frame);

void bnStdBoolExcOr(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolLShift(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolRShift(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolGT(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolGE(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolLT(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolLE(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolEqual(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolNotEqual(bnInterpreter* bone, bnFrame* frame);

// Integer

void bnStdIntegerFuncCall(bnInterpreter* bone, bnFrame* frame) {
        _throw(bone, frame, "internal error");
}

void bnStdIntegerPositive(bnInterpreter* bone, bnFrame* frame) {
        _throw(bone, frame, "internal error");
}

void bnStdIntegerNegative(bnInterpreter* bone, bnFrame* frame) {
        _throw(bone, frame, "internal error");
}

void bnStdIntegerChilda(bnInterpreter* bone, bnFrame* frame) {
        _throw(bone, frame, "internal error");
}

void bnStdIntegerNot(bnInterpreter* bone, bnFrame* frame) {
        _throw(bone, frame, "internal error");
}

void bnStdIntegerPlus(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai + bi));
}

void bnStdIntegerMinus(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai - bi));
}

void bnStdIntegerMultiply(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai * bi));
}

void bnStdIntegerDivide(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai / bi));
}

void bnStdIntegerModulo(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai % bi));
}

void bnStdIntegerBitAnd(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai & bi));
}

void bnStdIntegerBitOr(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, ai | bi));
}

// void bnStdIntegerLogicAnd(bnInterpreter* bone, bnFrame* frame){}

// void bnStdIntegerLogicOr(bnInterpreter* bone, bnFrame* frame){}

void bnStdIntegerExcOr(bnInterpreter* bone, bnFrame* frame) {}

void bnStdIntegerLShift(bnInterpreter* bone, bnFrame* frame) {}

void bnStdIntegerRShift(bnInterpreter* bone, bnFrame* frame) {}

void bnStdIntegerGT(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai > bi));
}

void bnStdIntegerGE(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai >= bi));
}

void bnStdIntegerLT(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai < bi));
}

void bnStdIntegerLE(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai <= bi));
}

void bnStdIntegerEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai == bi));
}

void bnStdIntegerNotEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        int ai = ((bnInteger*)a)->value;
        int bi = ((bnInteger*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai != bi));
}

void bnStdIntegerToString(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        char buf[100];
        sprintf(buf, "%d", ((bnInteger*)a)->value);
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewString(bone, bnIntern(bone->pool, buf)));
}

// String

void bnStdStringFuncCall(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringPositive(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringNegative(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringChilda(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringNot(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringPlus(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringMinus(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringMultiply(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringDivide(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringModulo(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringBitAnd(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringBitOr(bnInterpreter* bone, bnFrame* frame) {}

// void bnStdStringLogicAnd(bnInterpreter* bone, bnFrame* frame){}

// void bnStdStringLogicOr(bnInterpreter* bone, bnFrame* frame){}

void bnStdStringExcOr(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringLShift(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringRShift(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringGT(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringGE(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringLT(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringLE(bnInterpreter* bone, bnFrame* frame) {}

void bnStdStringEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_STRING || b->type != BN_OBJECT_STRING) {
                _throw(bone, frame, "internal error");
        }
        bnStringView ai = ((bnString*)a)->value;
        bnStringView bi = ((bnString*)b)->value;
        // bnPushStack(frame->vStack, bnNewInteger(bone, ai + bi));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai == bi));
}

void bnStdStringNotEqual(bnInterpreter* bone, bnFrame* frame) {}

// Array
void bnStdArrayArraySet(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        bnObject* c = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_ARRAY || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        bnArray* arr = a;
        bnInteger* idx = b;
        bnInteger* val = c;
        g_ptr_array_index(arr->arr, idx->value) = c;
}

void bnStdArrayArrayGet(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_ARRAY || b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "internal error");
        }
        bnArray* arr = a;
        bnInteger* idx = b;
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             g_ptr_array_index(arr->arr, idx->value));
}

static void _throw(bnInterpreter* bone, bnFrame* frame, const char* str) {
        bnThrow(bone, bnNewString(bone, bnIntern(bone->pool, str)),
                BN_JMP_CODE_EXCEPTION);
}

static bool file_exists(const char* path) {
        FILE* fp = fopen(path, "r");
        bool ret = fp != NULL;
        if (fp != NULL) {
                fclose(fp);
        }
        return ret;
}