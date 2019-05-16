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
#include "../util/io.h"
#include "array.h"
#include "bool.h"
#include "char.h"
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
static bool compare_list_array(GList* a, GPtrArray* b);

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
                bnFormatThrow(bone, "syntax error in `%s`", pathStr);
        }
        bnILToplevel* iltop = bnAST2IL(ast);
        // gen code
        bnFrame* sub = bnSubFrame(frame);
        bnEnviroment* env = bnNewEnviroment(bnIntern(bone->pool, pathStr));
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
                bnFormatThrow(bone, "syntax error in `%s`", pathStr);
        }
        bnILToplevel* iltop = bnAST2IL(ast);
        // gen code
        bnFrame* sub = bnSubFrame(frame);
        bnEnviroment* env = bnNewEnviroment(bnIntern(bone->pool, pathStr));
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
                             bnNewObject(bone));
}

void bnStdSystemString(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_ARRAY) {
                bnFormatThrow(bone, "should be `array` is array");
        }
        bnArray* aArr = a;
        GString* gbuf = g_string_new("");
        for (int i = 0; i < aArr->arr->len; i++) {
                bnObject* e = g_ptr_array_index(aArr->arr, i);
                if (e->type != BN_OBJECT_CHAR) {
                        bnFormatThrow(bone, "should be `array[%d]` is char", i);
                }
                bnChar* eChar = e;
                g_string_append_c(gbuf, eChar->value);
        }
        bnStringView gview = bnIntern(bone->pool, gbuf->str);
        g_string_free(gbuf, TRUE);
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewString(bone, gview));
}

void bnStdSystemArray(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "should be `length` is integer");
        }
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewArray(bone, ((bnInteger*)a)->value));
}

void bnStdSystemExternVar(bnInterpreter* bone, bnFrame* frame) {
        bnObject* name = bnPopStack(frame->vStack);
        if (name->type != BN_OBJECT_STRING) {
                _throw(bone, frame, "should be `name` is string");
        }
        gpointer v =
            g_hash_table_lookup(bone->externTable, ((bnString*)name)->value);
        if (v == NULL) {
                bnFormatThrow(bone, "not bound variable: `%s`",
                              bnView2Str(bone->pool, ((bnString*)name)->value));
        }
        bnObject* obj = v;
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             obj);
}

void bnStdSystemExternDef(bnInterpreter* bone, bnFrame* frame) {
        bnObject* name = bnPopStack(frame->vStack);
        bnObject* params = bnPopStack(frame->vStack);
        bnObject* returns = bnPopStack(frame->vStack);
        if (name->type != BN_OBJECT_STRING) {
                _throw(bone, frame, "should be `name` is string");
        }
        if (params->type != BN_OBJECT_ARRAY) {
                _throw(bone, frame, "should be `params` is array");
        }
        if (returns->type != BN_OBJECT_ARRAY) {
                _throw(bone, frame, "should be `returns` is array");
        }
        // find from extern table
        gpointer v =
            g_hash_table_lookup(bone->externTable, ((bnString*)name)->value);
        if (v == NULL) {
                bnFormatThrow(bone, "not bound variable: `%s`",
                              bnView2Str(bone->pool, ((bnString*)name)->value));
        }
        bnObject* obj = v;
        if (obj->type != BN_OBJECT_LAMBDA) {
                _throw(bone, frame, "C value is not lambda");
        }
        // check parameters
        bnLambda* lambda = obj;
        bnArray* paraArr = params;
        if (paraArr->arr->len != g_list_length(lambda->parameters)) {
                bnFormatThrow(bone, "illegal parameter length: %d != %d",
                              paraArr->arr->len,
                              g_list_length(lambda->parameters));
        }
        if (!compare_list_array(lambda->parameters, paraArr->arr)) {
                bnFormatThrow(bone, "missing parameter");
        }
        // check returns
        bnArray* retuArr = returns;
        if (retuArr->arr->len != g_list_length(lambda->returns)) {
                bnFormatThrow(bone, "illegal return length: %d != %d",
                              retuArr->arr->len,
                              g_list_length(lambda->returns));
        }
        if (!compare_list_array(lambda->returns, retuArr->arr)) {
                bnFormatThrow(bone, "missing return");
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
                g_string_free(bnPopStack(bone->callStack), TRUE);
        }
}

// Object

void bnStdObjectFuncCall(bnInterpreter* bone, bnFrame* frame) {}

