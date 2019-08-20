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
#include "heap.h"
#include "integer.h"
#include "interpreter.h"
#include "lambda.h"
#include "object.h"
#include "string.h"
#include "vm.h"

static void _throw(bnInterpreter* bone, bnFrame* frame, const char* str);
static bool file_exists(const char* path);
static bool compare_list_array(bnInterpreter* bone, GList* a, bnObject* b);

// デバッグビルドでのみ有効な関数
#if DEBUG
void bnStdDebugAssert(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (a->type != BN_OBJECT_BOOL) {
                _throw(bone, frame, "internal error");
        }
        if (!bnGetBoolValue(a)) {
                _throw(bone, frame, "assertion failed");
        }
}

void bnStdDebugDie(bnInterpreter* bone, bnFrame* frame) { abort(); }

void bnStdDebugPrint(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (a->type != BN_OBJECT_STRING) {
                _throw(bone, frame, "internal error");
        }
        const char* cstr = bnView2Str(bone->pool, bnGetStringValue(a));
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
        //インタプリタを任意の地点でブレークするための関数です。
        //実際はここにブレークポイントを設定してください。
        abort();
#endif
}

void bnStdDebugDumpTable(bnInterpreter* bone, bnFrame* frame) {
        GHashTableIter hashIter;
        g_hash_table_iter_init(&hashIter, frame->variableTable);
        gpointer k, v;
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                const char* kStr = bnView2Str(bone->pool, (bnStringView)k);
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
                const char* strk = bnView2Str(bone->pool, (bnStringView)k);
                for (int i = 0; i < depth; i++) fprintf(BN_STDOUT, "    ");
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

// 組み込み関数

void bnStdSystemInclude(bnInterpreter* bone, bnFrame* frame) {
        BN_CHECK_MEM();
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (a->type != BN_OBJECT_STRING) {
                _throw(bone, frame, "should be path is string");
        }
        bnStringView pathView = bnGetStringValue(a);
        const char* pathStr = bnView2Str(bone->pool, pathView);
        if (!file_exists(pathStr)) {
                bnFormatThrow(bone, "`%s` is not found", pathStr);
        }
        // 指定のファイルを解析する
        GString* resolved = bnResolveLoadPath(pathStr);
        bnAST* ast = bnParseFile(bone->pool, resolved->str);
        if (ast == NULL) {
                g_string_free(resolved, TRUE);
                bnFormatThrow(bone, "syntax error in `%s`", pathStr);
        }
        g_string_free(resolved, TRUE);
        //中間表現へ変換
        bnILToplevel* iltop = bnAST2IL(ast);
        //コード生成して実行
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
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (a->type != BN_OBJECT_STRING) {
                _throw(bone, frame, "should be path is string");
        }
        bnStringView pathView = bnGetStringValue(a);
        const char* pathStr = bnView2Str(bone->pool, pathView);
        if (!file_exists(pathStr)) {
                bnFormatThrow(bone, "`%s` is not found", pathStr);
        }
        //指定のファイルを解析
        GString* resolved = bnResolveLoadPath(pathStr);
        bnAST* ast = bnParseFile(bone->pool, resolved->str);
        if (ast == NULL) {
                g_string_free(resolved, TRUE);
                bnFormatThrow(bone, "syntax error in `%s`", pathStr);
        }
        g_string_free(resolved, TRUE);
        //中間表現へ変換
        bnILToplevel* iltop = bnAST2IL(ast);
        //コード生成して実行
        bnFrame* sub = bnSubFrame(frame);
        bnEnviroment* env = bnNewEnviroment(bnIntern(bone->pool, pathStr));
        bnGenerateILTopLevel(bone, iltop, env);
        bnWriteBuiltin(bone, sub, bone->pool);
        // get default hash
        GList* keys = NULL;
        GHashTableIter hashIter;
        gpointer k, v;
        g_hash_table_iter_init(&hashIter, sub->variableTable);
        while (g_hash_table_iter_next(&hashIter, &k, &v)) {
                keys = g_list_append(keys, v);
        }
        bnExecute(bone, env, sub);
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
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (a->type != BN_OBJECT_STRING) {
                _throw(bone, frame, "should be path is string");
        }
        bnStringView srcView = bnGetStringValue(a);
        const char* srcStr = bnView2Str(bone->pool, srcView);
        // 指定の文字列を解析する
        bnAST* ast = bnParseString(bone->pool, srcStr);
        if (ast == NULL) {
                bnFormatThrow(bone, "syntax error in `%s`", "string");
        }
        //中間表現へ変換する
        bnILToplevel* iltop = bnAST2IL(ast);
        //コード生成して実行
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
        bnWriteVariable2(frame, bone->pool, "ret", bnNewObject(bone));
}

void bnStdSystemString(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (a->type != BN_OBJECT_ARRAY) {
                bnFormatThrow(bone, "should be `array` is array");
        }
        bnObject* aArr = a;
        GString* gbuf = g_string_new("");
        for (int i = 0; i < bnGetArrayLength(aArr); i++) {
                bnReference eRef = bnGetArrayElementAt(aArr, i);
                bnObject* e = bnGetObject(bone->heap, eRef);
                if (e->type != BN_OBJECT_CHAR) {
                        bnFormatThrow(bone, "should be `array[%d]` is char", i);
                }
                bnGStringAppendC(gbuf, bnGetCharValue(e));
        }
        bnStringView gview = bnIntern(bone->pool, gbuf->str);
        g_string_free(gbuf, TRUE);
        bnWriteVariable2(frame, bone->pool, "ret", bnNewString(bone, gview));
}

void bnStdSystemArray(bnInterpreter* bone, bnFrame* frame) {
        bnObject* a = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (a->type != BN_OBJECT_INTEGER) {
                _throw(bone, frame, "should be `length` is integer");
        }
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnNewArray(bone, bnGetIntegerValue(a)));
}

