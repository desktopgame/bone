#include "io.h"
#include <stdlib.h>
#include "../config.inc"
#if __APPLE__
#include <limits.h>
#include <mach-o/dyld.h>
#endif
#if HAVE_WINDOWS
#include <Windows.h>
#include <tchar.h>
#endif
#include "string_util.h"

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

GString* bnResolveLoadPath(const char* relative) {
        //実行ファイルから解決
        GString* exeDir = bnGetExecutableFileDir();
        const char* fullpath = g_build_filename(exeDir->str, relative, NULL);
        if (bnExists(fullpath)) {
                GString* ret = g_string_new(fullpath);
                g_free((gpointer)fullpath);
                g_string_free(exeDir, TRUE);
                return ret;
        }
        g_free((gpointer)fullpath);
        g_string_free(exeDir, TRUE);
        //カレントワーキングディレクトリから解決
        gchar* cwd = g_get_current_dir();
        fullpath = g_build_filename(cwd, relative, NULL);
        if (bnExists(fullpath)) {
                GString* ret = g_string_new(fullpath);
                g_free((gpointer)fullpath);
                g_free((gpointer)cwd);
                return ret;
        }
        g_free((gpointer)fullpath);
        g_free(cwd);
        return g_string_new(relative);
}

GString* bnGetExecutableFileDir() {
        GString* str = g_string_new(NULL);
#define SET_CWD(buf)                             \
        do {                                     \
                gchar* cd = g_get_current_dir(); \
                g_string_append(buf, cd);        \
                g_free(cd);                      \
        } while (0)
#if __APPLE__
        char buf[512];
        uint32_t bufsize = 512;
        if (!_NSGetExecutablePath(buf, &bufsize)) {
                g_string_append(str, buf);
                int lc = bnLastPathComponent(str->str);
                g_string_erase(str, lc, str->len - lc);
        } else {
                SET_CWD(str);
        }
        return str;
#elif __linux__
        char buf[512];
        int ret = readlink("/proc/self/exe", buf, 512);
        if (ret != -1) {
                g_string_append(str, buf);
                int lc = bnLastPathComponent(str->str);
                g_string_erase(str, lc, str->len - lc);
        } else {
                perror("bnGetExecutableFileDir");
                SET_CWD(str);
        }
        return str;
#elif HAVE_WINDOWS
        TCHAR path[MAX_PATH];
        //参考URL:http://www7.plala.or.jp/kfb/program/exedir.html
        if (GetModuleFileName(NULL, path, MAX_PATH)) {
                TCHAR* ptmp = _tcsrchr(path, _T('\\'));
                if (ptmp != NULL) {
                        ptmp = _tcsinc(ptmp);
                        *ptmp = _T('\0');
                        g_string_append(str, (char*)path);
                } else {
                        SET_CWD(str);
                }
        } else {
                SET_CWD(str);
        }
        return str;
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