#include "test.h"
#include <assert.h>
#include <string.h>
#include "bone.h"
#include "cunit.h"
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
#include "util/string.h"
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
                if (!g_str_has_suffix(path, ".in") || strstr(path, "_R") ||
                    strstr(path, "_Inc")) {
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
#if _WIN32
                bnLink(bone, "testdata\\plugins");
#else
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
        printf("\n---\n");
        printf("FAIL [%s/%d]\n", header, src->len);
        printf("---\n");
        for (int i = 0; i < src->len; i++) {
                GString* str = g_ptr_array_index(src, i);
                printf("    %s\n", str->str);
        }
        g_ptr_array_set_free_func(src, free_gstring_with_segment);
        g_ptr_array_free(src, TRUE);
}

typedef enum test_mask {
        test_mask_parse = 1 << 0,
        test_mask_vm = 1 << 1,
        test_mask_run = 1 << 2,
        test_mask_expect_pass = 1 << 3,
        test_mask_expect_fail = 1 << 4,
} test_mask;

typedef enum test_result {
        test_result_pass,
        test_result_fail,
        test_result_unknown,
} test_result;

static test_result test_check_parse(const char* testDir, const char* testName,
                                    const gchar* path, int flags) {
        struct bnStringPool* pool = bnNewStringPool();
        test_result ret = test_result_pass;
        // clear parse result file
        char out[512];
        memset(out, '\0', 512);
        sprintf(out, "%s/out/%s.out", testDir, testName);
        // gchar* out = g_strconcat(testDir, ".out", NULL);
        if (g_file_test(out, (G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR))) {
                g_remove(out);
        }
        // parse and test
        bnAST* a = bnParseFile(pool, path);
        if (flags & test_mask_expect_pass) {
                bnILToplevel* iltop = bnAST2IL(a);
                writeIL(out, pool, iltop);
                if (a == NULL) {
                        ret = test_result_fail;
                }
                bnDeleteAST(a);
                bnDeleteILTopLevel(iltop);
        } else if (flags & test_mask_expect_fail) {
                if (a != NULL) {
                        ret = test_result_fail;
                }
        }
        bnDeleteStringPool(pool);
        return ret;
}

static test_result test_check_vm(const char* testDir, const char* testName,
                                 const gchar* path, int flags) {
        bnInterpreter* bone = bnNewInterpreter("", bnArgc(), bnArgv());
        test_result ret = test_result_pass;
        // clear parse result file
        char out[512];
        memset(out, '\0', 512);
        sprintf(out, "%s/out/%s.out", testDir, testName);
        if (g_file_test(out, (G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR))) {
                g_remove(out);
        }
        // parse and test
        bnAST* a = bnParseFile(bone->pool, path);
        if (flags & test_mask_expect_pass) {
                bnILToplevel* iltop = bnAST2IL(a);
                bnEnviroment* env = bnNewEnviroment(bnIntern(bone->pool, path));
                bnGenerateILTopLevel(bone, iltop, env);
                writeEnv(out, bone->pool, env);
                if (a == NULL) {
                        ret = test_result_fail;
                }
                bnDeleteAST(a);
                bnDeleteILTopLevel(iltop);
                bnDeleteEnviroment(env);
        } else if (flags & test_mask_expect_fail) {
                if (a != NULL) {
                        ret = test_result_fail;
                }
        }
        bnDeleteInterpreter(bone);
        return ret;
}
static test_result test_check_run(const char* testDir, const char* testName,
                                  const gchar* path, int flags) {
        bool panicTest = strstr(path, "_P");
        test_result res = test_result_pass;
        // clear run result file
        char out[512];
        memset(out, '\0', 512);
        sprintf(out, "%s/out/%s.std.out", testDir, testName);
        if (g_file_test(out, (G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR))) {
                g_remove(out);
        }
#if !defined(_WIN32)
        FILE* soutfp = fopen(out, "w");
        FILE* _stdout = stdout;
        stdout = soutfp;
#endif
        // parse and test
        bnInterpreter* bone = bnNewInterpreter(path, bnArgc(), bnArgv());
#if _WIN32
        bnLink(bone, "testdata\\plugins");
#else
        bnLink(bone, "./testdata/plugins");
#endif
        int ret = bnEval(bone);
#if !defined(_WIN32)
        stdout = _stdout;
        fclose(soutfp);
        writeFile(out);
#endif
        if (flags & test_mask_expect_pass) {
                if (ret != 0 && !panicTest) {
                        res = test_result_fail;
                }
        } else if (flags & test_mask_expect_fail) {
                if (ret == 0) {
                        res = test_result_fail;
                }
        }
        BN_CHECK_MEM();
        bnDeleteInterpreter(bone);
        return res;
}

static test_result test_check(const char* testDir, const char* testName,
                              const gchar* path, int flags) {
        if (flags & test_mask_parse) {
                return test_check_parse(testDir, testName, path, flags);
        } else if (flags & test_mask_vm) {
                return test_check_vm(testDir, testName, path, flags);
        } else if (flags & test_mask_run) {
                return test_check_run(testDir, testName, path, flags);
        } else {
                abort();
        }
        return test_result_unknown;
}

static test_result test_run(const char* testDir, const gchar* path) {
        /*
        // filename rule: file_TypeExpect
        // examples
        //
        // file_ParsePass:
        // parse test.
        // expect pass.
        //
        // file_VMFail:
        // code generate test.
        // expect fail.
        //
        // file_RunPass:
        // run test:
        // expect pass.
        //
        */
        // testdata/file_type.in
        int pos = bnLastPathComponent(path);
        // file_type.in
        gchar* filename = path + pos;
        // file_type
        gchar* filename_wext = g_strdup(filename);
        int dotpos = strlen(filename_wext) - 3;
        memset(filename_wext + dotpos, '\0', 3);
        // file type
        char* underbar = strstr(filename_wext, "_");
        gchar* type = NULL;
        if (underbar != NULL) {
                int underbarPos = underbar - filename_wext;
                type = filename_wext + underbarPos + 1;
        }
        int flags = 0;
        // check test type
        if (g_str_has_prefix(type, "Parse")) {
                flags |= test_mask_parse;
                type += 5;
        } else if (g_str_has_prefix(type, "VM")) {
                flags |= test_mask_vm;
                type += 2;
        } else if (g_str_has_prefix(type, "Run")) {
                flags |= test_mask_run;
                type += 3;
        } else {
                printf("no matches to rule: %s\n", path);
                abort();
        }
        // check expect type
        if (g_str_has_prefix(type, "Pass")) {
                flags |= test_mask_expect_pass;
                type += 4;
        } else if (g_str_has_prefix(type, "Fail")) {
                flags |= test_mask_expect_fail;
                type += 4;
        } else {
                printf("no matches to rule: %s\n", path);
                abort();
        }
        printf("test: %s\n", filename);
        test_result result = test_check(testDir, filename_wext, path, flags);
        g_free(filename_wext);
        return result;
}

void bnTest(const char* dir) {
        GError* err = NULL;
        GDir* dirp = g_dir_open(dir, 0, &err);
        gchar* file = ".";
        gchar* cwd = g_get_current_dir();
        while ((file = g_dir_read_name(dirp)) != NULL) {
                gchar* path = g_build_filename(cwd, dir, file, NULL);
                if (!g_str_has_suffix(path, ".in")) {
                        continue;
                }
                test_run(dir, path);
                g_free(path);
        }
        g_dir_close(dirp);
        g_free(cwd);
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