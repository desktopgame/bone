%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#define YYDEBUG 1
#define YYERROR_VERBOSE 1
%}
%union {
	ast* ast_value;
	char* svalue;
}

%locations
%token <ast_value>			INT
%token <ast_value>			DOUBLE
%token 	ADD SUB MUL DIV MOD
		ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
		AND_ASSIGN OR_ASSIGN LSHIFT_ASSIGN RSHIFT_ASSIGN EXC_OR_ASSIGN
		EQUAL NOTEQUAL INC DEC
		GT GE LT LE LSHIFT RSHIFT
		NOT BIT_AND BIT_OR LOGIC_AND LOGIC_OR LP RP
		EXC_OR
		IDENT
%type <ast_value> expression primary
%left EQUAL NOTEQUAL
%left GT GE LT LE
%left LOGIC_AND
%left LOGIC_OR
%left BIT_AND
%left BIT_OR
%left EXC_OR
%left LSHIFT RSHIFT
%left ADD SUB
%left MUL DIV MOD
%left NEGATIVE POSITIVE
%right ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN AND_ASSIGN OR_ASSIGN LSHIFT_ASSIGN RSHIFT_ASSIGN EXC_OR_ASSIGN
%right CHILDA NOT
%%
program
	: expression
	{
		extern void yy_register(ast* a);
		yy_register($1);
	}
	;
expression
	: primary
	| ADD expression %prec POSITIVE
	{
		$$ = ast_new_unary(ast_pos, $2);
	}
	| SUB expression %prec NEGATIVE
	{
		$$ = ast_new_unary(ast_neg, $2);
	}
	| expression ADD expression
	{
		$$ = ast_new_binary(ast_add, $1, $3);
	}
	| expression SUB expression
	{
		$$ = ast_new_binary(ast_sub, $1, $3);
	}
	| expression MUL expression
	{
		$$ = ast_new_binary(ast_mul, $1, $3);
	}
	| expression DIV expression
	{
		$$ = ast_new_binary(ast_div, $1, $3);
	}
	| expression MOD expression
	{
		$$ = ast_new_binary(ast_mod, $1, $3);
	}
	| expression BIT_OR expression
	{
		$$ = ast_new_binary(ast_bit_or, $1, $3);
	}
	| expression EQUAL expression
	{
		$$ = ast_new_binary(ast_equal, $1, $3);
	}
	| expression NOTEQUAL expression
	{
		$$ = ast_new_binary(ast_notequal, $1, $3);
	}
	| expression BIT_AND expression
	{
		$$ = ast_new_binary(ast_bit_and, $1, $3);
	}
	| expression EXC_OR expression
	{
		$$ = ast_new_binary(ast_exc_or, $1, $3);
	}
	| expression LOGIC_OR expression
	{
		$$ = ast_new_binary(ast_logic_or, $1, $3);
	}
	| expression LOGIC_AND expression
	{
		$$ = ast_new_binary(ast_logic_and, $1, $3);
	}
	| expression ASSIGN expression
	{
		$$ = ast_new_binary(ast_assign, $1, $3);
	}
	| expression ADD_ASSIGN expression
	{
		$$ = ast_new_binary(ast_add_assign, $1, $3);
	}
	| expression SUB_ASSIGN expression
	{
		$$ = ast_new_binary(ast_sub_assign, $1, $3);
	}
	| expression MUL_ASSIGN expression
	{
		$$ = ast_new_binary(ast_mul_assign, $1, $3);
	}
	| expression DIV_ASSIGN expression
	{
		$$ = ast_new_binary(ast_div_assign, $1, $3);
	}
	| expression MOD_ASSIGN expression
	{
		$$ = ast_new_binary(ast_mod_assign, $1, $3);
	}
	| expression AND_ASSIGN expression
	{
		$$ = ast_new_binary(ast_and_assign, $1, $3);
	}
	| expression OR_ASSIGN expression
	{
		$$ = ast_new_binary(ast_or_assign, $1, $3);
	}
	| expression EXC_OR_ASSIGN expression
	{
		$$ = ast_new_binary(ast_exc_or_assign, $1, $3);
	}
	| expression LSHIFT_ASSIGN expression
	{
		$$ = ast_new_binary(ast_lshift_assign, $1, $3);
	}
	| expression RSHIFT_ASSIGN expression
	{
		$$ = ast_new_binary(ast_rshift_assign, $1, $3);
	}
	| expression GT expression
	{
		$$ = ast_new_binary(ast_gt, $1, $3);
	}
	| expression GE expression
	{
		$$ = ast_new_binary(ast_ge, $1, $3);
	}
	| expression LT expression
	{
		$$ = ast_new_binary(ast_lt, $1, $3);
	}
	| expression LE expression
	{
		$$ = ast_new_binary(ast_le, $1, $3);
	}
	| expression LSHIFT expression {
		$$ = ast_new_binary(ast_lshift, $1, $3);
	}
	| expression RSHIFT expression {
		$$ = ast_new_binary(ast_rshift, $1, $3);
	}
	| CHILDA expression
	{
		$$ = ast_new_unary(ast_childa, $2);
	}
	| NOT expression
	{
		$$ = ast_new_unary(ast_not, $2);
	}
	| LP expression RP
	{
		$$ = $2;
	}
	;
primary
	: INT
	| DOUBLE
	;
%%