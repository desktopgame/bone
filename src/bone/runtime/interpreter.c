#include "interpreter.h"
#include "../il/il_toplevel.h"
#include "../parse/ast.h"
#include "../parse/ast2il.h"
#include "../parse/parser.h"
#include "enviroment.h"
#include "frame.h"
#include "integer.h"
#include "lambda.h"
#include "object.h"
#include "std.h"
#include "vm.h"

bnInterpreter* bnNewInterpreter(const char* filenameRef) {
        bnInterpreter* ret = BN_MALLOC(sizeof(bnInterpreter));
        ret->filenameRef = filenameRef;
        ret->pool = bnNewStringPool();
        ret->__return = NULL;
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
        bnFrame* frame = bnNewFrame();
        g_hash_table_insert(
            frame->variableTable, bnIntern(self->pool, "print"),
            bnNewLambdaFromCFunc(bnStdSystemPrint, self->pool, NULL));
        g_hash_table_insert(
            frame->variableTable, bnIntern(self->pool, "println"),
            bnNewLambdaFromCFunc(bnStdSystemPrintln, self->pool, NULL));
        g_hash_table_insert(
            frame->variableTable, bnIntern(self->pool, "object"),
            bnNewLambdaFromCFunc(bnStdSystemObject, self->pool, "ret", NULL));
        bnGenerateILTopLevel(self, iltop, env);
        bnDeleteAST(ret);
        bnExecute(self, env, frame);
        bnDeleteILTopLevel(iltop);
        self->__return = bnReturnValue(frame);
        return 0;
}

void bnPanic(bnInterpreter* self, bnObject* exception, int code) {
        self->__exception = exception;
        longjmp(self->__jmp, code);
}

void bnDeleteInterpreter(bnInterpreter* self) {
        bnDeleteStringPool(self->pool);
        BN_FREE(self);
}