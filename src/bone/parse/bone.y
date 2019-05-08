%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "../bone.h"
#define YYDEBUG 1
#define YYERROR_VERBOSE 1
%}
%union {
	bnAST* ast_value;
	bnStringView svvalue;
}

%locations
%token <ast_value>			INT
%token <ast_value>			DOUBLE
%token <ast_value>			STRING_LITERAL
%token <ast_value>			CHAR_LITERAL
%token <svvalue>			IDENT
%token 	ADD SUB MUL DIV MOD
		ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN
		AND_ASSIGN OR_ASSIGN LSHIFT_ASSIGN RSHIFT_ASSIGN EXC_OR_ASSIGN
		EQUAL NOTEQUAL INC DEC
		GT GE LT LE LSHIFT RSHIFT
		NOT BIT_AND BIT_OR LOGIC_AND LOGIC_OR LP RP LB RB IF ELSE
		EXC_OR DOTDOTDOT LSB RSB
		DOT COMMA SEMICOLON WHILE DEF RETURN_T SCOPE INJECTION PANIC
%type <ast_value>
	argument_list
	parameter_list
	statement_list
	statement_list_opt
	statement
	comp_stmt
	return_stmt
	injection_stmt
	panic_stmt
	expression
	expression_nobrace
	call_expr
	lambda_expr
	array_lit_expr
	lhs
	primary
%right ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN AND_ASSIGN OR_ASSIGN LSHIFT_ASSIGN RSHIFT_ASSIGN EXC_OR_ASSIGN
%left LOGIC_OR
%left LOGIC_AND
%left BIT_OR
%left EXC_OR
%left BIT_AND
%left EQUAL NOTEQUAL
%left GT GE LT LE
%left LSHIFT RSHIFT
%left ADD SUB INJECTION
%left MUL DIV MOD
%right CHILDA NOT NEGATIVE POSITIVE
%left DOT FUNCCALL LSB
%nonassoc LP
%%
program
	: statement_list
	{
		extern void yy_register(bnAST* a);
		yy_register($1);
	}
	| error '\n'
	{
		yyclearin;
	}
	;
argument_list
	: expression
	{
		$$ = bnNewArgumentAST($1);
	}
	| argument_list COMMA expression
	{
		$$ = bnNewArgumentListAST(bnNewArgumentAST($3), $1);
	}
	;
parameter_list
	: IDENT
	{
		$$ = bnNewParameterAST($1);
	}
	| parameter_list COMMA IDENT
	{
		$$ = bnNewParameterListAST(bnNewParameterAST($3), $1);
	}
	;
statement_list
	: statement
	{
		$$ = $1;
	}
	| statement_list statement
	{
		$$ = bnNewStatementListAST($2, $1);
	}
	;
statement_list_opt
	: /* empty */
	{
		$$ = bnNewBlankAST();
	}
	| statement_list
	{
		$$ = $1;
	}
	;
statement
	: expression SEMICOLON
	{
		$$ = bnNewExprStmtAST($1);
	}
	| comp_stmt
	| IF LP expression RP statement
	{
		$$ = bnNewIfAST($3, $5);
	}
	| IF LP expression RP statement ELSE statement
	{
		$$ = bnNewIfElseAST(bnNewIfAST($3, $5), $7);
	}
	| WHILE LP expression RP statement
	{
		$$ = bnNewWhileAST($3, $5);
	}
	| return_stmt
	| injection_stmt
	| panic_stmt
	;
comp_stmt
	: LB statement_list_opt RB
	{
		$$ = $2;
	}
	;
return_stmt
	: RETURN_T SEMICOLON
	{
		$$ = bnNewReturnAST(bnNewBlankAST());
	}
	;
injection_stmt
	: SCOPE INJECTION call_expr SEMICOLON
	{
		$$ = bnNewScopeInjectionAST($3);
	}
	;
panic_stmt
	: PANIC IDENT INJECTION expression SEMICOLON
	{
		$$ = bnNewPanicAST($2, $4);
	}
	;
expression
	: LP expression RP
	{
		$$ = $2;
	}
	| primary
	|  expression_nobrace
	;
