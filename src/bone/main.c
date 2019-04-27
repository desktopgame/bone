#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <glib.h>
#include <stdio.h>

static int helloBone() {
        printf("hello, bone\n");
        return 0;
}

static void helloBoneTest() { CU_ASSERT(helloBone() == 0); }

static void runTest() {
        CU_pSuite testSuite;

        CU_initialize_registry();
        testSuite = CU_add_suite("boneTestSuite", NULL, NULL);
        CU_add_test(testSuite, "helloBoneTest", helloBoneTest);
        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
        CU_cleanup_registry();
}

int main(int argc, char *argv[]) {
        runTest();
        return 0;
}