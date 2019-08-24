#include "string.h"
#include <string.h>

#if _MSC_VER
#define SLASH ('\\')
#else
#define SLASH ('/')
#endif

int bnLastPathComponent(const char* path) {
        int len = strlen(path);
        int pos = 0;
        const char* sp = path + (len - 1);
        while (pos < len && *sp != SLASH) {
                sp--;
                pos++;
        }
        return (len - pos);
}