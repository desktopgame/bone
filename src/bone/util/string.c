#include "string.h"
#include <string.h>

int bnLastPathComponent(const char* path) {
        int len = strlen(path);
        int pos = 0;
        const char* sp = path + (len - 1);
        while (pos < len && *sp != '/') {
                sp--;
                pos++;
        }
        return (len - pos);
}