#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <glib.h>
#include <stdio.h>

static int add(int a, int b) { return a; }

static void addTest() {
        printf("test\n");
        CU_ASSERT(add(0, 0) == 0);
        CU_ASSERT(add(1, 0) == 1);
        CU_ASSERT(add(1, 1) == 2);
}

static void runTest() {
        CU_pSuite testSuite;

        CU_initialize_registry();
        testSuite = CU_add_suite("addTestSuite", NULL, NULL);
        CU_add_test(testSuite, "addTest", addTest);
        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
        CU_cleanup_registry();
}

int main(int argc, char *argv[]) {
        runTest();
        return 0;
}