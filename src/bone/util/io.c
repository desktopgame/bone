#include "io.h"
#include <stdlib.h>
#if __APPLE__
#include <mach-o/dyld.h>
#endif
#if HAVE_WINDOWS
#include <Windows.h>
#endif
#include "../config.h"

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

GString* bnGetExecutableFileDir() {
        GString* str = g_string_new(NULL);
#define SET_CWD(buf)                     \
        gchar* cd = g_get_current_dir(); \
        g_string_append(buf, cd);        \
        g_free(cd);
#if __APPLE__
        char buf[512];
        uint32_t bufsize = 512;
        if (!_NSGetExecutablePath(buf, &bufsize)) {
                g_string_append(str, buf);
        } else {
                SET_CWD(str);
        }
        return str;
#elif __linux__
        char buf[512];
        int ret = readlink("/proc/self/exe", buf, 512);
        if (ret != -1) {
                g_string_append(str, buf);
                perror("bnGetExecutableFileDir");
        } else {
                SET_CWD(str);
        }
        return str;
#elif HAVE_WINDOWS
        SET_CWD(str);
#else
        SET_CWD(str);
        return str;
#endif
}

bool bnExists(const char* path) {
        FILE* fp = fopen(path, "r");
        bool ret = fp != NULL;
        if (fp != NULL) {
                fclose(fp);
        }
        return ret;
}