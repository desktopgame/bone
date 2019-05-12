#include "test.h"
#if !defined(_WIN32)
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#else
#define CU_ASSERT(cond) assert(cond)
#endif
#include <assert.h>
#include <string.h>
#include "bone.h"
#include "il/il_toplevel.h"
#include "parse/ast2il.h"
#include "parse/parser.h"
#include "runtime/enviroment.h"
#include "runtime/frame.h"
#include "runtime/integer.h"
#include "runtime/interpreter.h"
#include "runtime/object.h"
#include "runtime/opcode.h"
#include "runtime/vm.h"
#include "util/string_pool.h"

#if defined(_MSC_VER)
#include <BaseTsd.h>
// see:
// https://stackoverflow.com/questions/22265610/why-ssize-t-in-visual-studio-2010-is-defined-as-unsigned
typedef SSIZE_T ssize_t;
#endif

#define EXPECT_ERR (0)
#define EXPECT_SUC (1)

static void writeEnv(const gchar* out, struct bnStringPool* pool,
                     bnEnviroment* env) {
        if (env == NULL) {
                return;
        }
        FILE* fp = fopen(out, "w");
        if (fp == NULL) {
                perror("writeEnv");
                return;
        }
        int pos = 0;
        int len = env->codeArray->len;
        while (pos < len) {
                pos = bnPrintOpcode(fp, pool, env->codeArray, pos);
                fprintf(fp, "\n");
        }
        fclose(fp);
}

static void writeIL(const gchar* out, struct bnStringPool* pool,
                    bnILToplevel* il) {
        if (il == NULL) {
                return;
        }
        FILE* fp = fopen(out, "w");
        if (fp == NULL) {
                perror("writeIL");
                return;
        }
        bnDumpILTopLevel(fp, pool, il, 0);
        fclose(fp);
}

static void writeAST(const gchar* out, struct bnStringPool* pool, bnAST* a) {
        if (a == NULL) {
                return;
        }
        FILE* fp = fopen(out, "w");
        if (fp == NULL) {
                perror("writeAST");
                return;
        }
        bnDumpAST(fp, pool, a);
        fclose(fp);
}

static void writeFile(const gchar* out) {
        FILE* fp = fopen(out, "r");
        if (fp == NULL) {
                perror("writeFile");
                return;
        }
        char* line = NULL;
        size_t len = 0;
        ssize_t read;
        while ((read = getline(&line, &len, fp)) != -1) {
                printf("    %s", line);
        }
        free(line);
        fclose(fp);
}

static GList* bnGetFiles(const char* dir) {
        GError* err = NULL;
        GDir* dirp = g_dir_open(dir, 0, &err);
        gchar* file = ".";
        gchar* cwd = g_get_current_dir();
        GList* ret = NULL;
        while ((file = g_dir_read_name(dirp)) != NULL) {
                gchar* path = g_build_filename(cwd, dir, file, NULL);
                ret = g_list_append(ret, path);
        }
        ret = g_list_sort(ret, strcmp);
        g_dir_close(dirp);
        g_free(cwd);
        return ret;
}

static int bnParse(const char* dir, int flag) {
        GList* list = bnGetFiles(dir);
        GList* iter = list;
        struct bnStringPool* pool = bnNewStringPool();
        while (iter != NULL) {
                // do process only a suffix .in
                gchar* path = iter->data;
                if (!g_str_has_suffix(path, ".in")) {
                        iter = iter->next;
                        continue;
                }
                // clear parse result file
                gchar* out = g_strconcat(path, ".out", NULL);
                if (g_file_test(out,
                                (G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR))) {
                        g_remove(out);
                }
                // parse and test
                bnAST* a = bnParseFile(pool, path);
                if (flag == EXPECT_SUC) {
                        bnILToplevel* iltop = bnAST2IL(a);
                        writeIL(out, pool, iltop);
                        CU_ASSERT(a != NULL);
                        bnDeleteAST(a);
                        bnDeleteILTopLevel(iltop);
                } else if (flag == EXPECT_ERR) {
                        CU_ASSERT(a == NULL);
                }
                g_free(out);
                iter = iter->next;
        }
        g_list_free_full(list, free);
        bnDeleteStringPool(pool);
        return 0;
}