void bnStdObjectPositive(bnInterpreter* bone, bnFrame* frame) {}

void bnStdObjectNegative(bnInterpreter* bone, bnFrame* frame) {}

void bnStdObjectChilda(bnInterpreter* bone, bnFrame* frame) {}

void bnStdObjectNot(bnInterpreter* bone, bnFrame* frame) {}

void bnStdObjectPlus(bnInterpreter* bone, bnFrame* frame) {}

void bnStdObjectMinus(bnInterpreter* bone, bnFrame* frame) {}

void bnStdObjectMultiply(bnInterpreter* bone, bnFrame* frame) {}

void bnStdObjectDivide(bnInterpreter* bone, bnFrame* frame) {}

void bnStdObjectModulo(bnInterpreter* bone, bnFrame* frame) {}

void bnStdObjectBitAnd(bnInterpreter* bone, bnFrame* frame) {}

void bnStdObjectBitOr(bnInterpreter* bone, bnFrame* frame) {}

// void bnStdObjectLogicAnd(bnInterpreter* bone, bnFrame* frame){}

// void bnStdObjectLogicOr(bnInterpreter* bone, bnFrame* frame){}

void bnStdObjectExcOr(bnInterpreter* bone, bnFrame* frame) {}

void bnStdObjectLShift(bnInterpreter* bone, bnFrame* frame) {}

void bnStdObjectRShift(bnInterpreter* bone, bnFrame* frame) {}

void bnStdObjectGT(bnInterpreter* bone, bnFrame* frame) {}

void bnStdObjectGE(bnInterpreter* bone, bnFrame* frame) {}

void bnStdObjectLT(bnInterpreter* bone, bnFrame* frame) {}

void bnStdObjectLE(bnInterpreter* bone, bnFrame* frame) {}

void bnStdObjectEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, a == b));
}

void bnStdObjectNotEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, a != b));
}

void bnStdObjectDefine(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        bnObject* c = bnPopStack(frame->vStack);
        if (b->type != BN_OBJECT_STRING) {
                bnFormatThrow(bone, "shoud be `name` is string");
        }
        bnString* bStr = b;
        g_hash_table_replace(a->table, bStr->value, c);
}

void bnStdObjectUndef(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (b->type != BN_OBJECT_STRING) {
                bnFormatThrow(bone, "shoud be `name` is string");
        }
        bnString* bStr = b;
        gboolean removed = g_hash_table_remove(a->table, bStr->value);
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, removed));
}

void bnStdObjectDefined(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (b->type != BN_OBJECT_STRING) {
                bnFormatThrow(bone, "shoud be `name` is string");
        }
        bnString* bStr = b;
        gboolean contains = g_hash_table_contains(a->table, bStr->value);
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, contains));
}

// Bool

void bnStdBoolFuncCall(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolPositive(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolNegative(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolChilda(bnInterpreter* bone, bnFrame* frame);

void bnStdBoolNot(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, "should be `self` is bool");
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

void bnStdBoolBitAnd(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, "should be `self` is bool");
        }
        if (b->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, "should be argument is bool");
        }
        bnBool* boolA = a;
        bnBool* boolB = b;
        bnObject* c = g_hash_table_lookup(
            frame->variableTable,
            bnIntern(bone->pool,
                     (boolA->value & boolB->value) ? "true" : "false"));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             c);
}

void bnStdBoolBitOr(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, "should be `self` is bool");
        }
        if (b->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, "should be argument is bool");
        }
        bnBool* boolA = a;
        bnBool* boolB = b;
        bnObject* c = g_hash_table_lookup(
            frame->variableTable,
            bnIntern(bone->pool,
                     (boolA->value | boolB->value) ? "true" : "false"));
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             c);
}

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
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "should be `self` is integer");
        }
        int ai = ((bnInteger*)a)->value;
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewInteger(bone, -ai));
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
                _throw(bone, frame,
                       "overload of equality on string is compare for string.");
        }
        bnStringView ai = ((bnString*)a)->value;
        bnStringView bi = ((bnString*)b)->value;
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai == bi));
}

void bnStdStringNotEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_STRING || b->type != BN_OBJECT_STRING) {
                _throw(bone, frame,
                       "overload of equality on string is compare for string.");
        }
        bnStringView ai = ((bnString*)a)->value;
        bnStringView bi = ((bnString*)b)->value;
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnGetBool(bone->pool, frame, ai != bi));
}

