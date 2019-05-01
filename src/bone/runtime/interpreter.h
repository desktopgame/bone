#ifndef BONE_RUNTIME_INTERPRETER_H
#define BONE_RUNTIME_INTERPRETER_H
#include "../util/string_pool.h"

typedef struct bnInterpreter {
        const char* filenameRef;
        struct bnStringPool* pool;
} bnInterpreter;

bnInterpreter* bnNewInterpreter(const char* filenameRef);

int bnEval(bnInterpreter* self);

void bnDeleteInterpreter(bnInterpreter* self);
#endif