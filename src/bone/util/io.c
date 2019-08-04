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

FILE* bnStdout() { return out == NULL ? stdout : out; }

FILE* bnStderr() { return err == NULL ? stderr : err; }

FILE* bnStdin() { return in == NULL ? stdin : in; }

bool bnExists(const char* path) {
        FILE* fp = fopen(path, "r");
        bool ret = fp != NULL;
        if (fp != NULL) {
                fclose(fp);
        }
        return ret;
}