void bnStdStringAt(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_STRING) {
                _throw(bone, frame, "should be `self` is string");
        }
        if (b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "should be `index` is string");
        }
        bnStringView ai = ((bnString*)a)->value;
        bnInteger* bi = b;
        const char* astr = bnView2Str(bone->pool, ai);
        int astrlen = strlen(astr);
        if (bi->value < 0 || bi->value >= astrlen) {
                bnFormatThrow(bone, "over index in bounds: %d~%d", 0, astrlen);
        }
        g_hash_table_replace(frame->variableTable, bnIntern(bone->pool, "ret"),
                             bnNewChar(bone, astr[bi->value]));
}

// Char

void bnStdCharFuncCall(bnInterpreter* bone, bnFrame* frame) {}

void bnStdCharPositive(bnInterpreter* bone, bnFrame* frame) {}

void bnStdCharNegative(bnInterpreter* bone, bnFrame* frame) {}

void bnStdCharChilda(bnInterpreter* bone, bnFrame* frame) {}

void bnStdCharNot(bnInterpreter* bone, bnFrame* frame) {}

void bnStdCharPlus(bnInterpreter* bone, bnFrame* frame) {}

void bnStdCharMinus(bnInterpreter* bone, bnFrame* frame) {}

void bnStdCharMultiply(bnInterpreter* bone, bnFrame* frame) {}

void bnStdCharDivide(bnInterpreter* bone, bnFrame* frame) {}

void bnStdCharModulo(bnInterpreter* bone, bnFrame* frame) {}

void bnStdCharBitAnd(bnInterpreter* bone, bnFrame* frame) {}

void bnStdCharBitOr(bnInterpreter* bone, bnFrame* frame) {}

// void bnStdCharLogicAnd(bnInterpreter* bone, bnFrame* frame){}

// void bnStdCharLogicOr(bnInterpreter* bone, bnFrame* frame){}

void bnStdCharExcOr(bnInterpreter* bone, bnFrame* frame) {}

void bnStdCharLShift(bnInterpreter* bone, bnFrame* frame) {}

void bnStdCharRShift(bnInterpreter* bone, bnFrame* frame) {}

void bnStdCharGT(bnInterpreter* bone, bnFrame* frame) {}

void bnStdCharGE(bnInterpreter* bone, bnFrame* frame) {}

void bnStdCharLT(bnInterpreter* bone, bnFrame* frame) {}

void bnStdCharLE(bnInterpreter* bone, bnFrame* frame) {}

void bnStdCharEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_CHAR || b->type != BN_OBJECT_CHAR) {
                _throw(bone, frame,
                       "overload of equality on char is compare for char.");
        }
        bnChar* aChar = a;
        bnChar* bChar = b;
        g_hash_table_replace(
            frame->variableTable, bnIntern(bone->pool, "ret"),
            bnGetBool(bone->pool, frame, aChar->value == bChar->value));
}

void bnStdCharNotEqual(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_CHAR || b->type != BN_OBJECT_CHAR) {
                _throw(bone, frame,
                       "overload of equality on char is compare for char.");
        }
        bnChar* aChar = a;
        bnChar* bChar = b;
        g_hash_table_replace(
            frame->variableTable, bnIntern(bone->pool, "ret"),
            bnGetBool(bone->pool, frame, aChar->value != bChar->value));
}

// Array
void bnStdArrayArraySet(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        bnObject* c = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_ARRAY) {
                _throw(bone, frame, "should be `self` is array");
        }
        if (b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "should be `index` is integer");
        }
        bnArray* arr = a;
        bnInteger* idx = b;
        bnInteger* val = c;
        g_ptr_array_index(arr->arr, idx->value) = c;
}

void bnStdArrayArrayGet(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        bnObject* b = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_ARRAY) {
                _throw(bone, frame, "should be `self` is array");
        }
        if (b->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "should be `index` is integer");
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

static bool file_exists(const char* path) { return bnExists(path); }

static bool compare_list_array(GList* a, GPtrArray* b) {
        if (g_list_length(a) != b->len) {
                return false;
        }
        for (int i = 0; i < b->len; i++) {
                bnString* e = g_ptr_array_index(b, i);
                if (e->value != (bnStringView)a->data) {
                        return false;
                }
                a = a->next;
        }
        return true;
}