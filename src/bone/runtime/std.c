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
                // abort();
                _throw(bone, frame, "assertion failed");
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
        printf("> %s", cstr);
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
		BN_CHECK_MEM();
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
		BN_CHECK_MEM();
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
        // assert(frame->panic == NULL);
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

void bnStdSystemEval(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnPopStack(frame->vStack);
        if (a->type != BN_OBJECT_STRING) {
                _throw(bone, frame, "should be path is string");
        }
        bnStringView srcView = ((bnString*)a)->value;
        const char* srcStr = bnView2Str(bone->pool, srcView);
        // parse file
        bnAST* ast = bnParseString(bone->pool, srcStr);
        if (ast == NULL) {
                bnFormatThrow(bone, "syntax error in `%s`", "string");
        }
        bnILToplevel* iltop = bnAST2IL(ast);
        // gen code
        bnFrame* sub = bnSubFrame(frame);
        bnEnviroment* env = bnNewEnviroment(bnIntern(bone->pool, "string"));
        bnGenerateILTopLevel(bone, iltop, env);
        bnInjectFrame(frame->variableTable, sub);
        bnExecute(bone, env, sub);
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
                bnGStringAppendC(gbuf, eChar->value);
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