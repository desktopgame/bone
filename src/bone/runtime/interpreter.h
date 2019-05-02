#ifndef BONE_RUNTIME_INTERPRETER_H
#define BONE_RUNTIME_INTERPRETER_H
#include <setjmp.h>
#include "../util/string_pool.h"

#define BN_JMP_CODE_EXCEPTION (1)

struct bnObject;

typedef struct bnInterpreter {
        const char* filenameRef;
        struct bnStringPool* pool;
        struct bnObject* __exception;
        jmp_buf __jmp;
} bnInterpreter;

bnInterpreter* bnNewInterpreter(const char* filenameRef);

int bnEval(bnInterpreter* self);

/**
 * bnPanic is call longjmp().
 * shoud be invoke only in can panicable function.
 * @param self
 * @param exception
 * @param code
 */
void bnPanic(bnInterpreter* self, struct bnObject* exception, int code);

void bnDeleteInterpreter(bnInterpreter* self);
#endif