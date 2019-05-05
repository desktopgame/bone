#include "interpreter.h"
#include "../il/il_toplevel.h"
#include "../parse/ast.h"
#include "../parse/ast2il.h"
#include "../parse/parser.h"
#include "bool.h"
#include "enviroment.h"
#include "frame.h"
#include "heap.h"
#include "integer.h"
#include "lambda.h"
#include "object.h"
#include "std.h"
#include "vm.h"

bnInterpreter* bnNewInterpreter(const char* filenameRef) {
        bnInterpreter* ret = BN_MALLOC(sizeof(bnInterpreter));
        ret->filenameRef = filenameRef;
        ret->pool = bnNewStringPool();
        ret->heap = bnNewHeap();
        ret->frame = NULL;
        ret->__exception = NULL;
        return ret;
}

int bnEval(bnInterpreter* self) {
        bnAST* ret = bnParseFile(self->pool, self->filenameRef);
        if (ret == NULL) {
                // fail parse
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
        bnDeleteAST(ret);
        bnExecute(self, env, self->frame);
        bnDeleteILTopLevel(iltop);
        bnDeleteEnviroment(env);
        bnGC(self->heap, self->frame);
        bnDeleteFrame(self->frame);
        bnGC(self->heap, NULL);
        self->frame = NULL;
        return 0;
}

void bnWriteDefaults(bnInterpreter* self, bnFrame* frame,
                     struct bnStringPool* pool) {
        // declare true, false
        g_hash_table_replace(frame->variableTable, bnIntern(pool, "true"),
                             bnNewBool(self, true));
        g_hash_table_replace(frame->variableTable, bnIntern(pool, "false"),
                             bnNewBool(self, false));
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
#endif
        g_hash_table_replace(
            frame->variableTable, bnIntern(pool, "object"),
            bnNewLambdaFromCFunc(self, bnStdSystemObject, pool, BN_C_ADD_RETURN,
                                 "ret", BN_C_ADD_EXIT));
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
}

void bnPanic(bnInterpreter* self, bnObject* exception, int code) {
        self->__exception = exception;
        longjmp(self->__jmp, code);
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
        bnDeleteStringPool(self->pool);
        bnDeleteHeap(self->heap);
        BN_FREE(self);
}