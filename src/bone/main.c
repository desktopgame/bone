
#if !defined(_WIN32)
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "test.h"
#include "util/io.h"
#include "util/memory.h"

static void runTest() {
#if !defined(_WIN32)
        CU_pSuite testSuite;

        CU_initialize_registry();
        testSuite = CU_add_suite("boneTestSuite", NULL, NULL);
        CU_add_test(testSuite, "bnParseTest", bnParseTest);
        CU_add_test(testSuite, "bnVMTest", bnVMTest);
        CU_add_test(testSuite, "bnRunTest", bnRunTest);
        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
        CU_cleanup_registry();
#else
        bnParseTest();
        bnStringPoolTest();
        bnVMTest();
        bnRunTest();
#endif
}

int main(int argc, char* argv[]) {
        bnInitIO();
        runTest();
        bnDumpMemoryLeaks(stdout);
        return 0;
}