#include "interactive.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../il/il_toplevel.h"
#include "../parse/ast.h"
#include "../parse/ast2il.h"
#include "../parse/parser.h"
#include "enviroment.h"
#include "frame.h"
#include "heap.h"
#include "interpreter.h"
#include "vm.h"

int bnInteractive(FILE* in) {
        int status = 0;
        bool r = true;
        char* line;
        size_t len;
        ssize_t read;
        bnInterpreter* bone = bnNewInterpreter("stdin", 0, NULL);
        bnFrame* current = NULL;
        while (r) {
                if ((read = getline(&line, &len, in)) == -1) {
                        perror("bnInteractive");
                        break;
                }
                if (line[read - 1] == '\n') {
                        line[read - 1] = '\0';
                }
                if (!strcmp(line, "exit") || !strcmp(line, "quit")) {
                        r = false;
                        break;
                }
                // line parse
                bnAST* a = bnParseString(bone->pool, line);
                if (a == NULL) {
                        continue;
                }
                // to il
                bnILToplevel* top = bnAST2IL(a);
                // gen code
                bnEnviroment* env =
                    bnNewEnviroment(bnIntern(bone->pool, "stdin"));
                bnGenerateILTopLevel(bone, top, env);
                g_ptr_array_add(env->codeArray, BN_OP_DEFER_NEXT);
                // run code
                bnFrame* fr = bnNewFrame();
                bnWriteDefaults(bone, fr, bone->pool);
                if (current != NULL) {
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
        free(line);
        bnGC(bone);
        bnDeleteFrame(bone->frame);
        bone->frame = NULL;
        g_hash_table_remove_all(bone->externTable);
        bnGC(bone);
        bnDeleteInterpreter(bone);
        return status;
}