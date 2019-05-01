#include "test.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <string.h>
#include "bone.h"
#include "il/il_toplevel.h"
#include "parse/ast2il.h"
#include "parse/parser.h"
#include "util/string_pool.h"

#define EXPECT_ERR (0)
#define EXPECT_SUC (1)

static void writeIL(const gchar* out, bnILToplevel* il) {
        if (il == NULL) {
                return;
        }
        FILE* fp = fopen(out, "w");
        if (fp == NULL) {
                perror("writeIL");
                return;
        }
        bnDumpILTopLevel(fp, il, 0);
        fclose(fp);
}

static void writeAST(const gchar* out, bnAST* a) {
        if (a == NULL) {
                return;
        }
        FILE* fp = fopen(out, "w");
        if (fp == NULL) {
                perror("writeAST");
                return;
        }
        bnDumpAST(fp, a);
        fclose(fp);
}

static int bnParse(const char* dir, int flag) {
        GError* err = NULL;
        GDir* dirp = g_dir_open(dir, 0, &err);
        gchar* file = ".";
        gchar* cwd = g_get_current_dir();
        while ((file = g_dir_read_name(dirp)) != NULL) {
                // do process only a suffix .in
                gchar* path = g_build_filename(cwd, dir, file, NULL);
                if (!g_str_has_suffix(path, ".in")) {
                        continue;
                }
                // clear parse result file
                gchar* out = g_strconcat(path, ".out", NULL);
                if (g_file_test(out,
                                (G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR))) {
                        g_remove(out);
                }
                // parse and test
                bnAST* a = bnParseFile(path);
                printf("%s\n", path);
                if (flag == EXPECT_SUC) {
                        bnILToplevel* iltop = bnAST2IL(a);
                        writeIL(out, iltop);
                        CU_ASSERT(a != NULL);
                        bnDeleteAST(a);
                        bnDeleteILTopLevel(iltop);
                } else if (flag == EXPECT_ERR) {
                        CU_ASSERT(a == NULL);
                }
                g_free(path);
                g_free(out);
        }
        g_dir_close(dirp);
        g_free(cwd);
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