#include "test.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include "bone.h"
#include "parse/parser.h"

#define EXPECT_ERR (0)
#define EXPECT_SUC (1)

static int bnParse(const char* dir, int flag) {
        GError* err = NULL;
        GDir* dirp = g_dir_open(dir, 0, &err);
        gchar* file = ".";
        gchar* cwd = g_get_current_dir();
        while ((file = g_dir_read_name(dirp)) != NULL) {
                gchar* path = g_build_filename(cwd, dir, file, NULL);
                bnAST* a = bnParseFile(path);
                printf("%s\n", path);
                if (flag == EXPECT_SUC) {
                        CU_ASSERT(a != NULL);
                        bnDeleteAST(a);
                } else if (flag == EXPECT_ERR) {
                        CU_ASSERT(a == NULL);
                }
                g_free(path);
        }
        g_dir_close(dirp);
        g_free(cwd);
        return 0;
}

void bnParseTest() {
        bnParse("./testdata/parse/err", EXPECT_ERR);
        bnParse("./testdata/parse/suc", EXPECT_SUC);
}