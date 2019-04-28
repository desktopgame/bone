/* A Bison parser, made by GNU Bison 3.0.5.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_BONE_TAB_H_INCLUDED
# define YY_YY_BONE_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT = 258,
    DOUBLE = 259,
    STRING_LITERAL = 260,
    ADD = 261,
    SUB = 262,
    MUL = 263,
    DIV = 264,
    MOD = 265,
    ASSIGN = 266,
    ADD_ASSIGN = 267,
    SUB_ASSIGN = 268,
    MUL_ASSIGN = 269,
    DIV_ASSIGN = 270,
    MOD_ASSIGN = 271,
    AND_ASSIGN = 272,
    OR_ASSIGN = 273,
    LSHIFT_ASSIGN = 274,
    RSHIFT_ASSIGN = 275,
    EXC_OR_ASSIGN = 276,
    EQUAL = 277,
    NOTEQUAL = 278,
    INC = 279,
    DEC = 280,
    GT = 281,
    GE = 282,
    LT = 283,
    LE = 284,
    LSHIFT = 285,
    RSHIFT = 286,
    NOT = 287,
    BIT_AND = 288,
    BIT_OR = 289,
    LOGIC_AND = 290,
    LOGIC_OR = 291,
    LP = 292,
    RP = 293,
    EXC_OR = 294,
    IDENT = 295,
    NEGATIVE = 296,
    POSITIVE = 297,
    CHILDA = 298
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 8 "bone.y" /* yacc.c:1916  */

	bnAST* ast_value;
	char* svalue;

#line 103 "bone.tab.h" /* yacc.c:1916  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_BONE_TAB_H_INCLUDED  */