expression_nobrace
	: ADD expression %prec POSITIVE
	{
		$$ = bnNewUnaryAST(BN_AST_POSITIVE, $2);
	}
	| SUB expression %prec NEGATIVE
	{
		$$ = bnNewUnaryAST(BN_AST_NEGATIVE, $2);
	}
	| expression INJECTION expression
	{
		$$ = bnNewObjectInjectionAST($1, $3);
	}
	| expression ADD expression
	{
		$$ = bnNewBinaryAST(BN_AST_PLUS, $1, $3);
	}
	| expression SUB expression
	{
		$$ = bnNewBinaryAST(BN_AST_MINUS, $1, $3);
	}
	| expression MUL expression
	{
		$$ = bnNewBinaryAST(BN_AST_MULTIPLY, $1, $3);
	}
	| expression DIV expression
	{
		$$ = bnNewBinaryAST(BN_AST_DIVIDE, $1, $3);
	}
	| expression MOD expression
	{
		$$ = bnNewBinaryAST(BN_AST_MODULO, $1, $3);
	}
	| expression BIT_OR expression
	{
		$$ = bnNewBinaryAST(BN_AST_BIT_OR, $1, $3);
	}
	| expression EQUAL expression
	{
		$$ = bnNewBinaryAST(BN_AST_EQUAL, $1, $3);
	}
	| expression NOTEQUAL expression
	{
		$$ = bnNewBinaryAST(BN_AST_NOTEQUAL, $1, $3);
	}
	| expression BIT_AND expression
	{
		$$ = bnNewBinaryAST(BN_AST_BIT_AND, $1, $3);
	}
	| expression EXC_OR expression
	{
		$$ = bnNewBinaryAST(BN_AST_EXC_OR, $1, $3);
	}
	| expression LOGIC_OR expression
	{
		$$ = bnNewBinaryAST(BN_AST_LOGIC_OR, $1, $3);
	}
	| expression LOGIC_AND expression
	{
		$$ = bnNewBinaryAST(BN_AST_LOGIC_AND, $1, $3);
	}
	| lhs ASSIGN expression
	{
		$$ = bnNewBinaryAST(BN_AST_ASSIGN, $1, $3);
	}
	| lhs ADD_ASSIGN expression
	{
		$$ = bnNewBinaryAST(BN_AST_PLUS_ASSIGN, $1, $3);
	}
	| lhs SUB_ASSIGN expression
	{
		$$ = bnNewBinaryAST(BN_AST_MINUS_ASSIGN, $1, $3);
	}
	| lhs MUL_ASSIGN expression
	{
		$$ = bnNewBinaryAST(BN_AST_MULTIPLY_ASSIGN, $1, $3);
	}
	| lhs DIV_ASSIGN expression
	{
		$$ = bnNewBinaryAST(BN_AST_DIVIDE_ASSIGN, $1, $3);
	}
	| lhs MOD_ASSIGN expression
	{
		$$ = bnNewBinaryAST(BN_AST_MODULO_ASSIGN, $1, $3);
	}
	| lhs AND_ASSIGN expression
	{
		$$ = bnNewBinaryAST(BN_AST_AND_ASSIGN, $1, $3);
	}
	| lhs OR_ASSIGN expression
	{
		$$ = bnNewBinaryAST(BN_AST_OR_ASSIGN, $1, $3);
	}
	| lhs EXC_OR_ASSIGN expression
	{
		$$ = bnNewBinaryAST(BN_AST_EXC_OR_ASSIGN, $1, $3);
	}
	| lhs LSHIFT_ASSIGN expression
	{
		$$ = bnNewBinaryAST(BN_AST_LSHIFT_ASSIGN, $1, $3);
	}
	| lhs RSHIFT_ASSIGN expression
	{
		$$ = bnNewBinaryAST(BN_AST_RSHIFT_ASSIGN, $1, $3);
	}
	| expression GT expression
	{
		$$ = bnNewBinaryAST(BN_AST_GT, $1, $3);
	}
	| expression GE expression
	{
		$$ = bnNewBinaryAST(BN_AST_GE, $1, $3);
	}
	| expression LT expression
	{
		$$ = bnNewBinaryAST(BN_AST_LT, $1, $3);
	}
	| expression LE expression
	{
		$$ = bnNewBinaryAST(BN_AST_LE, $1, $3);
	}
	| expression LSHIFT expression {
		$$ = bnNewBinaryAST(BN_AST_LSHIFT, $1, $3);
	}
	| expression RSHIFT expression {
		$$ = bnNewBinaryAST(BN_AST_RSHIFT, $1, $3);
	}
	| CHILDA expression
	{
		$$ = bnNewUnaryAST(BN_AST_CHILDA, $2);
	}
	| NOT expression
	{
		$$ = bnNewUnaryAST(BN_AST_NOT, $2);
	}
	| lhs
	| call_expr
	;
call_expr
	: expression LP argument_list RP %prec FUNCCALL
	{
		$$ = bnNewFuncCall($1, $3);
	}
	| expression LP RP %prec FUNCCALL
	{
		$$ = bnNewFuncCall($1, bnNewBlankAST());
	}
	;
lambda_expr
	: DEF LP parameter_list RP LP parameter_list RP comp_stmt
	{
		$$ = bnNewLambdaAST($3, $6, $8);
	}
	| DEF LP RP LP parameter_list RP comp_stmt
	{
		$$ = bnNewLambdaAST(bnNewBlankAST(), $5, $7);
	}
	| DEF LP parameter_list RP LP RP comp_stmt
	{
		$$ = bnNewLambdaAST($3, bnNewBlankAST(), $7);
	}
	| DEF LP RP LP RP comp_stmt
	{
		$$ = bnNewLambdaAST(bnNewBlankAST(), bnNewBlankAST(), $6);
	}
	| DEF LP RP LP DOTDOTDOT RP comp_stmt
	{
		$$ = bnNewLambdaAST(bnNewBlankAST(), bnNewDotDotDotAST(), $7);
	}
	| DEF LP parameter_list RP LP DOTDOTDOT RP comp_stmt
	{
		$$ = bnNewLambdaAST($3, bnNewDotDotDotAST(), $8);
	}
	;
array_lit_expr
	: LSB argument_list RSB
	{
		$$ = bnNewArrayLit($2);
	}
	| LSB RSB
	{
		$$ = bnNewArrayLit(bnNewBlankAST());
	}
	;
lhs
	: IDENT
	{
		$$ = bnNewVariableAST($1);
	}
	| expression DOT IDENT
	{
		$$ = bnNewMemberAccessAST($1, $3);
	}
	| expression LSB expression RSB
	{
		$$ = bnNewArraySubscriptAST($1, $3);
	}
	;
primary
	: INT
	| DOUBLE
	| STRING_LITERAL
	| CHAR_LITERAL
	| lambda_expr
	| array_lit_expr
	;
%%