void bnStdSystemExternVar(bnInterpreter* bone, bnFrame* frame) {
        bnObject* name = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (name->type != BN_OBJECT_STRING) {
                _throw(bone, frame, "should be `name` is string");
        }
        bnReference objRef = bnReadExtern(bone, bnGetStringValue(name));
        if (objRef == NULL) {
                bnFormatThrow(bone, "not bound variable: `%s`",
                              bnView2Str(bone->pool, bnGetStringValue(name)));
        }
        bnWriteVariable2(frame, bone->pool, "ret", objRef);
}

void bnStdSystemExternDef(bnInterpreter* bone, bnFrame* frame) {
        bnObject* name = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnObject* params = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        bnObject* returns = bnGetObject(bone->heap, bnPopStack(frame->vStack));
        if (name->type != BN_OBJECT_STRING) {
                _throw(bone, frame, "should be `name` is string");
        }
        if (params->type != BN_OBJECT_ARRAY) {
                _throw(bone, frame, "should be `params` is array");
        }
        if (returns->type != BN_OBJECT_ARRAY) {
                _throw(bone, frame, "should be `returns` is array");
        }
        //エクスポートテーブルから検索する
        bnReference objRef = bnReadExtern(bone, bnGetStringValue(name));
        if (objRef == NULL) {
                bnFormatThrow(bone, "not bound variable: `%s`",
                              bnView2Str(bone->pool, bnGetStringValue(name)));
        }
        bnObject* obj = bnGetObject(bone->heap, objRef);
        if (obj->type != BN_OBJECT_LAMBDA) {
                _throw(bone, frame, "C value is not lambda");
        }
        //実際の関数テーブルとマッチするか引数をチェックする
        bnObject* lambda = obj;
        bnObject* paraArr = params;
        if (bnGetArrayLength(paraArr) !=
            g_list_length(bnGetParameterList(lambda))) {
                bnFormatThrow(bone, "illegal parameter length: %d != %d",
                              bnGetArrayLength(paraArr),
                              g_list_length(bnGetParameterList(lambda)));
        }
        if (!compare_list_array(bone, bnGetParameterList(lambda), paraArr)) {
                bnFormatThrow(bone, "missing parameter");
        }
        //実際の関数テーブルとマッチするか戻り値をチェックする
        bnObject* retuArr = returns;
        if (bnGetArrayLength(retuArr) !=
            g_list_length(bnGetReturnValueList(lambda))) {
                bnFormatThrow(bone, "illegal return length: %d != %d",
                              bnGetArrayLength(retuArr),
                              g_list_length(bnGetReturnValueList(lambda)));
        }
        if (!compare_list_array(bone, bnGetReturnValueList(lambda), retuArr)) {
                bnFormatThrow(bone, "missing return");
        }
        bnWriteVariable2(frame, bone->pool, "ret", objRef);
}

void bnStdSystemPanic(bnInterpreter* bone, bnFrame* frame) {
        bnReference a = bnPopStack(frame->vStack);
        bnThrow(bone, a, BN_JMP_CODE_EXCEPTION);
}

void bnStdSystemRecover(bnInterpreter* bone, bnFrame* frame) {
        bnWriteVariable2(frame, bone->pool, "ret",
                         bnReadVariable2(frame, bone->pool, "false"));

        if (frame->prev->panic != NULL) {
                bnWriteVariable2(frame, bone->pool, "ret", frame->prev->panic);
                frame->prev->panic = NULL;
                g_string_free(bnPopStack(bone->callStack), TRUE);
        }
}

static void _throw(bnInterpreter* bone, bnFrame* frame, const char* str) {
        bnThrow(bone, bnNewString(bone, bnIntern(bone->pool, str)),
                BN_JMP_CODE_EXCEPTION);
}

static bool file_exists(const char* path) { return bnExists(path); }

static bool compare_list_array(bnInterpreter* bone, GList* a, bnObject* b) {
        if (g_list_length(a) != bnGetArrayLength(b)) {
                return false;
        }
        for (int i = 0; i < bnGetArrayLength(b); i++) {
                bnReference eRef = bnGetArrayElementAt(b, i);
                bnObject* e = bnGetObject(bone->heap, eRef);
                if (bnGetStringValue(e) != (bnStringView)a->data) {
                        return false;
                }
                a = a->next;
        }
        return true;
}