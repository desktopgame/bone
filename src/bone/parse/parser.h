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
#endif