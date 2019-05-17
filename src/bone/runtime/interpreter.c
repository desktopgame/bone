#include "interpreter.h"
#include "../il/il_toplevel.h"
#include "../parse/ast.h"
#include "../parse/ast2il.h"
#include "../parse/parser.h"
#include "array.h"
#include "bool.h"
#include "enviroment.h"
#include "extern/file.h"
#include "extern/reflection.h"
#include "extern/system.h"
#include "frame.h"
#include "heap.h"
#include "integer.h"
#include "lambda.h"
#include "module.h"
#include "object.h"
#include "std.h"
#include "string.h"
#include "vm.h"

static void free_gstr(void* v);
static void load_plugins(bnInterpreter* self, const char* currentdir);
static void load_plugin(bnInterpreter* self, gchar* path);
static void unload_plugins(bnInterpreter* self);
static void unload_plugin(void* handle);
static bool is_dll(gchar* path);

bnInterpreter* bnNewInterpreter(const char* filenameRef, int argc,
                                char* argv[]) {
        bnInterpreter* ret = BN_MALLOC(sizeof(bnInterpreter));
        ret->filenameRef = filenameRef;
        ret->plugins = NULL;
        ret->pool = bnNewStringPool();
        ret->heap = bnNewHeap();
        ret->argc = argc;
        ret->argv = g_ptr_array_new();
        ret->frame = NULL;
        ret->externTable =
            g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, NULL);
        ret->nativeAlloc = NULL;
        ret->__jstack = bnNewJStack();
        ret->callStack = bnNewStack();
        if (ret->argc > 1) {
                for (int i = 0; i < ret->argc; i++) {
                        g_ptr_array_add(ret->argv,
                                        bnIntern(ret->pool, argv[i]));
                }
        }
        bnExternSystem(ret);
        bnExternFile(ret);
        bnExternReflection(ret);
        return ret;
}

void bnLink(bnInterpreter* bone, const char* path) { load_plugins(bone, path); }

int bnEval(bnInterpreter* self) {
        if (!bnExists(self->filenameRef)) {
                printf("abort:`%s` is not found", self->filenameRef);
                return 1;
        }
        bnAST* ret = bnParseFile(self->pool, self->filenameRef);
        if (ret == NULL) {
                // fail parse
                printf("abort:syntax error in `%s`", self->filenameRef);
                return 1;
        }
        bnILToplevel* iltop = bnAST2IL(ret);
        // generate instructions
        bnEnviroment* env =
            bnNewEnviroment(bnIntern(self->pool, self->filenameRef));
        self->frame = bnNewFrame();
        bnWriteDefaults(self, self->frame, self->pool);
        bnGenerateILTopLevel(self, iltop, env);
        g_ptr_array_add(env->codeArray, BN_OP_DEFER_NEXT);
        bnDeleteAST(ret);
        bnExecute(self, env, self->frame);
        unload_plugins(self);
        while (bnGetStackSize(self->callStack) > 0) {
                GString* gbuf = bnPopStack(self->callStack);
                printf("TRACE: %s\n", gbuf->str);
                g_string_free(gbuf, TRUE);
        }
        if (self->frame->panic != NULL) {
                printf("panic:");
                bnPrintObject(stdout, self, self->frame->panic);
                printf("\n");
        }
        int status = self->frame->panic ? 1 : 0;
        self->frame->panic = NULL;
        bnDeleteILTopLevel(iltop);
        bnDeleteEnviroment(env);
        // g_hash_table_remove_all(self->externTable);
        bnGC(self);
        bnDeleteFrame(self->frame);
        self->frame = NULL;
        g_hash_table_remove_all(self->externTable);
        bnGC(self);
        return status;
}

