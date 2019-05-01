#ifndef BONE_PARSE_PARSER_H
#define BONE_PARSE_PARSER_H
#include "../util/string_pool.h"
#include "ast.h"

typedef enum bnParserInputTag {
        BN_PARSER_INPUT_FROM_SOURCE,
        BN_PARSER_INPUT_FROM_FILE
} bnParserInputTag;

bnAST* bnParseFile(struct bnStringPool* pool, const char* filename);

bnAST* bnParseString(struct bnStringPool* pool, const char* source);

bnParserInputTag bnGetParserInputTag();

bnStringView bnInternIdentifier(const char* str);

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