#include "il_lineno.h"
#include <stdbool.h>

#if !defined(NGLOBAL)
static int gLine = -1;
#endif

static bool showLine(int line) {
#if defined(NGLOBAL)
        return false;
#else
        if (gLine == line) {
                return false;
        }
        gLine = line;
        return true;
#endif
}

void bnDumpLineno(FILE* fp, int line) {
        if (showLine(line)) {
                fprintf(fp, "<%d>", line);
        }
}