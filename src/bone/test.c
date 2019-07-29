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
#include "util/args.h"
#include "util/string_pool.h"

#if defined(_MSC_VER)
#include <BaseTsd.h>
// see:
// https://stackoverflow.com/questions/22265610/why-ssize-t-in-visual-studio-2010-is-defined-as-unsigned
typedef SSIZE_T ssize_t;
#endif

#define EXPECT_ERR (0)
#define EXPECT_SUC (1)

static void free_gstring_with_segment(gpointer pdata) {
	GString* str = pdata;
	g_string_free(str, TRUE);
}


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
                pos = bnPrintOpcode(fp, pool, env, pos);
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

static int bnParse(GPtrArray* dest, const char* dir, int flag) {
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
                        if (a == NULL) {
                                g_ptr_array_add(dest, g_string_new(path));
                        }
                        bnDeleteAST(a);
                        bnDeleteILTopLevel(iltop);
                } else if (flag == EXPECT_ERR) {
                        CU_ASSERT(a == NULL);
                        if (a != NULL) {
                                g_ptr_array_add(dest, g_string_new(path));
                        }
                }
                g_free(out);
                iter = iter->next;
				BN_CHECK_MEM();
        }
        g_list_free_full(list, g_free);
        bnDeleteStringPool(pool);
        return 0;
}

static int bnVM(GPtrArray* dest, const char* dir, int flag) {
        GList* list = bnGetFiles(dir);
        GList* iter = list;
        bnInterpreter* bone = bnNewInterpreter("", bnArgc(), bnArgv());
        while (iter != NULL) {
                // do process only a suffix .in
                gchar* path = iter->data;
                if (!g_str_has_suffix(path, ".in") || strstr(path, "_R")) {
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
                        bnEnviroment* env =
                            bnNewEnviroment(bnIntern(bone->pool, path));
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
				BN_CHECK_MEM();
        }
        g_list_free_full(list, g_free);
        bnDeleteInterpreter(bone);
        return 0;
}

static int bnRun(GPtrArray* dest, const char* dir, int flag) {
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
                bnInterpreter* bone =
                    bnNewInterpreter(path, bnArgc(), bnArgv());
#if !_WIN32
                bnLink(bone, "./testdata/plugins");
#endif
                int ret = bnEval(bone);
#if !defined(_WIN32)
                stdout = _stdout;
                fclose(soutfp);
                writeFile(sout);
#endif
                if (flag == EXPECT_SUC) {
                        CU_ASSERT(ret == 0 || panicTest);
                        if (ret != 0 && !panicTest) {
                                g_ptr_array_add(dest, g_string_new(path));
                        }
                } else if (flag == EXPECT_ERR) {
                        CU_ASSERT(ret != 0);
                        if (ret == 0) {
                                g_ptr_array_add(dest, g_string_new(path));
                        }
                }
                bnDeleteInterpreter(bone);
                iter = iter->next;
				BN_CHECK_MEM();
        }
        g_list_free_full(list, g_free);
		BN_CHECK_MEM();
        return 0;
}

static void dump_result(const char* header, GPtrArray* src) {
        printf("FAIL [%s/%d]\n", header, src->len);
        for (int i = 0; i < src->len; i++) {
                GString* str = g_ptr_array_index(src, i);
                printf("    %s\n", str->str);
        }
        g_ptr_array_set_free_func(src, free_gstring_with_segment);
        g_ptr_array_free(src, TRUE);
}

void bnParseTest() {
        GPtrArray* dest = g_ptr_array_new();
#if _WIN32
        bnParse(dest, "testdata\\parse\\err", EXPECT_ERR);
        bnParse(dest, "testdata\\parse\\suc", EXPECT_SUC);
#else
		bnParse(dest, "./testdata/parse/err", EXPECT_ERR);
		bnParse(dest, "./testdata/parse/suc", EXPECT_SUC);
#endif
		BN_CHECK_MEM();
        dump_result("PARSE", dest);
}

void bnVMTest() {
        GPtrArray* dest = g_ptr_array_new();
#if _WIN32
        bnVM(dest, "testdata\\vm\\err", EXPECT_ERR);
        bnVM(dest, "testdata\\vm\\suc", EXPECT_SUC);
#else
		bnVM(dest, "./testdata/vm/err", EXPECT_ERR);
		bnVM(dest, "./testdata/vm/suc", EXPECT_SUC);
#endif
        dump_result("VM", dest);
}

void bnRunTest() {
        GPtrArray* dest = g_ptr_array_new();
#if _WIN32
        bnRun(dest, "testdata\\vm\\err", EXPECT_ERR);
        bnRun(dest, "testdata\\vm\\suc", EXPECT_SUC);
#else
		bnRun(dest, "./testdata/vm/err", EXPECT_ERR);
		bnRun(dest, "./testdata/vm/suc", EXPECT_SUC);
#endif
        dump_result("RUN", dest);
}