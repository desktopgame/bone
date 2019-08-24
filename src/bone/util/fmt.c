#include "fmt.h"

void bnIndent(int depth) { bnFindent(stdout, depth); }

void bnFindent(FILE* fp, int depth) {
        for (int i = 0; i < depth; i++) {
                fprintf(fp, "\t");
        }
}