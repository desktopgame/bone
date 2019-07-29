
#if !defined(_WIN32)
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#endif
#if _MSC_VER && DEBUG
#include <crtdbg.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "bone.h"
#include "glib.h"
#include "runtime/interactive.h"
#include "runtime/interpreter.h"
#include "test.h"
#include "util/args.h"
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
        bnVMTest();
        bnRunTest();
#endif
}

int main(int argc, char* argv[]) {
#if _MSC_VER && DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
        bnInitArgs(argc, argv);
        bnInitIO();
        int status = 0;
        if (argc == 1) {
                //$ bone
                // start interactive mode
#if DEBUG
                runTest();
#else
                bnInteractive(stdin);
#endif
        } else if (argc >= 2) {
                //$ bone file, args...
                GString* input = g_string_new(argv[1]);
                if (!g_str_has_suffix(input->str, ".bn")) {
                        g_string_append(input, ".bn");
                }
                bnInterpreter* bone = bnNewInterpreter(input->str, argc, argv);
                bnLink(bone, ".");
                status = bnEval(bone);
                bnDeleteInterpreter(bone);
                g_string_free(input, TRUE);
        }
#if DEBUG
#if _MSC_VER
		_CrtDumpMemoryLeaks();
#else
        bnDumpMemoryLeaks(stdout);
#endif
#endif
        return status;
}