static int bnVM(const char* dir, int flag) {
        GList* list = bnGetFiles(dir);
        GList* iter = list;
        bnInterpreter* bone = bnNewInterpreter("");
        while (iter != NULL) {
                // do process only a suffix .in
                gchar* path = iter->data;
                if (!g_str_has_suffix(path, ".in")) {
                        iter = iter->next;
                        continue;
                }
                // clear parse result file
                gchar* out = g_strconcat(path, ".out", NULL);
                if (g_file_test(out,
                                (G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR))) {
                        g_remove(out);
                }
                // parse and test
                bnAST* a = bnParseFile(bone->pool, path);
                if (flag == EXPECT_SUC) {
                        bnILToplevel* iltop = bnAST2IL(a);
                        bnEnviroment* env = bnNewEnviroment();
                        bnGenerateILTopLevel(bone, iltop, env);
                        writeEnv(out, bone->pool, env);
                        CU_ASSERT(a != NULL);
                        bnDeleteAST(a);
                        bnDeleteILTopLevel(iltop);
                        bnDeleteEnviroment(env);
                } else if (flag == EXPECT_ERR) {
                        CU_ASSERT(a == NULL);
                }
                g_free(out);
                iter = iter->next;
        }
        g_list_free_full(list, free);
        bnDeleteInterpreter(bone);
        return 0;
}

static int bnRun(const char* dir, int flag) {
        GList* list = bnGetFiles(dir);
        GList* iter = list;
        while (iter != NULL) {
                // do process only a suffix .in
                gchar* path = iter->data;
                if (!g_str_has_suffix(path, ".in") || !strstr(path, "_R")) {
                        iter = iter->next;
                        continue;
                }
                bool panicTest = strstr(path, "_P");
                printf("\n");
                // clear run result file
                gchar* sout = g_strconcat(path, ".std.out", NULL);
                if (g_file_test(sout,
                                (G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR))) {
                        g_remove(sout);
                }
                printf("RUN %s\n", path);
#if !defined(_WIN32)
                FILE* soutfp = fopen(sout, "w");
                FILE* _stdout = stdout;
                stdout = soutfp;
#endif
                // parse and test
                bnInterpreter* bone = bnNewInterpreter(path);
                int ret = bnEval(bone);
#if !defined(_WIN32)
                stdout = _stdout;
                fclose(soutfp);
                writeFile(sout);
#endif
                if (flag == EXPECT_SUC) {
                        CU_ASSERT(ret == 0 || panicTest);
                } else if (flag == EXPECT_ERR) {
                        CU_ASSERT(ret != 0);
                }
                bnDeleteInterpreter(bone);
                iter = iter->next;
        }
        g_list_free_full(list, free);
        return 0;
}

void bnParseTest() {
        bnParse("./testdata/parse/err", EXPECT_ERR);
        bnParse("./testdata/parse/suc", EXPECT_SUC);
}

void bnStringPoolTest() {
        struct bnStringPool* pool = bnNewStringPool();
        bnStringView nullView = bnIntern(pool, NULL);
        bnStringView emptyView = bnIntern(pool, "");
        CU_ASSERT(nullView == BN_NULL_VIEW);
        CU_ASSERT(emptyView == BN_EMPTY_VIEW);
        const char* kwdstr = "keyword";
        bnStringView kwdView = bnIntern(pool, kwdstr);
        CU_ASSERT(!strcmp(kwdstr, bnView2Str(pool, kwdView)));
        bnDeleteStringPool(pool);
}

void bnVMTest() {
        bnVM("./testdata/vm/err", EXPECT_ERR);
        bnVM("./testdata/vm/suc", EXPECT_SUC);
}

void bnRunTest() {
        bnRun("./testdata/vm/err", EXPECT_ERR);
        bnRun("./testdata/vm/suc", EXPECT_SUC);
}