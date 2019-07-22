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

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#if defined(_MSC_VER)
size_t windows_getline(char** lineptr, size_t* n, FILE* stream) {
        char* bufptr = NULL;
        char* p = bufptr;
        size_t size;
        int c;

        if (lineptr == NULL) {
                return -1;
        }
        if (stream == NULL) {
                return -1;
        }
        if (n == NULL) {
                return -1;
        }
        bufptr = *lineptr;
        size = *n;

        c = fgetc(stream);
        if (c == EOF) {
                return -1;
        }
        if (bufptr == NULL) {
                bufptr = malloc(128);
                if (bufptr == NULL) {
                        return -1;
                }
                size = 128;
        }
        p = bufptr;
        while (c != EOF) {
                if ((p - bufptr) > (size - 1)) {
                        size = size + 128;
                        bufptr = realloc(bufptr, size);
                        if (bufptr == NULL) {
                                return -1;
                        }
                }
                *p++ = c;
                if (c == '\n') {
                        break;
                }
                c = fgetc(stream);
        }

        *p++ = '\0';
        *lineptr = bufptr;
        *n = size;

        return p - bufptr - 1;
}
#endif

#if defined(_MSC_VER)
#define xgetline(line, n, stream) (windows_getline(line, n, stream))
#else
#define xgetline(line, n, stream) (getline(line, n, stream))
#endif


int bnInteractive(FILE* in) {
        int status = 0;
        bool r = true;
        char* line;
        size_t len;
        ssize_t read;
        bnInterpreter* bone = bnNewInterpreter("stdin", 0, NULL);
        bnFrame* current = NULL;
        while (r) {
                if ((read = xgetline(&line, &len, in)) == -1) {
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