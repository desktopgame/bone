#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <stdio.h>
#include "test.h"

static void runTest() {
        CU_pSuite testSuite;

        CU_initialize_registry();
        testSuite = CU_add_suite("boneTestSuite", NULL, NULL);
        CU_add_test(testSuite, "bnParseTest", bnParseTest);
        CU_add_test(testSuite, "bnStringPoolTest", bnStringPoolTest);
        CU_add_test(testSuite, "bnVMTest", bnVMTest);
        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
        CU_cleanup_registry();
}

int main(int argc, char* argv[]) {
        runTest();
        return 0;
}