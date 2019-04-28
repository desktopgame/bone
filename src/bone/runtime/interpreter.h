#ifndef BONE_RUNTIME_INTERPRETER_H
#define BONE_RUNTIME_INTERPRETER_H
#include "../bone.h"

typedef struct bnInterpreter {
        const char* filenameRef;
} bnInterpreter;

bnInterpreter* bnNewInterpreter(const char* filenameRef);

int bnEval(bnInterpreter* self);

void bnDeleteInterpreter(bnInterpreter* self);
#endif