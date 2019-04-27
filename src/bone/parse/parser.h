#ifndef CALC_PARSE_PARSER_H
#define CALC_PARSE_PARSER_H
#include "ast.h"

typedef enum parser_input_tag {
        parser_input_from_source,
        parser_input_from_file
} parser_input_tag;

ast* parse_from_file(const char* filename);

ast* parse_from_source(const char* source);

parser_input_tag parse_get_input_tag();
#endif