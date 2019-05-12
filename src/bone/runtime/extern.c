#include "extern.h"
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

void bnExtExit(bnInterpreter* bone, bnFrame* frame) {
        bnObject* statusObj = bnPopStack(frame->vStack);
        if (statusObj->type != BN_OBJECT_INTEGER) {
                bnThrow(bone, bnNewString(bone, "status code is not integer"),
                        BN_JMP_CODE_EXCEPTION);
        }
        bnInteger* statusInt = statusObj;
        exit(statusInt->value);
}

void bnExtAbort(bnInterpreter* bone, bnFrame* frame) { abort(); }