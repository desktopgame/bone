#include "interpreter.h"
#include "../parse/ast.h"
#include "../parse/parser.h"

bnInterpreter* bnNewInterpreter(const char* filenameRef) {
        bnInterpreter* ret = BN_MALLOC(sizeof(bnInterpreter));
        ret->filenameRef = filenameRef;
        return ret;
}

int bnEval(bnInterpreter* self) {
        bnAST* ret = bnParseFile(self->filenameRef);
        int code = ret == NULL ? 1 : 0;
        bnDumpAST(stdout, ret);
        bnDeleteAST(ret);
        return code;
}

void bnDeleteInterpreter(bnInterpreter* self) { BN_FREE(self); }