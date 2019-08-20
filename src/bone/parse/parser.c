#include "parser.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../bone.h"

static bnParserInputTag gParserInputTag;
static int gLine = -1;
static int gColumn = -1;
static GString *gStr = NULL;
static GQueue *gLineQ = NULL;
static GRecMutex gMutex;
static struct bnStringPool *gPool;
static void init_string_lit();
static void free_lq();

bnAST *bnParseFile(struct bnStringPool *pool, const char *filename) {
        g_rec_mutex_lock(&gMutex);
        gParserInputTag = BN_PARSER_INPUT_FROM_FILE;
        gPool = pool;
        extern FILE *yyin;
        extern void yy_calc_start(void);
        extern int yyparse(void);
        extern bnAST *yy_release();
        FILE *fp = fopen(filename, "r");
        if (fp == NULL) {
                perror("bnParseFile");
                g_rec_mutex_unlock(&gMutex);
                return NULL;
        }
        yy_calc_start();
        yyin = fp;
        gLineQ = g_queue_new();
        if (yyparse()) {
                free_lq();
                g_rec_mutex_unlock(&gMutex);
                bnCleanAST(true);
                return NULL;
        }
        free_lq();
        bnAST *ret = yy_release();
        g_rec_mutex_unlock(&gMutex);
        bnCleanAST(false);
        return ret;
}

bnAST *bnParseString(struct bnStringPool *pool, const char *source) {
        g_rec_mutex_lock(&gMutex);
        gParserInputTag = BN_PARSER_INPUT_FROM_SOURCE;
        gPool = pool;
        extern void yy_setstr(char *source);
        extern void yy_clearstr();
        extern void yy_calc_start(void);
        extern int yyparse(void);
        extern bnAST *yy_release();
        yy_calc_start();
        yy_setstr(strdup(source));
        gLineQ = g_queue_new();
        if (yyparse()) {
                yy_clearstr();
                free_lq();
                g_rec_mutex_unlock(&gMutex);
                bnCleanAST(true);
                return NULL;
        }
        yy_clearstr();
        free_lq();
        bnAST *ret = yy_release();
        g_rec_mutex_unlock(&gMutex);
        bnCleanAST(false);
        return ret;
}

bnParserInputTag bnGetParserInputTag() { return gParserInputTag; }

void bnBeginStringLit() {
        init_string_lit();
        BN_CHECK_MEM();
}

bnStringView bnInternIdentifier(const char *str) {
        return bnIntern(gPool, str);
}

void bnAppendStringLit(char c) {
        assert(gStr != NULL);
        bnGStringAppendC(gStr, c);
        BN_CHECK_MEM();
}

bnAST *bnEndStringLit() {
        bnAST *ret = bnNewStringAST(bnInternIdentifier(gStr->str));
        g_string_free(gStr, TRUE);
        gStr = NULL;
        BN_CHECK_MEM();
        return ret;
}

void bnSetParseLine(int line) { gLine = line; }

int bnGetParseLine() { return gLine; }

void bnSetParseColumn(int column) { gColumn = column; }

int bnGetParseColumn() { return gColumn; }

void bnPushParseLine(int lineno) {
        if (gLineQ == NULL) {
                return;
        }
        g_queue_push_head(gLineQ, GINT_TO_POINTER(lineno));
}

int bnPopParseLine() {
        guint len = g_queue_get_length(gLineQ);
        if (len == 0) {
                return -1;
        }
        return (int)g_queue_pop_head(gLineQ);
}

// private
static void init_string_lit() {
        assert(gStr == NULL);
        gStr = g_string_new(NULL);
}

static void free_lq() {
        g_queue_free(gLineQ);
        gLineQ = NULL;
}