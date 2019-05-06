#include "io.h"
#include <stdlib.h>

static FILE* out = NULL;
static FILE* err = NULL;
static FILE* in = NULL;

void bnInitIO() {
        out = stdout;
        err = stderr;
        in = stdin;
}

FILE* bnStdout() { return out; }

FILE* bnStderr() { return err; }

FILE* bnStdin() { return in; }