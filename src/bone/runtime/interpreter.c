#include "interpreter.h"
#include "../il/il_toplevel.h"
#include "../parse/ast.h"
#include "../parse/ast2il.h"
#include "../parse/parser.h"
#include "bool.h"
#include "enviroment.h"
#include "extern.h"
#include "frame.h"
#include "heap.h"
#include "integer.h"
#include "lambda.h"
#include "object.h"
#include "std.h"
#include "string.h"
#include "vm.h"

bnInterpreter* bnNewInterpreter(const char* filenameRef) {
        bnInterpreter* ret = BN_MALLOC(sizeof(bnInterpreter));
        ret->filenameRef = filenameRef;
        ret->pool = bnNewStringPool();
        ret->heap = bnNewHeap();
        ret->frame = NULL;
        ret->externTable =
            g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, NULL);
        ret->nativeAlloc = NULL;
        ret->__jstack = bnNewJStack();
        g_hash_table_replace(
            ret->externTable, bnIntern(ret->pool, "exit"),
            bnNewLambdaFromCFunc(ret, bnExtExit, ret->pool, BN_C_ADD_PARAM,
                                 "status", BN_C_ADD_EXIT));
        g_hash_table_replace(
            ret->externTable, bnIntern(ret->pool, "abort"),
            bnNewLambdaFromCFunc(ret, bnExtAbort, ret->pool, BN_C_ADD_EXIT));
        return ret;
}

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
        if (iltop == NULL) {
                // fail compile
                return 1;
        }
        // generate instructions
        bnEnviroment* env = bnNewEnviroment();
        self->frame = bnNewFrame();
        bnWriteDefaults(self, self->frame, self->pool);
        bnGenerateILTopLevel(self, iltop, env);
        g_ptr_array_add(env->codeArray, BN_OP_DEFER_NEXT);
        bnDeleteAST(ret);
        bnExecute(self, env, self->frame);
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
        g_hash_table_replace(
            frame->variableTable, bnIntern(pool, "object"),
            bnNewLambdaFromCFunc(self, bnStdSystemObject, pool, BN_C_ADD_RETURN,
                                 "ret", BN_C_ADD_EXIT));
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
        g_hash_table_destroy(self->externTable);
        BN_FREE(self);
}