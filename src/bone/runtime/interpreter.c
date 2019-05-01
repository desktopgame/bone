#include "interpreter.h"
#include "../parse/ast.h"
#include "../parse/parser.h"

bnInterpreter* bnNewInterpreter(const char* filenameRef) {
        bnInterpreter* ret = BN_MALLOC(sizeof(bnInterpreter));
        ret->filenameRef = filenameRef;
        ret->pool = bnNewStringPool();
        return ret;
}

int bnEval(bnInterpreter* self) {
        return -1;
        /*
        bnAST* ret = bnParseFile(self->pool, self->filenameRef);
        int code = ret == NULL ? 1 : 0;
        bnDumpAST(stdout, ret);
        bnDeleteAST(ret);
        return code;
        */
}

void bnDeleteInterpreter(bnInterpreter* self) {
        bnDeleteStringPool(self->pool);
        BN_FREE(self);
}