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
    CHAR_LITERAL = 261,
    IDENT = 262,
    ADD = 263,
    SUB = 264,
    MUL = 265,
    DIV = 266,
    MOD = 267,
    ASSIGN = 268,
    ADD_ASSIGN = 269,
    SUB_ASSIGN = 270,
    MUL_ASSIGN = 271,
    DIV_ASSIGN = 272,
    MOD_ASSIGN = 273,
    AND_ASSIGN = 274,
    OR_ASSIGN = 275,
    LSHIFT_ASSIGN = 276,
    RSHIFT_ASSIGN = 277,
    EXC_OR_ASSIGN = 278,
    EQUAL = 279,
    NOTEQUAL = 280,
    INC = 281,
    DEC = 282,
    GT = 283,
    GE = 284,
    LT = 285,
    LE = 286,
    LSHIFT = 287,
    RSHIFT = 288,
    NOT = 289,
    BIT_AND = 290,
    BIT_OR = 291,
    LOGIC_AND = 292,
    LOGIC_OR = 293,
    LP = 294,
    RP = 295,
    LB = 296,
    RB = 297,
    IF = 298,
    ELSE = 299,
    EXC_OR = 300,
    DOTDOTDOT = 301,
    LSB = 302,
    RSB = 303,
    DOT = 304,
    COMMA = 305,
    SEMICOLON = 306,
    WHILE = 307,
    DEF = 308,
    RETURN_T = 309,
    SCOPE = 310,
    INJECTION = 311,
    PANIC = 312,
    CHILDA = 313,
    NEGATIVE = 314,
    POSITIVE = 315,
    FUNCCALL = 316
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 9 "bone.y" /* yacc.c:1916  */

	bnAST* ast_value;
	bnStringView svvalue;

#line 121 "bone.tab.h" /* yacc.c:1916  */
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
