#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <stdio.h>
#include "parse/parser.h"

static int fileParse() {
        bnAST* ret = bnParseFile("sample.txt");
        int code = ret == NULL ? 1 : 0;
        bnDumpAST(ret);
        bnDeleteAST(ret);
        return code;
}

static void fileParseTest() { CU_ASSERT(fileParse() == 0); }

static void runTest() {
        CU_pSuite testSuite;

        CU_initialize_registry();
        testSuite = CU_add_suite("boneTestSuite", NULL, NULL);
        CU_add_test(testSuite, "fileParseTest", fileParseTest);
        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
        CU_cleanup_registry();
}

int main(int argc, char* argv[]) {
        runTest();
        return 0;
}