void bnWriteDefaults(bnInterpreter* self, bnFrame* frame,
                     struct bnStringPool* pool) {
        // declare true, false
        bnBool* t = bnNewBool(self, true);
        bnBool* f = bnNewBool(self, false);
        t->r = f;
        f->r = t;
        g_hash_table_replace(frame->variableTable, bnIntern(pool, "true"), t);
        g_hash_table_replace(frame->variableTable, bnIntern(pool, "false"), f);
#if DEBUG
        g_hash_table_replace(
            frame->variableTable, bnIntern(pool, "assert"),
            bnNewLambdaFromCFunc(self, bnStdDebugAssert, pool, BN_C_ADD_PARAM,
                                 "cond", BN_C_ADD_EXIT));
        g_hash_table_replace(
            frame->variableTable, bnIntern(pool, "die"),
            bnNewLambdaFromCFunc(self, bnStdDebugDie, pool, BN_C_ADD_EXIT));

        g_hash_table_replace(
            frame->variableTable, bnIntern(pool, "print"),
            bnNewLambdaFromCFunc(self, bnStdDebugPrint, pool, BN_C_ADD_PARAM,
                                 "str", BN_C_ADD_EXIT));
        g_hash_table_replace(
            frame->variableTable, bnIntern(pool, "println"),
            bnNewLambdaFromCFunc(self, bnStdDebugPrintln, pool, BN_C_ADD_PARAM,
                                 "str", BN_C_ADD_EXIT));
        g_hash_table_replace(
            frame->variableTable, bnIntern(pool, "debugBreak"),
            bnNewLambdaFromCFunc(self, bnStdDebugBreak, pool, BN_C_ADD_EXIT));
        g_hash_table_replace(frame->variableTable, bnIntern(pool, "dumpTable"),
                             bnNewLambdaFromCFunc(self, bnStdDebugDumpTable,
                                                  pool, BN_C_ADD_EXIT));
        g_hash_table_replace(
            frame->variableTable, bnIntern(pool, "showInfo"),
            bnNewLambdaFromCFunc(self, bnStdDebugShowInfo, pool, BN_C_ADD_PARAM,
                                 "obj", BN_C_ADD_EXIT));
#endif
        g_hash_table_replace(frame->variableTable, bnIntern(pool, "argc"),
                             bnNewInteger(self, BN_MAX(self->argc - 2, 0)));
        bnArray* argv = bnNewArray(self, BN_MAX(self->argc - 2, 0));
        for (int i = 2; i < self->argc; i++) {
                g_ptr_array_index(argv->arr, i - 2) =
                    bnNewString(self, g_ptr_array_index(self->argv, i));
        }
        g_hash_table_replace(frame->variableTable, bnIntern(pool, "argv"),
                             argv);
        g_hash_table_replace(
            frame->variableTable, bnIntern(pool, "object"),
            bnNewLambdaFromCFunc(self, bnStdSystemObject, pool, BN_C_ADD_RETURN,
                                 "ret", BN_C_ADD_EXIT));
        g_hash_table_replace(
            frame->variableTable, bnIntern(pool, "string"),
            bnNewLambdaFromCFunc(self, bnStdSystemString, pool, BN_C_ADD_PARAM,
                                 "array", BN_C_ADD_RETURN, "ret",
                                 BN_C_ADD_EXIT));
        g_hash_table_replace(
            frame->variableTable, bnIntern(pool, "array"),
            bnNewLambdaFromCFunc(self, bnStdSystemArray, pool, BN_C_ADD_PARAM,
                                 "length", BN_C_ADD_RETURN, "ret",
                                 BN_C_ADD_EXIT));
        g_hash_table_replace(
            frame->variableTable, bnIntern(pool, "include"),
            bnNewLambdaFromCFunc(self, bnStdSystemInclude, pool, BN_C_ADD_PARAM,
                                 "path", BN_C_ADD_RETURN, "...",
                                 BN_C_ADD_EXIT));
        g_hash_table_replace(
            frame->variableTable, bnIntern(pool, "load"),
            bnNewLambdaFromCFunc(self, bnStdSystemLoad, pool, BN_C_ADD_PARAM,
                                 "path", BN_C_ADD_RETURN, "...",
                                 BN_C_ADD_EXIT));

        g_hash_table_replace(
            frame->variableTable, bnIntern(pool, "extern_var"),
            bnNewLambdaFromCFunc(self, bnStdSystemExternVar, pool,
                                 BN_C_ADD_PARAM, "name", BN_C_ADD_RETURN, "ret",
                                 BN_C_ADD_EXIT));
        g_hash_table_replace(
            frame->variableTable, bnIntern(pool, "extern_def"),
            bnNewLambdaFromCFunc(self, bnStdSystemExternDef, pool,
                                 BN_C_ADD_PARAM, "name", BN_C_ADD_PARAM,
                                 "params", BN_C_ADD_PARAM, "returns",
                                 BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
        g_hash_table_replace(
            frame->variableTable, bnIntern(pool, "panic"),
            bnNewLambdaFromCFunc(self, bnStdSystemPanic, pool, BN_C_ADD_PARAM,
                                 "error", BN_C_ADD_EXIT));
        g_hash_table_replace(
            frame->variableTable, bnIntern(pool, "recover"),
            bnNewLambdaFromCFunc(self, bnStdSystemRecover, pool,
                                 BN_C_ADD_RETURN, "ret", BN_C_ADD_EXIT));
}

void bnFormatThrow(bnInterpreter* self, const char* fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        bnVFormatThrow(self, fmt, ap);
        va_end(ap);
}

void bnVFormatThrow(bnInterpreter* self, const char* fmt, va_list ap) {
        char buf[100];
        vsprintf(buf, fmt, ap);
        bnObject* obj = bnNewString(self, bnIntern(self->pool, buf));
        bnThrow(self, obj, BN_JMP_CODE_EXCEPTION);
}

void bnThrow(bnInterpreter* self, bnObject* exception, int code) {
        bnPanic(self, exception);
        BN_JMP_DO(self->__jstack, code);
}

void bnPanic(bnInterpreter* self, bnObject* exception) {
        bnFrame* iter = self->frame;
        while (1) {
                if (iter->next == NULL) {
                        break;
                }
                iter = iter->next;
        }
        iter->panic = exception;
}

bnObject* bnGetBool(struct bnStringPool* pool, bnFrame* frame, bool cond) {
        bnObject* ret = cond ? bnGetTrue(pool, frame) : bnGetFalse(pool, frame);
        assert(ret != NULL);
        return ret;
}

bnObject* bnGetTrue(struct bnStringPool* pool, bnFrame* frame) {
        return g_hash_table_lookup(frame->variableTable,
                                   bnIntern(pool, "true"));
}

bnObject* bnGetFalse(struct bnStringPool* pool, bnFrame* frame) {
        return g_hash_table_lookup(frame->variableTable,
                                   bnIntern(pool, "false"));
}

void bnDeleteInterpreter(bnInterpreter* self) {
        // free a allocated by bone program
        self->frame = NULL;
        g_hash_table_remove_all(self->externTable);
        bnGC(self);
        // free a other
        bnDeleteStringPool(self->pool);
        bnDeleteHeap(self->heap);
        bnDeleteJStack(self->__jstack);
        bnDeleteStack(self->callStack, free_gstr);
        g_hash_table_destroy(self->externTable);
        g_ptr_array_unref(self->argv);
        BN_FREE(self);
}

static void free_gstr(void* v) { g_string_free(v, TRUE); }

static void load_plugins(bnInterpreter* self, const char* currentdir) {
        GDir* dir = g_dir_open(currentdir, 0, NULL);
        //ディレクトリを開けた場合
        if (!dir) {
                return;
        }
        const gchar* name;
        while (name = g_dir_read_name(dir)) {
                gchar* path;
                gboolean is_dir;
                path = g_build_filename(currentdir, name, NULL);
                if (g_file_test(path, G_FILE_TEST_IS_DIR) || !is_dll(path)) {
                        g_free(path);
                        continue;
                }
                load_plugin(self, path);
                g_free(path);
        }
        g_dir_close(dir);
}

static void load_plugin(bnInterpreter* self, gchar* path) {
        bnModule* mod = bnNewModule(path);
        bnPluginInit init = (bnPluginInit)bnGetSymbol(mod, "Init");
        if (init != NULL) {
                init(self);
        }
        self->plugins = g_list_append(self->plugins, mod);
}

static void unload_plugins(bnInterpreter* self) {
        GList* iter = self->plugins;
        while (iter != NULL) {
                bnModule* mod = iter->data;
                bnPluginDestroy destroy =
                    (bnPluginDestroy)bnGetSymbol(mod, "Destroy");
                if (destroy != NULL) {
                        destroy(self);
                }
                bnDeleteModule(mod);
                iter->data = NULL;
                iter = iter->next;
        }
        g_list_free(self->plugins);
        self->plugins = NULL;
}

static bool is_dll(gchar* path) {
#if __APPLE__
        return g_str_has_suffix(path, ".dylib");
#elif __WIN32
        return g_str_has_suffix(path, ".dll");
#elif __linux__
        return g_str_has_suffix(path, ".so");
#else
        return false;
#endif
}
