#include "il_lineno.h"
#include <stdbool.h>

static int gLine = -1;

static bool showLine(int line) {
        if (gLine == line) {
                return false;
        }
        gLine = line;
        return true;
}

void bnDumpLineno(FILE* fp, int line) {
        if (showLine(line)) {
                fprintf(fp, "<%d>", line);
        }
}