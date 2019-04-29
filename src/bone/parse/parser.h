#ifndef BONE_PARSE_PARSER_H
#define BONE_PARSE_PARSER_H
#include "ast.h"

typedef enum bnParserInputTag {
        BN_PARSER_INPUT_FROM_SOURCE,
        BN_PARSER_INPUT_FROM_FILE
} bnParserInputTag;

bnAST* bnParseFile(const char* filename);

bnAST* bnParseString(const char* source);

bnParserInputTag bnGetParserInputTag();

void bnBeginStringLit();

void bnAppendStringLit(char c);

bnAST* bnEndStringLit();

void bnSetParseLine(int line);

int bnGetParseLine();

void bnSetParseColumn(int column);

int bnGetParseColumn();

void bnPushParseLine(int lineno);

int bnPopParseLine();
#endif