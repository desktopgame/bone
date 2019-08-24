#include "interactive.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../config.inc"
#include "../il/il_toplevel.h"
#include "../parse/ast.h"
#include "../parse/ast2il.h"
#include "../parse/parser.h"
#include "enviroment.h"
#include "frame.h"
#include "heap.h"
#include "interpreter.h"
#include "vm.h"

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

static void show_gutter();
static void remove_escapes(GString* buf);
static void read_line(FILE* in, GString* buf);

int bnInteractive(FILE* in) {
        int status = 0;
        bool r = true;
        bnInterpreter* bone = bnNewInterpreter("stdin", 0, NULL);
        bnFrame* current = NULL;
        GString* line = g_string_new(NULL);
        int loops = 0;
        //バージョン情報を表示
        printf("bone %s\n", BUILD_VERSION);
        printf("launched a interactive mode.\n");
        printf("break from loop can by type quit or exit.\n");
        printf("if want input multilines, please line terminated by escape.\n");
        //メインループ開始
        while (r) {
                show_gutter();
                if (loops > 0) {
                        g_string_erase(line, 0, line->len);
                        read_line(in, line);
                } else {
                        const char* initCode = "{} <- load(\"repl.bn\");";
                        g_string_append(line, initCode);
                        printf("%s\n", initCode);
                        loops++;
                }
                // 空行
                if (line->len == 0) {
                        continue;
                }
                // exit, quitで終了する
                if (!strcmp(line->str, "exit") || !strcmp(line->str, "quit")) {
                        r = false;
                        break;
                }
                //最後がエスケープで終了するなら、継続して読み込む
                while (g_str_has_suffix(line->str, "\\")) {
                        remove_escapes(line);
                        show_gutter();
                        read_line(in, line);
                }
                remove_escapes(line);
                // 1行をASTにパースする
                bnAST* a = bnParseString(bone->pool, line->str);
                if (a == NULL) {
                        continue;
                }
                //中間表現へ変換
                bnILToplevel* top = bnAST2IL(a);
                //コード生成
                bnEnviroment* env =
                    bnNewEnviroment(bnIntern(bone->pool, "stdin"));
                bnGenerateILTopLevel(bone, top, env);
                bnWriteCode(env, BN_OP_DEFER_NEXT);
                //実行
                bnFrame* fr = bnNewFrame();
                bnWriteBuiltin(bone, fr, bone->pool);
                if (current != NULL) {
                        //以前の実行の情報を引き継ぐ
                        bnInjectFrame(current->variableTable, fr);
                        bnDeleteFrame(current);
                }
                bone->frame = fr;
                bnExecute(bone, env, fr);
                bnDeleteEnviroment(env);
                bnDeleteILTopLevel(top);
                bnDeleteAST(a);
                current = fr;
        }
        g_string_free(line, TRUE);
        bnGC(bone);
        bnDeleteFrame(bone->frame);
        bone->frame = NULL;
        g_hash_table_remove_all(bone->externTable);
        bnGC(bone);
        bnDeleteInterpreter(bone);
        return status;
}

static void show_gutter() {
        printf(">>> ");
        fflush(stdout);
}

static void remove_escapes(GString* buf) {
        int len = 0;
        int pos = buf->len - 1;
        while (pos > 0) {
                char c = buf->str[pos];
                if (c != '\\') {
                        break;
                }
                len++;
                pos--;
        }
        if (len > 0) {
                g_string_erase(buf, pos + 1, len);
        }
}

static void read_line(FILE* in, GString* buf) {
        char last = '\0';
        while (1) {
                int c = fgetc(in);
                if (c == EOF || c == '\n') {
                        break;
                }
                last = c;
                bnGStringAppendC(buf, c);
        }
}