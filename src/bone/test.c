#include "test.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include "bone.h"
#include "parse/parser.h"

static int bnParse(const char* dir) {
        GError* err = NULL;
        GDir* dirp = g_dir_open(dir, 0, &err);
        gchar* file = ".";
        while ((file = g_dir_read_name(dirp)) != NULL) {
                g_printf("%s: f\n", file);
        }
        g_dir_close(dirp);
        return 0;
}

void bnParseTest() { CU_ASSERT(bnParse("./testdata/parse/fail") == 0); }