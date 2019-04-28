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
		$$ = ast_new_unary(BN_AST_POSITIVE, $2);
	}
	| SUB expression %prec NEGATIVE
	{
		$$ = ast_new_unary(BN_AST_NEGATIVE, $2);
	}
	| expression ADD expression
	{
		$$ = ast_new_binary(BN_AST_PLUS, $1, $3);
	}
	| expression SUB expression
	{
		$$ = ast_new_binary(BN_AST_MINUS, $1, $3);
	}
	| expression MUL expression
	{
		$$ = ast_new_binary(BN_AST_MULTIPLY, $1, $3);
	}
	| expression DIV expression
	{
		$$ = ast_new_binary(BN_AST_DIVIDE, $1, $3);
	}
	| expression MOD expression
	{
		$$ = ast_new_binary(BN_AST_MODULO, $1, $3);
	}
	| expression BIT_OR expression
	{
		$$ = ast_new_binary(BN_AST_BIT_OR, $1, $3);
	}
	| expression EQUAL expression
	{
		$$ = ast_new_binary(BN_AST_EQUAL, $1, $3);
	}
	| expression NOTEQUAL expression
	{
		$$ = ast_new_binary(BN_AST_NOTEQUAL, $1, $3);
	}
	| expression BIT_AND expression
	{
		$$ = ast_new_binary(BN_AST_BIT_AND, $1, $3);
	}
	| expression EXC_OR expression
	{
		$$ = ast_new_binary(BN_AST_EXC_OR, $1, $3);
	}
	| expression LOGIC_OR expression
	{
		$$ = ast_new_binary(BN_AST_LOGIC_OR, $1, $3);
	}
	| expression LOGIC_AND expression
	{
		$$ = ast_new_binary(BN_AST_LOGIC_AND, $1, $3);
	}
	| expression ASSIGN expression
	{
		$$ = ast_new_binary(BN_AST_ASSIGN, $1, $3);
	}
	| expression ADD_ASSIGN expression
	{
		$$ = ast_new_binary(BN_AST_PLUS_ASSIGN, $1, $3);
	}
	| expression SUB_ASSIGN expression
	{
		$$ = ast_new_binary(BN_AST_MINUS_ASSIGN, $1, $3);
	}
	| expression MUL_ASSIGN expression
	{
		$$ = ast_new_binary(BN_AST_MULTIPLY_ASSIGN, $1, $3);
	}
	| expression DIV_ASSIGN expression
	{
		$$ = ast_new_binary(BN_AST_DIVIDE_ASSIGN, $1, $3);
	}
	| expression MOD_ASSIGN expression
	{
		$$ = ast_new_binary(BN_AST_MODULO_ASSIGN, $1, $3);
	}
	| expression AND_ASSIGN expression
	{
		$$ = ast_new_binary(BN_AST_AND_ASSIGN, $1, $3);
	}
	| expression OR_ASSIGN expression
	{
		$$ = ast_new_binary(BN_AST_OR_ASSIGN, $1, $3);
	}
	| expression EXC_OR_ASSIGN expression
	{
		$$ = ast_new_binary(BN_AST_EXC_OR_ASSIGN, $1, $3);
	}
	| expression LSHIFT_ASSIGN expression
	{
		$$ = ast_new_binary(BN_AST_LSHIFT_ASSIGN, $1, $3);
	}
	| expression RSHIFT_ASSIGN expression
	{
		$$ = ast_new_binary(BN_AST_RSHIFT_ASSIGN, $1, $3);
	}
	| expression GT expression
	{
		$$ = ast_new_binary(BN_AST_GT, $1, $3);
	}
	| expression GE expression
	{
		$$ = ast_new_binary(BN_AST_GE, $1, $3);
	}
	| expression LT expression
	{
		$$ = ast_new_binary(BN_AST_LT, $1, $3);
	}
	| expression LE expression
	{
		$$ = ast_new_binary(BN_AST_LE, $1, $3);
	}
	| expression LSHIFT expression {
		$$ = ast_new_binary(BN_AST_LSHIFT, $1, $3);
	}
	| expression RSHIFT expression {
		$$ = ast_new_binary(BN_AST_RSHIFT, $1, $3);
	}
	| CHILDA expression
	{
		$$ = ast_new_unary(BN_AST_CHILDA, $2);
	}
	| NOT expression
	{
		$$ = ast_new_unary(BN_AST_NOT, $2);
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