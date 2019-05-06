#ifndef BONE_RUNTIME_INTERPRETER_H
#define BONE_RUNTIME_INTERPRETER_H
#include <setjmp.h>
#include <stdbool.h>
#include "../util/string_pool.h"
#include "../glib.h"

#define BN_JMP_CODE_EXCEPTION (1)

struct bnObject;
struct bnFrame;
struct bnHeap;

typedef struct bnInterpreter {
        const char* filenameRef;
        GList* nativeAlloc;
        struct bnStringPool* pool;
        struct bnFrame* frame;
        struct bnHeap* heap;
        struct bnObject* __exception;
        jmp_buf __jmp;
} bnInterpreter;

bnInterpreter* bnNewInterpreter(const char* filenameRef);

int bnEval(bnInterpreter* self);

/**
 * @param self
 * @param frame
 * @param pool
 */
void bnWriteDefaults(bnInterpreter* self, struct bnFrame* frame,
                     struct bnStringPool* pool);

/**
 * bnPanic is call longjmp().
 * shoud be invoke only in can panicable function.
 * @param self
 * @param exception
 * @param code
 */
void bnPanic(bnInterpreter* self, struct bnObject* exception, int code);

/**
 * @param pool
 * @param frame
 * @param cond
 * @return
 */
struct bnObject* bnGetBool(struct bnStringPool* pool, struct bnFrame* frame,
                           bool cond);

/**
 * @param pool
 * @param frame
 * @return
 */
struct bnObject* bnGetTrue(struct bnStringPool* pool, struct bnFrame* frame);

/**
 * @param pool
 * @param frame
 * @return
 */
struct bnObject* bnGetFalse(struct bnStringPool* pool, struct bnFrame* frame);

void bnDeleteInterpreter(bnInterpreter* self);
#endif