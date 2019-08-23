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

int main(int argc, char* argv[]) {
#if _MSC_VER && DEBUG
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
        bnInitArgs(argc, argv);
        bnInitIO();
        int status = 0;
        if (argc == 1) {
                //デバッグモードならテスト、そうでなければインタラクティブモードを起動する。
#if DEBUG
                status = bnTest("testdata");
#else
                bnInteractive(stdin);
#endif
        } else if (argc >= 2) {
                //拡張子が指定されていなければ付け足す。
                GString* input = g_string_new(argv[1]);
                if (!g_str_has_suffix(input->str, ".bn") &&
                    !g_str_has_suffix(input->str, ".in")) {
                        g_string_append(input, ".bn");
                }
                GString* exeDir = bnGetExecutableFileDir();
                gchar* pluginDir =
                    g_build_filename(exeDir->str, "plugins", NULL);
                bnInterpreter* bone = bnNewInterpreter(input->str, argc, argv);
                bnLink(bone, pluginDir);
                status = bnEval(bone);
                bnDeleteInterpreter(bone);
                g_free(pluginDir);
                g_string_free(input, TRUE);
                g_string_free(exeDir, TRUE);
        }
#if _MSC_VER && DEBUG
        _CrtDumpMemoryLeaks();
#endif
        return status;
}