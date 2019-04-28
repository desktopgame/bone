#include "parser.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static bnParserInputTag gParserInputTag;

bnAST *bnParseFile(const char *filename) {
        gParserInputTag = BN_PARSER_INPUT_FROM_FILE;
        extern FILE *yyin;
        extern void yy_calc_start(void);
        extern int yyparse(void);
        extern bnAST *yy_release();
        FILE *fp = fopen(filename, "r");
        if (fp == NULL) {
                printf("can't opened file: %s", filename);
                return NULL;
        }
        yy_calc_start();
        yyin = fp;
        if (yyparse()) {
                //失敗
                return NULL;
        }
        return yy_release();
}

bnAST *bnParseString(const char *source) {
        gParserInputTag = BN_PARSER_INPUT_FROM_SOURCE;
        extern void yy_setstr(char *source);
        extern void yy_clearstr();
        extern void yy_calc_start(void);
        extern int yyparse(void);
        extern bnAST *yy_release();
        yy_calc_start();
        yy_setstr(strdup(source));
        if (yyparse()) {
                yy_clearstr();
                return NULL;
        }
        yy_clearstr();
        return yy_release();
}

bnParserInputTag bnGetParserInputTag() { return gParserInputTag; }