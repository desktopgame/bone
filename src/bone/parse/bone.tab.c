﻿#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-function-declaration"
#pragma clang diagnostic ignored "-Wunused-function"
/* A Bison parser, made by GNU Bison 3.0.5.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Copy the first part of user declarations.  */
#line 1 "bone.y" /* yacc.c:339  */

#include <stdio.h>
#include <stdlib.h>
#include "../bone.h"
#include "ast.h"
#define YYDEBUG 1
#define YYERROR_VERBOSE 1

#line 75 "bone.tab.c" /* yacc.c:339  */

#ifndef YY_NULLPTR
#if defined __cplusplus && 201103L <= __cplusplus
#define YY_NULLPTR nullptr
#else
#define YY_NULLPTR 0
#endif
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
#undef YYERROR_VERBOSE
#define YYERROR_VERBOSE 1
#else
#define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "bone.tab.h".  */
#ifndef YY_YY_BONE_TAB_H_INCLUDED
#define YY_YY_BONE_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
#define YYTOKENTYPE
enum yytokentype {
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
        DEFER = 312,
        CONTINUE = 313,
        BREAK = 314,
        CHILDA = 315,
        NEGATIVE = 316,
        POSITIVE = 317,
        FUNCCALL = 318
};
#endif

/* Value type.  */
#if !defined YYSTYPE && !defined YYSTYPE_IS_DECLARED

union YYSTYPE {
#line 9 "bone.y" /* yacc.c:355  */

        bnAST *ast_value;
        bnStringView svvalue;

#line 184 "bone.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
#define YYSTYPE_IS_TRIVIAL 1
#define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if !defined YYLTYPE && !defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE {
        int first_line;
        int first_column;
        int last_line;
        int last_column;
};
#define YYLTYPE_IS_DECLARED 1
#define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse(void);

#endif /* !YY_YY_BONE_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 215 "bone.tab.c" /* yacc.c:358  */

#ifdef short
#undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
#ifdef __SIZE_TYPE__
#define YYSIZE_T __SIZE_TYPE__
#elif defined size_t
#define YYSIZE_T size_t
#elif !defined YYSIZE_T
#include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#define YYSIZE_T size_t
#else
#define YYSIZE_T unsigned int
#endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T)-1)

#ifndef YY_
#if defined YYENABLE_NLS && YYENABLE_NLS
#if ENABLE_NLS
#include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#define YY_(Msgid) dgettext("bison-runtime", Msgid)
#endif
#endif
#ifndef YY_
#define YY_(Msgid) Msgid
#endif
#endif

#ifndef YY_ATTRIBUTE
#if (defined __GNUC__ &&                                           \
     (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__))) || \
    defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#define YY_ATTRIBUTE(Spec) __attribute__(Spec)
#else
#define YY_ATTRIBUTE(Spec) /* empty */
#endif
#endif

#ifndef YY_ATTRIBUTE_PURE
#define YY_ATTRIBUTE_PURE YY_ATTRIBUTE((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
#define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE((__unused__))
#endif

#if !defined _Noreturn && \
    (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
#if defined _MSC_VER && 1200 <= _MSC_VER
#define _Noreturn __declspec(noreturn)
#else
#define _Noreturn YY_ATTRIBUTE((__noreturn__))
#endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if !defined lint || defined __GNUC__
#define YYUSE(E) ((void)(E))
#else
#define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                       \
        _Pragma("GCC diagnostic push")                            \
            _Pragma("GCC diagnostic ignored \"-Wuninitialized\"") \
                _Pragma("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
#define YY_IGNORE_MAYBE_UNINITIALIZED_END _Pragma("GCC diagnostic pop")
#else
#define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
#define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
#define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
#define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if !defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

#ifdef YYSTACK_USE_ALLOCA
#if YYSTACK_USE_ALLOCA
#ifdef __GNUC__
#define YYSTACK_ALLOC __builtin_alloca
#elif defined __BUILTIN_VA_ARG_INCR
#include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#elif defined _AIX
#define YYSTACK_ALLOC __alloca
#elif defined _MSC_VER
#include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#define alloca _alloca
#else
#define YYSTACK_ALLOC alloca
#if !defined _ALLOCA_H && !defined EXIT_SUCCESS
#include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
/* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif
#endif
#endif
#endif
#endif

#ifdef YYSTACK_ALLOC
/* Pacify GCC's 'empty if-body' warning.  */
#define YYSTACK_FREE(Ptr) \
        do { /* empty */  \
                ;         \
        } while (0)
#ifndef YYSTACK_ALLOC_MAXIMUM
/* The OS might guarantee only one guard page at the bottom of the stack,
   and a page size can be as small as 4096 bytes.  So we cannot safely
   invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
   to allow for a few compiler-allocated temporary stack slots.  */
#define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#endif
#else
#define YYSTACK_ALLOC YYMALLOC
#define YYSTACK_FREE YYFREE
#ifndef YYSTACK_ALLOC_MAXIMUM
#define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#endif
#if (defined __cplusplus && !defined EXIT_SUCCESS && \
     !((defined YYMALLOC || defined malloc) &&       \
       (defined YYFREE || defined free)))
#include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif
#endif
#ifndef YYMALLOC
#define YYMALLOC malloc
#if !defined malloc && !defined EXIT_SUCCESS
void *malloc(YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#endif
#endif
#ifndef YYFREE
#define YYFREE free
#if !defined free && !defined EXIT_SUCCESS
void free(void *);      /* INFRINGES ON USER NAME SPACE */
#endif
#endif
#endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */

#if (!defined yyoverflow &&                                \
     (!defined __cplusplus ||                              \
      (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL && \
       defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc {
        yytype_int16 yyss_alloc;
        YYSTYPE yyvs_alloc;
        YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
#define YYSTACK_GAP_MAXIMUM (sizeof(union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
#define YYSTACK_BYTES(N)                                                    \
        ((N) * (sizeof(yytype_int16) + sizeof(YYSTYPE) + sizeof(YYLTYPE)) + \
         2 * YYSTACK_GAP_MAXIMUM)

#define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
#define YYSTACK_RELOCATE(Stack_alloc, Stack)                            \
        do {                                                            \
                YYSIZE_T yynewbytes;                                    \
                YYCOPY(&yyptr->Stack_alloc, Stack, yysize);             \
                Stack = &yyptr->Stack_alloc;                            \
                yynewbytes =                                            \
                    yystacksize * sizeof(*Stack) + YYSTACK_GAP_MAXIMUM; \
                yyptr += yynewbytes / sizeof(*yyptr);                   \
        } while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
#ifndef YYCOPY
#if defined __GNUC__ && 1 < __GNUC__
#define YYCOPY(Dst, Src, Count) \
        __builtin_memcpy(Dst, Src, (Count) * sizeof(*(Src)))
#else
#define YYCOPY(Dst, Src, Count)                                              \
        do {                                                                 \
                YYSIZE_T yyi;                                                \
                for (yyi = 0; yyi < (Count); yyi++) (Dst)[yyi] = (Src)[yyi]; \
        } while (0)
#endif
#endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL 55
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST 805

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS 65
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS 18
/* YYNRULES -- Number of rules.  */
#define YYNRULES 83
/* YYNSTATES -- Number of states.  */
#define YYNSTATES 167

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK 2
#define YYMAXUTOK 318

#define YYTRANSLATE(YYX) \
        ((unsigned int)(YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] = {
    0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  64, 2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,
    2,  2,  2,  2,  2,  2,  2,  2,  2,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
    49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] = {
    0,   62,  62,  67,  73,  77,  83,  87,  93,  97,  104, 107, 113, 117,
    118, 122, 126, 130, 131, 132, 133, 137, 143, 149, 155, 161, 167, 171,
    172, 175, 179, 183, 187, 191, 195, 199, 203, 207, 211, 215, 219, 223,
    227, 231, 235, 239, 243, 247, 251, 255, 259, 263, 267, 271, 275, 279,
    283, 287, 291, 295, 298, 301, 305, 309, 310, 313, 317, 323, 327, 331,
    335, 339, 343, 349, 353, 359, 363, 367, 373, 374, 375, 376, 377, 378};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] = {"$end",
                                      "error",
                                      "$undefined",
                                      "INT",
                                      "DOUBLE",
                                      "STRING_LITERAL",
                                      "CHAR_LITERAL",
                                      "IDENT",
                                      "ADD",
                                      "SUB",
                                      "MUL",
                                      "DIV",
                                      "MOD",
                                      "ASSIGN",
                                      "ADD_ASSIGN",
                                      "SUB_ASSIGN",
                                      "MUL_ASSIGN",
                                      "DIV_ASSIGN",
                                      "MOD_ASSIGN",
                                      "AND_ASSIGN",
                                      "OR_ASSIGN",
                                      "LSHIFT_ASSIGN",
                                      "RSHIFT_ASSIGN",
                                      "EXC_OR_ASSIGN",
                                      "EQUAL",
                                      "NOTEQUAL",
                                      "INC",
                                      "DEC",
                                      "GT",
                                      "GE",
                                      "LT",
                                      "LE",
                                      "LSHIFT",
                                      "RSHIFT",
                                      "NOT",
                                      "BIT_AND",
                                      "BIT_OR",
                                      "LOGIC_AND",
                                      "LOGIC_OR",
                                      "LP",
                                      "RP",
                                      "LB",
                                      "RB",
                                      "IF",
                                      "ELSE",
                                      "EXC_OR",
                                      "DOTDOTDOT",
                                      "LSB",
                                      "RSB",
                                      "DOT",
                                      "COMMA",
                                      "SEMICOLON",
                                      "WHILE",
                                      "DEF",
                                      "RETURN_T",
                                      "SCOPE",
                                      "INJECTION",
                                      "DEFER",
                                      "CONTINUE",
                                      "BREAK",
                                      "CHILDA",
                                      "NEGATIVE",
                                      "POSITIVE",
                                      "FUNCCALL",
                                      "'\\n'",
                                      "$accept",
                                      "program",
                                      "argument_list",
                                      "parameter_list",
                                      "statement_list",
                                      "statement_list_opt",
                                      "statement",
                                      "comp_stmt",
                                      "return_stmt",
                                      "injection_stmt",
                                      "defer_stmt",
                                      "expression",
                                      "expression_nobrace",
                                      "call_expr",
                                      "lambda_expr",
                                      "array_lit_expr",
                                      "lhs",
                                      "primary",
                                      YY_NULLPTR};
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] = {
    0,   256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267,
    268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280,
    281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293,
    294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306,
    307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 10};
#endif

#define YYPACT_NINF -135

#define yypact_value_is_default(Yystate) (!!((Yystate) == (-135)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) 0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] = {
    89,   -45,  -135, -135, -135, -135, -135, 231,  231,  231,  231,  147,
    -12,  164,  -11,  -5,   -4,   -16,  147,  -2,   34,   231,  39,   147,
    -135, -135, -135, -135, -135, 277,  -135, -135, -135, -135, 782,  -135,
    -135, -31,  -31,  -31,  326,  147,  1,    231,  -135, -17,  494,  231,
    5,    -135, 231,  -135, -135, -135, -31,  -135, -135, 231,  231,  231,
    231,  231,  231,  231,  231,  231,  231,  231,  231,  231,  231,  231,
    231,  231,  222,  231,  231,  43,   -135, 231,  231,  231,  231,  231,
    231,  231,  231,  231,  231,  231,  231,  -135, -135, 368,  -135, 231,
    410,  -135, 47,   -26,  494,  36,   -1,   -1,   -31,  -31,  -31,  727,
    727,  210,  210,  210,  210,  738,  738,  695,  620,  578,  536,  -135,
    -15,  662,  452,  -135, -1,   494,  494,  494,  494,  494,  494,  494,
    494,  494,  494,  494,  147,  494,  147,  64,   49,   84,   -135, -135,
    -135, 55,   -135, 59,   61,   -14,  65,   -135, 147,  -135, 59,   59,
    59,   62,   -8,   -135, -135, -135, -135, 59,   59,   -135, -135};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] = {
    0,  0,  78, 79, 80, 81, 75, 0,  0,  0,  0,  10, 0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  2,  8,  13, 17, 18, 19, 0,  28, 64, 82, 83, 63, 27, 3,  29,
    30, 62, 0,  11, 0,  0,  74, 0,  4,  0,  0,  23, 0,  25, 20, 21, 61, 1,  9,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  12, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  26, 22, 0,  73,
    0,  0,  6,  0,  0,  0,  64, 32, 33, 34, 35, 36, 38, 39, 55, 56, 57, 58, 59,
    60, 40, 37, 43, 42, 66, 0,  41, 0,  76, 31, 44, 45, 46, 47, 48, 49, 50, 51,
    53, 54, 52, 0,  5,  0,  0,  0,  0,  24, 65, 77, 14, 16, 0,  0,  0,  0,  7,
    0,  70, 0,  0,  0,  0,  0,  15, 71, 68, 69, 0,  0,  72, 67};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] = {-135, -135, 29,   -133, 95,   -135,
                                       -18,  -134, -135, -135, -135, -6,
                                       -135, 57,   -135, -135, -135, -135};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] = {-1, 22, 45, 99, 23, 42, 24, 25, 26,
                                        27, 28, 29, 30, 31, 32, 33, 34, 35};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] = {
    51,  37,  38,  39,  40,  56,  149, 46,  74,  59,  60,  61,  97,  153, 140,
    54,  76,  158, 77,  36,  160, 161, 162, 56,  141, 143, 155, 43,  47,  165,
    166, 94,  164, 95,  48,  95,  141, 93,  74,  55,  50,  96,  141, 92,  100,
    98,  76,  49,  77,  52,  123, 102, 103, 104, 105, 106, 107, 108, 109, 110,
    111, 112, 113, 114, 115, 116, 117, 118, 46,  121, 122, 97,  97,  124, 125,
    126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 53,  139, 142, 150, 137,
    1,   151, 2,   3,   4,   5,   6,   7,   8,   152, 11,  154, 163, 120, 147,
    156, 41,  101, 0,   0,   148, 157, 0,   0,   0,   0,   0,   0,   145, 0,
    146, 0,   0,   9,   0,   0,   0,   0,   10,  0,   11,  0,   12,  0,   159,
    0,   13,  0,   0,   0,   0,   14,  15,  16,  17,  0,   18,  19,  20,  21,
    2,   3,   4,   5,   6,   7,   8,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   2,   3,   4,   5,   6,   7,   8,   0,   0,   0,   0,   0,   0,
    0,   9,   0,   0,   0,   0,   10,  0,   11,  0,   12,  0,   0,   0,   13,
    0,   0,   0,   9,   14,  15,  16,  17,  10,  18,  19,  20,  21,  0,   0,
    0,   13,  44,  0,   0,   0,   0,   15,  57,  58,  59,  60,  61,  0,   21,
    2,   3,   4,   5,   6,   7,   8,   0,   0,   2,   3,   4,   5,   6,   7,
    8,   0,   68,  69,  0,   0,   0,   0,   0,   74,  0,   0,   0,   0,   0,
    0,   9,   76,  0,   77,  0,   10,  119, 0,   0,   9,   79,  0,   0,   13,
    10,  0,   0,   0,   0,   15,  0,   0,   13,  0,   0,   0,   21,  0,   15,
    57,  58,  59,  60,  61,  0,   21,  0,   0,   0,   0,   0,   0,   0,   0,
    0,   62,  63,  0,   0,   64,  65,  66,  67,  68,  69,  0,   70,  71,  72,
    73,  74,  0,   0,   0,   0,   0,   75,  0,   76,  0,   77,  0,   78,  0,
    0,   0,   0,   79,  57,  58,  59,  60,  61,  0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   62,  63,  0,   0,   64,  65,  66,  67,  68,  69,
    0,   70,  71,  72,  73,  74,  91,  0,   0,   0,   0,   75,  0,   76,  0,
    77,  57,  58,  59,  60,  61,  0,   79,  0,   0,   0,   0,   0,   0,   0,
    0,   0,   62,  63,  0,   0,   64,  65,  66,  67,  68,  69,  0,   70,  71,
    72,  73,  74,  136, 0,   0,   0,   0,   75,  0,   76,  0,   77,  57,  58,
    59,  60,  61,  0,   79,  0,   0,   0,   0,   0,   0,   0,   0,   0,   62,
    63,  0,   0,   64,  65,  66,  67,  68,  69,  0,   70,  71,  72,  73,  74,
    138, 0,   0,   0,   0,   75,  0,   76,  0,   77,  57,  58,  59,  60,  61,
    0,   79,  0,   0,   0,   0,   0,   0,   0,   0,   0,   62,  63,  0,   0,
    64,  65,  66,  67,  68,  69,  0,   70,  71,  72,  73,  74,  0,   0,   0,
    0,   0,   75,  0,   76,  144, 77,  57,  58,  59,  60,  61,  0,   79,  0,
    0,   0,   0,   0,   0,   0,   0,   0,   62,  63,  0,   0,   64,  65,  66,
    67,  68,  69,  0,   70,  71,  72,  73,  74,  0,   0,   0,   0,   0,   75,
    0,   76,  0,   77,  57,  58,  59,  60,  61,  0,   79,  0,   0,   0,   0,
    0,   0,   0,   0,   0,   62,  63,  0,   0,   64,  65,  66,  67,  68,  69,
    0,   70,  71,  72,  0,   74,  0,   0,   0,   0,   0,   75,  0,   76,  0,
    77,  57,  58,  59,  60,  61,  0,   79,  0,   0,   0,   0,   0,   0,   0,
    0,   0,   62,  63,  0,   0,   64,  65,  66,  67,  68,  69,  0,   70,  71,
    0,   0,   74,  0,   0,   0,   0,   0,   75,  0,   76,  0,   77,  57,  58,
    59,  60,  61,  0,   79,  0,   0,   0,   0,   0,   0,   0,   0,   0,   62,
    63,  0,   0,   64,  65,  66,  67,  68,  69,  0,   70,  0,   0,   0,   74,
    0,   0,   0,   0,   0,   75,  0,   76,  0,   77,  57,  58,  59,  60,  61,
    0,   79,  0,   0,   0,   0,   0,   0,   0,   0,   0,   62,  63,  0,   0,
    64,  65,  66,  67,  68,  69,  0,   70,  0,   0,   0,   74,  0,   57,  58,
    59,  60,  61,  0,   76,  0,   77,  0,   0,   0,   0,   0,   0,   79,  62,
    63,  0,   0,   64,  65,  66,  67,  68,  69,  0,   0,   0,   0,   0,   74,
    57,  58,  59,  60,  61,  0,   0,   76,  0,   77,  0,   57,  58,  59,  60,
    61,  79,  0,   0,   0,   64,  65,  66,  67,  68,  69,  0,   0,   0,   0,
    0,   74,  0,   0,   0,   0,   0,   0,   0,   76,  0,   77,  74,  0,   0,
    0,   0,   0,   79,  0,   76,  0,   77,  0,   0,   0,   0,   0,   0,   79,
    80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90};

static const yytype_int16 yycheck[] = {
    18, 7,   8,  9,  10,  23,  139, 13, 39,  10, 11, 12, 7,  147, 40,  21,
    47, 150, 49, 64, 154, 155, 156, 41, 50,  40, 40, 39, 39, 163, 164, 48,
    40, 50,  39, 50, 50,  43,  39,  0,  56,  47, 50, 42, 50, 40,  47,  51,
    49, 51,  7,  57, 58,  59,  60,  61, 62,  63, 64, 65, 66, 67,  68,  69,
    70, 71,  72, 73, 74,  75,  76,  7,  7,   79, 80, 81, 82, 83,  84,  85,
    86, 87,  88, 89, 90,  51,  39,  51, 39,  95, 1,  7,  3,  4,   5,   6,
    7,  8,   9,  44, 41,  40,  40,  74, 40,  40, 11, 50, -1, -1,  46,  46,
    -1, -1,  -1, -1, -1,  -1,  136, -1, 138, -1, -1, 34, -1, -1,  -1,  -1,
    39, -1,  41, -1, 43,  -1,  152, -1, 47,  -1, -1, -1, -1, 52,  53,  54,
    55, -1,  57, 58, 59,  60,  3,   4,  5,   6,  7,  8,  9,  -1,  -1,  -1,
    -1, -1,  -1, -1, -1,  -1,  -1,  3,  4,   5,  6,  7,  8,  9,   -1,  -1,
    -1, -1,  -1, -1, -1,  34,  -1,  -1, -1,  -1, 39, -1, 41, -1,  43,  -1,
    -1, -1,  47, -1, -1,  -1,  34,  52, 53,  54, 55, 39, 57, 58,  59,  60,
    -1, -1,  -1, 47, 48,  -1,  -1,  -1, -1,  53, 8,  9,  10, 11,  12,  -1,
    60, 3,   4,  5,  6,   7,   8,   9,  -1,  -1, 3,  4,  5,  6,   7,   8,
    9,  -1,  32, 33, -1,  -1,  -1,  -1, -1,  39, -1, -1, -1, -1,  -1,  -1,
    34, 47,  -1, 49, -1,  39,  40,  -1, -1,  34, 56, -1, -1, 47,  39,  -1,
    -1, -1,  -1, 53, -1,  -1,  47,  -1, -1,  -1, 60, -1, 53, 8,   9,   10,
    11, 12,  -1, 60, -1,  -1,  -1,  -1, -1,  -1, -1, -1, -1, 24,  25,  -1,
    -1, 28,  29, 30, 31,  32,  33,  -1, 35,  36, 37, 38, 39, -1,  -1,  -1,
    -1, -1,  45, -1, 47,  -1,  49,  -1, 51,  -1, -1, -1, -1, 56,  8,   9,
    10, 11,  12, -1, -1,  -1,  -1,  -1, -1,  -1, -1, -1, -1, -1,  24,  25,
    -1, -1,  28, 29, 30,  31,  32,  33, -1,  35, 36, 37, 38, 39,  40,  -1,
    -1, -1,  -1, 45, -1,  47,  -1,  49, 8,   9,  10, 11, 12, -1,  56,  -1,
    -1, -1,  -1, -1, -1,  -1,  -1,  -1, 24,  25, -1, -1, 28, 29,  30,  31,
    32, 33,  -1, 35, 36,  37,  38,  39, 40,  -1, -1, -1, -1, 45,  -1,  47,
    -1, 49,  8,  9,  10,  11,  12,  -1, 56,  -1, -1, -1, -1, -1,  -1,  -1,
    -1, -1,  24, 25, -1,  -1,  28,  29, 30,  31, 32, 33, -1, 35,  36,  37,
    38, 39,  40, -1, -1,  -1,  -1,  45, -1,  47, -1, 49, 8,  9,   10,  11,
    12, -1,  56, -1, -1,  -1,  -1,  -1, -1,  -1, -1, -1, 24, 25,  -1,  -1,
    28, 29,  30, 31, 32,  33,  -1,  35, 36,  37, 38, 39, -1, -1,  -1,  -1,
    -1, 45,  -1, 47, 48,  49,  8,   9,  10,  11, 12, -1, 56, -1,  -1,  -1,
    -1, -1,  -1, -1, -1,  -1,  24,  25, -1,  -1, 28, 29, 30, 31,  32,  33,
    -1, 35,  36, 37, 38,  39,  -1,  -1, -1,  -1, -1, 45, -1, 47,  -1,  49,
    8,  9,   10, 11, 12,  -1,  56,  -1, -1,  -1, -1, -1, -1, -1,  -1,  -1,
    24, 25,  -1, -1, 28,  29,  30,  31, 32,  33, -1, 35, 36, 37,  -1,  39,
    -1, -1,  -1, -1, -1,  45,  -1,  47, -1,  49, 8,  9,  10, 11,  12,  -1,
    56, -1,  -1, -1, -1,  -1,  -1,  -1, -1,  -1, 24, 25, -1, -1,  28,  29,
    30, 31,  32, 33, -1,  35,  36,  -1, -1,  39, -1, -1, -1, -1,  -1,  45,
    -1, 47,  -1, 49, 8,   9,   10,  11, 12,  -1, 56, -1, -1, -1,  -1,  -1,
    -1, -1,  -1, -1, 24,  25,  -1,  -1, 28,  29, 30, 31, 32, 33,  -1,  35,
    -1, -1,  -1, 39, -1,  -1,  -1,  -1, -1,  45, -1, 47, -1, 49,  8,   9,
    10, 11,  12, -1, 56,  -1,  -1,  -1, -1,  -1, -1, -1, -1, -1,  24,  25,
    -1, -1,  28, 29, 30,  31,  32,  33, -1,  35, -1, -1, -1, 39,  -1,  8,
    9,  10,  11, 12, -1,  47,  -1,  49, -1,  -1, -1, -1, -1, -1,  56,  24,
    25, -1,  -1, 28, 29,  30,  31,  32, 33,  -1, -1, -1, -1, -1,  39,  8,
    9,  10,  11, 12, -1,  -1,  47,  -1, 49,  -1, 8,  9,  10, 11,  12,  56,
    -1, -1,  -1, 28, 29,  30,  31,  32, 33,  -1, -1, -1, -1, -1,  39,  -1,
    -1, -1,  -1, -1, -1,  -1,  47,  -1, 49,  39, -1, -1, -1, -1,  -1,  56,
    -1, 47,  -1, 49, -1,  -1,  -1,  -1, -1,  -1, 56, 13, 14, 15,  16,  17,
    18, 19,  20, 21, 22,  23};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] = {
    0,  1,  3,  4,  5,  6,  7,  8,  9,  34, 39, 41, 43, 47, 52, 53, 54, 55, 57,
    58, 59, 60, 66, 69, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 64, 76,
    76, 76, 76, 69, 70, 39, 48, 67, 76, 39, 39, 51, 56, 71, 51, 51, 76, 0,  71,
    8,  9,  10, 11, 12, 24, 25, 28, 29, 30, 31, 32, 33, 35, 36, 37, 38, 39, 45,
    47, 49, 51, 56, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 40, 42, 76, 48,
    50, 76, 7,  40, 68, 76, 78, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
    76, 76, 76, 76, 76, 40, 67, 76, 76, 7,  76, 76, 76, 76, 76, 76, 76, 76, 76,
    76, 76, 76, 40, 76, 40, 39, 40, 50, 51, 40, 48, 71, 71, 40, 46, 68, 39, 7,
    44, 72, 40, 40, 40, 46, 68, 71, 72, 72, 72, 40, 40, 72, 72};

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] = {
    0,  65, 66, 66, 67, 67, 68, 68, 69, 69, 70, 70, 71, 71, 71, 71, 71,
    71, 71, 71, 71, 71, 72, 73, 74, 75, 76, 76, 76, 77, 77, 77, 77, 77,
    77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
    77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 78, 78, 79,
    79, 79, 79, 79, 79, 80, 80, 81, 81, 81, 82, 82, 82, 82, 82, 82};

/* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] = {
    0, 2, 1, 2, 1, 3, 1, 3, 1, 2, 0, 1, 2, 1, 5, 7, 5, 1, 1, 1, 2,
    2, 3, 2, 4, 2, 3, 1, 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2,
    1, 1, 4, 3, 8, 7, 7, 6, 7, 8, 3, 2, 1, 3, 4, 1, 1, 1, 1, 1, 1};

#define yyerrok (yyerrstatus = 0)
#define yyclearin (yychar = YYEMPTY)
#define YYEMPTY (-2)
#define YYEOF 0

#define YYACCEPT goto yyacceptlab
#define YYABORT goto yyabortlab
#define YYERROR goto yyerrorlab

#define YYRECOVERING() (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                        \
        do                                                            \
                if (yychar == YYEMPTY) {                              \
                        yychar = (Token);                             \
                        yylval = (Value);                             \
                        YYPOPSTACK(yylen);                            \
                        yystate = *yyssp;                             \
                        goto yybackup;                                \
                } else {                                              \
                        yyerror(YY_("syntax error: cannot back up")); \
                        YYERROR;                                      \
                }                                                     \
        while (0)

/* Error token number */
#define YYTERROR 1
#define YYERRCODE 256

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(Current, Rhs, N)                                       \
        do                                                                    \
                if (N) {                                                      \
                        (Current).first_line = YYRHSLOC(Rhs, 1).first_line;   \
                        (Current).first_column =                              \
                            YYRHSLOC(Rhs, 1).first_column;                    \
                        (Current).last_line = YYRHSLOC(Rhs, N).last_line;     \
                        (Current).last_column = YYRHSLOC(Rhs, N).last_column; \
                } else {                                                      \
                        (Current).first_line = (Current).last_line =          \
                            YYRHSLOC(Rhs, 0).last_line;                       \
                        (Current).first_column = (Current).last_column =      \
                            YYRHSLOC(Rhs, 0).last_column;                     \
                }                                                             \
        while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])

/* Enable debugging if requested.  */
#if YYDEBUG

#ifndef YYFPRINTF
#include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#define YYFPRINTF fprintf
#endif

#define YYDPRINTF(Args)                      \
        do {                                 \
                if (yydebug) YYFPRINTF Args; \
        } while (0)

/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
#if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned yy_location_print_(FILE *yyo, YYLTYPE const *const yylocp) {
        unsigned res = 0;
        int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
        if (0 <= yylocp->first_line) {
                res += YYFPRINTF(yyo, "%d", yylocp->first_line);
                if (0 <= yylocp->first_column)
                        res += YYFPRINTF(yyo, ".%d", yylocp->first_column);
        }
        if (0 <= yylocp->last_line) {
                if (yylocp->first_line < yylocp->last_line) {
                        res += YYFPRINTF(yyo, "-%d", yylocp->last_line);
                        if (0 <= end_col) res += YYFPRINTF(yyo, ".%d", end_col);
                } else if (0 <= end_col && yylocp->first_column < end_col)
                        res += YYFPRINTF(yyo, "-%d", end_col);
        }
        return res;
}

#define YY_LOCATION_PRINT(File, Loc) yy_location_print_(File, &(Loc))

#else
#define YY_LOCATION_PRINT(File, Loc) ((void)0)
#endif
#endif

#define YY_SYMBOL_PRINT(Title, Type, Value, Location)                   \
        do {                                                            \
                if (yydebug) {                                          \
                        YYFPRINTF(stderr, "%s ", Title);                \
                        yy_symbol_print(stderr, Type, Value, Location); \
                        YYFPRINTF(stderr, "\n");                        \
                }                                                       \
        } while (0)

/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void yy_symbol_value_print(FILE *yyoutput, int yytype,
                                  YYSTYPE const *const yyvaluep,
                                  YYLTYPE const *const yylocationp) {
        FILE *yyo = yyoutput;
        YYUSE(yyo);
        YYUSE(yylocationp);
        if (!yyvaluep) return;
#ifdef YYPRINT
        if (yytype < YYNTOKENS) YYPRINT(yyoutput, yytoknum[yytype], *yyvaluep);
#endif
        YYUSE(yytype);
}

/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void yy_symbol_print(FILE *yyoutput, int yytype,
                            YYSTYPE const *const yyvaluep,
                            YYLTYPE const *const yylocationp) {
        YYFPRINTF(yyoutput, "%s %s (", yytype < YYNTOKENS ? "token" : "nterm",
                  yytname[yytype]);

        YY_LOCATION_PRINT(yyoutput, *yylocationp);
        YYFPRINTF(yyoutput, ": ");
        yy_symbol_value_print(yyoutput, yytype, yyvaluep, yylocationp);
        YYFPRINTF(yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void yy_stack_print(yytype_int16 *yybottom, yytype_int16 *yytop) {
        YYFPRINTF(stderr, "Stack now");
        for (; yybottom <= yytop; yybottom++) {
                int yybot = *yybottom;
                YYFPRINTF(stderr, " %d", yybot);
        }
        YYFPRINTF(stderr, "\n");
}

#define YY_STACK_PRINT(Bottom, Top)                           \
        do {                                                  \
                if (yydebug) yy_stack_print((Bottom), (Top)); \
        } while (0)

/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void yy_reduce_print(yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                            int yyrule) {
        unsigned long int yylno = yyrline[yyrule];
        int yynrhs = yyr2[yyrule];
        int yyi;
        YYFPRINTF(stderr, "Reducing stack by rule %d (line %lu):\n", yyrule - 1,
                  yylno);
        /* The symbols being reduced.  */
        for (yyi = 0; yyi < yynrhs; yyi++) {
                YYFPRINTF(stderr, "   $%d = ", yyi + 1);
                yy_symbol_print(stderr, yystos[yyssp[yyi + 1 - yynrhs]],
                                &(yyvsp[(yyi + 1) - (yynrhs)]),
                                &(yylsp[(yyi + 1) - (yynrhs)]));
                YYFPRINTF(stderr, "\n");
        }
}

#define YY_REDUCE_PRINT(Rule)                                            \
        do {                                                             \
                if (yydebug) yy_reduce_print(yyssp, yyvsp, yylsp, Rule); \
        } while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
#define YYDPRINTF(Args)
#define YY_SYMBOL_PRINT(Title, Type, Value, Location)
#define YY_STACK_PRINT(Bottom, Top)
#define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
#define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

#if YYERROR_VERBOSE

#ifndef yystrlen
#if defined __GLIBC__ && defined _STRING_H
#define yystrlen strlen
#else
/* Return the length of YYSTR.  */
static YYSIZE_T yystrlen(const char *yystr) {
        YYSIZE_T yylen;
        for (yylen = 0; yystr[yylen]; yylen++) continue;
        return yylen;
}
#endif
#endif

#ifndef yystpcpy
#if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#define yystpcpy stpcpy
#else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *yystpcpy(char *yydest, const char *yysrc) {
        char *yyd = yydest;
        const char *yys = yysrc;

        while ((*yyd++ = *yys++) != '\0') continue;

        return yyd - 1;
}
#endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T yytnamerr(char *yyres, const char *yystr) {
        if (*yystr == '"') {
                YYSIZE_T yyn = 0;
                char const *yyp = yystr;

                for (;;) switch (*++yyp) {
                                case '\'':
                                case ',':
                                        goto do_not_strip_quotes;

                                case '\\':
                                        if (*++yyp != '\\')
                                                goto do_not_strip_quotes;
                                        /* Fall through.  */
                                default:
                                        if (yyres) yyres[yyn] = *yyp;
                                        yyn++;
                                        break;

                                case '"':
                                        if (yyres) yyres[yyn] = '\0';
                                        return yyn;
                        }
        do_not_strip_quotes:;
        }

        if (!yyres) return yystrlen(yystr);

        return yystpcpy(yyres, yystr) - yyres;
}
#endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int yysyntax_error(YYSIZE_T *yymsg_alloc, char **yymsg,
                          yytype_int16 *yyssp, int yytoken) {
        YYSIZE_T yysize0 = yytnamerr(YY_NULLPTR, yytname[yytoken]);
        YYSIZE_T yysize = yysize0;
        enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
        /* Internationalized format string. */
        const char *yyformat = YY_NULLPTR;
        /* Arguments of yyformat. */
        char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
        /* Number of reported tokens (one for the "unexpected", one per
           "expected"). */
        int yycount = 0;

        /* There are many possibilities here to consider:
           - If this state is a consistent state with a default action, then
             the only way this function was invoked is if the default action
             is an error action.  In that case, don't check for expected
             tokens because there are none.
           - The only way there can be no lookahead present (in yychar) is if
             this state is a consistent state with a default action.  Thus,
             detecting the absence of a lookahead is sufficient to determine
             that there is no unexpected or expected token to report.  In that
             case, just report a simple "syntax error".
           - Don't assume there isn't a lookahead just because this state is a
             consistent state with a default action.  There might have been a
             previous inconsistent state, consistent state with a non-default
             action, or user semantic action that manipulated yychar.
           - Of course, the expected token list depends on states to have
             correct lookahead information, and it depends on the parser not
             to perform extra reductions after fetching a lookahead from the
             scanner and before detecting a syntax error.  Thus, state merging
             (from LALR or IELR) and default reductions corrupt the expected
             token list.  However, the list is correct for canonical LR with
             one exception: it will still contain any token that will not be
             accepted due to an error action in a later state.
        */
        if (yytoken != YYEMPTY) {
                int yyn = yypact[*yyssp];
                yyarg[yycount++] = yytname[yytoken];
                if (!yypact_value_is_default(yyn)) {
                        /* Start YYX at -YYN if negative to avoid negative
                           indexes in YYCHECK.  In other words, skip the first
                           -YYN actions for
                           this state because they are default actions.  */
                        int yyxbegin = yyn < 0 ? -yyn : 0;
                        /* Stay within bounds of both yycheck and yytname.  */
                        int yychecklim = YYLAST - yyn + 1;
                        int yyxend =
                            yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
                        int yyx;

                        for (yyx = yyxbegin; yyx < yyxend; ++yyx)
                                if (yycheck[yyx + yyn] == yyx &&
                                    yyx != YYTERROR &&
                                    !yytable_value_is_error(
                                        yytable[yyx + yyn])) {
                                        if (yycount ==
                                            YYERROR_VERBOSE_ARGS_MAXIMUM) {
                                                yycount = 1;
                                                yysize = yysize0;
                                                break;
                                        }
                                        yyarg[yycount++] = yytname[yyx];
                                        {
                                                YYSIZE_T yysize1 =
                                                    yysize +
                                                    yytnamerr(YY_NULLPTR,
                                                              yytname[yyx]);
                                                if (!(yysize <= yysize1 &&
                                                      yysize1 <=
                                                          YYSTACK_ALLOC_MAXIMUM))
                                                        return 2;
                                                yysize = yysize1;
                                        }
                                }
                }
        }

        switch (yycount) {
#define YYCASE_(N, S)         \
        case N:               \
                yyformat = S; \
                break
                default: /* Avoid compiler warnings. */
                        YYCASE_(0, YY_("syntax error"));
                        YYCASE_(1, YY_("syntax error, unexpected %s"));
                        YYCASE_(
                            2,
                            YY_("syntax error, unexpected %s, expecting %s"));
                        YYCASE_(3, YY_("syntax error, unexpected %s, expecting "
                                       "%s or %s"));
                        YYCASE_(4, YY_("syntax error, unexpected %s, expecting "
                                       "%s or %s or %s"));
                        YYCASE_(5, YY_("syntax error, unexpected %s, expecting "
                                       "%s or %s or %s or %s"));
#undef YYCASE_
        }

        {
                YYSIZE_T yysize1 = yysize + yystrlen(yyformat);
                if (!(yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                        return 2;
                yysize = yysize1;
        }

        if (*yymsg_alloc < yysize) {
                *yymsg_alloc = 2 * yysize;
                if (!(yysize <= *yymsg_alloc &&
                      *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
                        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
                return 1;
        }

        /* Avoid sprintf, as that infringes on the user's name space.
           Don't have undefined behavior even if the translation
           produced a string with the wrong number of "%s"s.  */
        {
                char *yyp = *yymsg;
                int yyi = 0;
                while ((*yyp = *yyformat) != '\0')
                        if (*yyp == '%' && yyformat[1] == 's' &&
                            yyi < yycount) {
                                yyp += yytnamerr(yyp, yyarg[yyi++]);
                                yyformat += 2;
                        } else {
                                yyp++;
                                yyformat++;
                        }
        }
        return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void yydestruct(const char *yymsg, int yytype, YYSTYPE *yyvaluep,
                       YYLTYPE *yylocationp) {
        YYUSE(yyvaluep);
        YYUSE(yylocationp);
        if (!yymsg) yymsg = "Deleting";
        YY_SYMBOL_PRINT(yymsg, yytype, yyvaluep, yylocationp);

        YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
        YYUSE(yytype);
        YY_IGNORE_MAYBE_UNINITIALIZED_END
}

/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
#if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
    = {1, 1, 1, 1}
#endif
;
/* Number of syntax errors so far.  */
int yynerrs;

/*----------.
| yyparse.  |
`----------*/

int yyparse(void) {
        int yystate;
        /* Number of tokens to shift before error messages enabled.  */
        int yyerrstatus;

        /* The stacks and their tools:
           'yyss': related to states.
           'yyvs': related to semantic values.
           'yyls': related to locations.

           Refer to the stacks through separate pointers, to allow yyoverflow
           to reallocate them elsewhere.  */

        /* The state stack.  */
        yytype_int16 yyssa[YYINITDEPTH];
        yytype_int16 *yyss;
        yytype_int16 *yyssp;

        /* The semantic value stack.  */
        YYSTYPE yyvsa[YYINITDEPTH];
        YYSTYPE *yyvs;
        YYSTYPE *yyvsp;

        /* The location stack.  */
        YYLTYPE yylsa[YYINITDEPTH];
        YYLTYPE *yyls;
        YYLTYPE *yylsp;

        /* The locations where the error started and ended.  */
        YYLTYPE yyerror_range[3];

        YYSIZE_T yystacksize;

        int yyn;
        int yyresult;
        /* Lookahead token as an internal (translated) token number.  */
        int yytoken = 0;
        /* The variables used to return semantic value and location from the
           action routines.  */
        YYSTYPE yyval;
        YYLTYPE yyloc;

#if YYERROR_VERBOSE
        /* Buffer for error messages, and its allocated size.  */
        char yymsgbuf[128];
        char *yymsg = yymsgbuf;
        YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N) (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

        /* The number of symbols on the RHS of the reduced rule.
           Keep to zero when no symbol should be popped.  */
        int yylen = 0;

        yyssp = yyss = yyssa;
        yyvsp = yyvs = yyvsa;
        yylsp = yyls = yylsa;
        yystacksize = YYINITDEPTH;

        YYDPRINTF((stderr, "Starting parse\n"));

        yystate = 0;
        yyerrstatus = 0;
        yynerrs = 0;
        yychar = YYEMPTY; /* Cause a token to be read.  */
        yylsp[0] = yylloc;
        goto yysetstate;

        /*------------------------------------------------------------.
        | yynewstate -- Push a new state, which is found in yystate.  |
        `------------------------------------------------------------*/
yynewstate:
        /* In all cases, when you get here, the value and location stacks
           have just been pushed.  So pushing a state here evens the stacks.  */
        yyssp++;

yysetstate:
        *yyssp = yystate;

        if (yyss + yystacksize - 1 <= yyssp) {
                /* Get the current used size of the three stacks, in elements.
                 */
                YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
                {
                        /* Give user a chance to reallocate the stack.  Use
                           copies of these so that the &'s don't force the real
                           ones into memory.  */
                        YYSTYPE *yyvs1 = yyvs;
                        yytype_int16 *yyss1 = yyss;
                        YYLTYPE *yyls1 = yyls;

                        /* Each stack pointer address is followed by the size of
                           the data in use in that stack, in bytes.  This used
                           to be a conditional around just the two extra args,
                           but that might
                           be undefined if yyoverflow is a macro.  */
                        yyoverflow(YY_("memory exhausted"), &yyss1,
                                   yysize * sizeof(*yyssp), &yyvs1,
                                   yysize * sizeof(*yyvsp), &yyls1,
                                   yysize * sizeof(*yylsp), &yystacksize);

                        yyls = yyls1;
                        yyss = yyss1;
                        yyvs = yyvs1;
                }
#else /* no yyoverflow */
#ifndef YYSTACK_RELOCATE
                goto yyexhaustedlab;
#else
                /* Extend the stack our own way.  */
                if (YYMAXDEPTH <= yystacksize) goto yyexhaustedlab;
                yystacksize *= 2;
                if (YYMAXDEPTH < yystacksize) yystacksize = YYMAXDEPTH;

                {
                        yytype_int16 *yyss1 = yyss;
                        union yyalloc *yyptr = (union yyalloc *)YYSTACK_ALLOC(
                            YYSTACK_BYTES(yystacksize));
                        if (!yyptr) goto yyexhaustedlab;
                        YYSTACK_RELOCATE(yyss_alloc, yyss);
                        YYSTACK_RELOCATE(yyvs_alloc, yyvs);
                        YYSTACK_RELOCATE(yyls_alloc, yyls);
#undef YYSTACK_RELOCATE
                        if (yyss1 != yyssa) YYSTACK_FREE(yyss1);
                }
#endif
#endif /* no yyoverflow */

                yyssp = yyss + yysize - 1;
                yyvsp = yyvs + yysize - 1;
                yylsp = yyls + yysize - 1;

                YYDPRINTF((stderr, "Stack size increased to %lu\n",
                           (unsigned long int)yystacksize));

                if (yyss + yystacksize - 1 <= yyssp) YYABORT;
        }

        YYDPRINTF((stderr, "Entering state %d\n", yystate));

        if (yystate == YYFINAL) YYACCEPT;

        goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

        /* Do appropriate processing given the current state.  Read a
           lookahead token if we need one and don't already have one.  */

        /* First try to decide what to do without reference to lookahead token.
         */
        yyn = yypact[yystate];
        if (yypact_value_is_default(yyn)) goto yydefault;

        /* Not known => get a lookahead token if don't already have one.  */

        /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
        if (yychar == YYEMPTY) {
                YYDPRINTF((stderr, "Reading a token: "));
                yychar = yylex();
        }

        if (yychar <= YYEOF) {
                yychar = yytoken = YYEOF;
                YYDPRINTF((stderr, "Now at end of input.\n"));
        } else {
                yytoken = YYTRANSLATE(yychar);
                YY_SYMBOL_PRINT("Next token is", yytoken, &yylval, &yylloc);
        }

        /* If the proper action on seeing token YYTOKEN is to reduce or to
           detect an error, take that action.  */
        yyn += yytoken;
        if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken) goto yydefault;
        yyn = yytable[yyn];
        if (yyn <= 0) {
                if (yytable_value_is_error(yyn)) goto yyerrlab;
                yyn = -yyn;
                goto yyreduce;
        }

        /* Count tokens shifted since error; after three, turn off error
           status.  */
        if (yyerrstatus) yyerrstatus--;

        /* Shift the lookahead token.  */
        YY_SYMBOL_PRINT("Shifting", yytoken, &yylval, &yylloc);

        /* Discard the shifted token.  */
        yychar = YYEMPTY;

        yystate = yyn;
        YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
        *++yyvsp = yylval;
        YY_IGNORE_MAYBE_UNINITIALIZED_END
        *++yylsp = yylloc;
        goto yynewstate;

/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
        yyn = yydefact[yystate];
        if (yyn == 0) goto yyerrlab;
        goto yyreduce;

/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
        /* yyn is the number of a rule to reduce with.  */
        yylen = yyr2[yyn];

        /* If YYLEN is nonzero, implement the default value of the action:
           '$$ = $1'.

           Otherwise, the following line sets YYVAL to garbage.
           This behavior is undocumented and Bison
           users should not rely upon it.  Assigning to YYVAL
           unconditionally makes the parser a bit smaller, and it avoids a
           GCC warning that YYVAL may be used uninitialized.  */
        yyval = yyvsp[1 - yylen];

        /* Default location. */
        YYLLOC_DEFAULT(yyloc, (yylsp - yylen), yylen);
        yyerror_range[1] = yyloc;
        YY_REDUCE_PRINT(yyn);
        switch (yyn) {
                case 2:
#line 63 "bone.y" /* yacc.c:1663  */
                {
                        extern void yy_register(bnAST * a);
                        yy_register((yyvsp[0].ast_value));
                }
#line 1638 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 3:
#line 68 "bone.y" /* yacc.c:1663  */
                {
                        yyclearin;
                }
#line 1646 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 4:
#line 74 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewArgumentAST((yyvsp[0].ast_value));
                }
#line 1654 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 5:
#line 78 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewArgumentListAST(
                            bnNewArgumentAST((yyvsp[0].ast_value)),
                            (yyvsp[-2].ast_value));
                }
#line 1662 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 6:
#line 84 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewParameterAST((yyvsp[0].svvalue));
                }
#line 1670 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 7:
#line 88 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewParameterListAST(
                            bnNewParameterAST((yyvsp[0].svvalue)),
                            (yyvsp[-2].ast_value));
                }
#line 1678 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 8:
#line 94 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = (yyvsp[0].ast_value);
                }
#line 1686 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 9:
#line 98 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewStatementListAST(
                            (yyvsp[0].ast_value), (yyvsp[-1].ast_value));
                }
#line 1694 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 10:
#line 104 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewBlankAST();
                }
#line 1702 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 11:
#line 108 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = (yyvsp[0].ast_value);
                }
#line 1710 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 12:
#line 114 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewExprStmtAST((yyvsp[-1].ast_value));
                }
#line 1718 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 14:
#line 119 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewIfAST((yyvsp[-2].ast_value),
                                                       (yyvsp[0].ast_value));
                }
#line 1726 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 15:
#line 123 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewIfElseAST(bnNewIfAST((yyvsp[-4].ast_value),
                                                      (yyvsp[-2].ast_value)),
                                           (yyvsp[0].ast_value));
                }
#line 1734 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 16:
#line 127 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewWhileAST((yyvsp[-2].ast_value),
                                                          (yyvsp[0].ast_value));
                }
#line 1742 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 20:
#line 134 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewAST(BN_AST_CONTINUE);
                }
#line 1750 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 21:
#line 138 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewAST(BN_AST_BREAK);
                }
#line 1758 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 22:
#line 144 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = (yyvsp[-1].ast_value);
                }
#line 1766 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 23:
#line 150 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewReturnAST(bnNewBlankAST());
                }
#line 1774 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 24:
#line 156 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewScopeInjectionAST((yyvsp[-1].ast_value));
                }
#line 1782 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 25:
#line 162 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewDeferAST((yyvsp[0].ast_value));
                }
#line 1790 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 26:
#line 168 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = (yyvsp[-1].ast_value);
                }
#line 1798 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 29:
#line 176 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewUnaryAST(BN_AST_POSITIVE,
                                                          (yyvsp[0].ast_value));
                }
#line 1806 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 30:
#line 180 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewUnaryAST(BN_AST_NEGATIVE,
                                                          (yyvsp[0].ast_value));
                }
#line 1814 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 31:
#line 184 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewObjectInjectionAST(
                            (yyvsp[-2].ast_value), (yyvsp[0].ast_value));
                }
#line 1822 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 32:
#line 188 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewBinaryAST(BN_AST_PLUS, (yyvsp[-2].ast_value),
                                           (yyvsp[0].ast_value));
                }
#line 1830 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 33:
#line 192 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewBinaryAST(BN_AST_MINUS, (yyvsp[-2].ast_value),
                                           (yyvsp[0].ast_value));
                }
#line 1838 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 34:
#line 196 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewBinaryAST(
                            BN_AST_MULTIPLY, (yyvsp[-2].ast_value),
                            (yyvsp[0].ast_value));
                }
#line 1846 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 35:
#line 200 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewBinaryAST(BN_AST_DIVIDE, (yyvsp[-2].ast_value),
                                           (yyvsp[0].ast_value));
                }
#line 1854 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 36:
#line 204 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewBinaryAST(BN_AST_MODULO, (yyvsp[-2].ast_value),
                                           (yyvsp[0].ast_value));
                }
#line 1862 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 37:
#line 208 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewBinaryAST(BN_AST_BIT_OR, (yyvsp[-2].ast_value),
                                           (yyvsp[0].ast_value));
                }
#line 1870 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 38:
#line 212 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewBinaryAST(BN_AST_EQUAL, (yyvsp[-2].ast_value),
                                           (yyvsp[0].ast_value));
                }
#line 1878 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 39:
#line 216 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewBinaryAST(
                            BN_AST_NOTEQUAL, (yyvsp[-2].ast_value),
                            (yyvsp[0].ast_value));
                }
#line 1886 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 40:
#line 220 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewBinaryAST(
                            BN_AST_BIT_AND, (yyvsp[-2].ast_value),
                            (yyvsp[0].ast_value));
                }
#line 1894 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 41:
#line 224 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewBinaryAST(BN_AST_EXC_OR, (yyvsp[-2].ast_value),
                                           (yyvsp[0].ast_value));
                }
#line 1902 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 42:
#line 228 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewBinaryAST(
                            BN_AST_LOGIC_OR, (yyvsp[-2].ast_value),
                            (yyvsp[0].ast_value));
                }
#line 1910 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 43:
#line 232 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewBinaryAST(
                            BN_AST_LOGIC_AND, (yyvsp[-2].ast_value),
                            (yyvsp[0].ast_value));
                }
#line 1918 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 44:
#line 236 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewBinaryAST(BN_AST_ASSIGN, (yyvsp[-2].ast_value),
                                           (yyvsp[0].ast_value));
                }
#line 1926 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 45:
#line 240 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewBinaryAST(
                            BN_AST_PLUS_ASSIGN, (yyvsp[-2].ast_value),
                            (yyvsp[0].ast_value));
                }
#line 1934 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 46:
#line 244 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewBinaryAST(
                            BN_AST_MINUS_ASSIGN, (yyvsp[-2].ast_value),
                            (yyvsp[0].ast_value));
                }
#line 1942 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 47:
#line 248 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewBinaryAST(
                            BN_AST_MULTIPLY_ASSIGN, (yyvsp[-2].ast_value),
                            (yyvsp[0].ast_value));
                }
#line 1950 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 48:
#line 252 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewBinaryAST(
                            BN_AST_DIVIDE_ASSIGN, (yyvsp[-2].ast_value),
                            (yyvsp[0].ast_value));
                }
#line 1958 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 49:
#line 256 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewBinaryAST(
                            BN_AST_MODULO_ASSIGN, (yyvsp[-2].ast_value),
                            (yyvsp[0].ast_value));
                }
#line 1966 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 50:
#line 260 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewBinaryAST(
                            BN_AST_AND_ASSIGN, (yyvsp[-2].ast_value),
                            (yyvsp[0].ast_value));
                }
#line 1974 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 51:
#line 264 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewBinaryAST(
                            BN_AST_OR_ASSIGN, (yyvsp[-2].ast_value),
                            (yyvsp[0].ast_value));
                }
#line 1982 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 52:
#line 268 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewBinaryAST(
                            BN_AST_EXC_OR_ASSIGN, (yyvsp[-2].ast_value),
                            (yyvsp[0].ast_value));
                }
#line 1990 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 53:
#line 272 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewBinaryAST(
                            BN_AST_LSHIFT_ASSIGN, (yyvsp[-2].ast_value),
                            (yyvsp[0].ast_value));
                }
#line 1998 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 54:
#line 276 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewBinaryAST(
                            BN_AST_RSHIFT_ASSIGN, (yyvsp[-2].ast_value),
                            (yyvsp[0].ast_value));
                }
#line 2006 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 55:
#line 280 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewBinaryAST(BN_AST_GT, (yyvsp[-2].ast_value),
                                           (yyvsp[0].ast_value));
                }
#line 2014 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 56:
#line 284 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewBinaryAST(BN_AST_GE, (yyvsp[-2].ast_value),
                                           (yyvsp[0].ast_value));
                }
#line 2022 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 57:
#line 288 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewBinaryAST(BN_AST_LT, (yyvsp[-2].ast_value),
                                           (yyvsp[0].ast_value));
                }
#line 2030 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 58:
#line 292 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewBinaryAST(BN_AST_LE, (yyvsp[-2].ast_value),
                                           (yyvsp[0].ast_value));
                }
#line 2038 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 59:
#line 295 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewBinaryAST(BN_AST_LSHIFT, (yyvsp[-2].ast_value),
                                           (yyvsp[0].ast_value));
                }
#line 2046 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 60:
#line 298 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewBinaryAST(BN_AST_RSHIFT, (yyvsp[-2].ast_value),
                                           (yyvsp[0].ast_value));
                }
#line 2054 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 61:
#line 302 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewUnaryAST(BN_AST_CHILDA, (yyvsp[0].ast_value));
                }
#line 2062 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 62:
#line 306 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewUnaryAST(BN_AST_NOT, (yyvsp[0].ast_value));
                }
#line 2070 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 65:
#line 314 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewFuncCall(
                            (yyvsp[-3].ast_value), (yyvsp[-1].ast_value));
                }
#line 2078 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 66:
#line 318 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewFuncCall((yyvsp[-2].ast_value),
                                                          bnNewBlankAST());
                }
#line 2086 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 67:
#line 324 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewLambdaAST(
                            (yyvsp[-5].ast_value), (yyvsp[-2].ast_value),
                            (yyvsp[0].ast_value));
                }
#line 2094 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 68:
#line 328 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewLambdaAST(
                            bnNewBlankAST(), (yyvsp[-2].ast_value),
                            (yyvsp[0].ast_value));
                }
#line 2102 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 69:
#line 332 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewLambdaAST(
                            (yyvsp[-4].ast_value), bnNewBlankAST(),
                            (yyvsp[0].ast_value));
                }
#line 2110 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 70:
#line 336 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewLambdaAST(bnNewBlankAST(), bnNewBlankAST(),
                                           (yyvsp[0].ast_value));
                }
#line 2118 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 71:
#line 340 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewLambdaAST(bnNewBlankAST(), bnNewDotDotDotAST(),
                                           (yyvsp[0].ast_value));
                }
#line 2126 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 72:
#line 344 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewLambdaAST(
                            (yyvsp[-5].ast_value), bnNewDotDotDotAST(),
                            (yyvsp[0].ast_value));
                }
#line 2134 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 73:
#line 350 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewArrayLit((yyvsp[-1].ast_value));
                }
#line 2142 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 74:
#line 354 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewArrayLit(bnNewBlankAST());
                }
#line 2150 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 75:
#line 360 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) =
                            bnNewVariableAST((yyvsp[0].svvalue));
                }
#line 2158 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 76:
#line 364 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewMemberAccessAST(
                            (yyvsp[-2].ast_value), (yyvsp[0].svvalue));
                }
#line 2166 "bone.tab.c" /* yacc.c:1663  */
                break;

                case 77:
#line 368 "bone.y" /* yacc.c:1663  */
                {
                        (yyval.ast_value) = bnNewArraySubscriptAST(
                            (yyvsp[-3].ast_value), (yyvsp[-1].ast_value));
                }
#line 2174 "bone.tab.c" /* yacc.c:1663  */
                break;

#line 2178 "bone.tab.c" /* yacc.c:1663  */
                default:
                        break;
        }
        /* User semantic actions sometimes alter yychar, and that requires
           that yytoken be updated with the new translation.  We take the
           approach of translating immediately before every use of yytoken.
           One alternative is translating here after every semantic action,
           but that translation would be missed if the semantic action invokes
           YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
           if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
           incorrect destructor might then be invoked immediately.  In the
           case of YYERROR or YYBACKUP, subsequent parser actions might lead
           to an incorrect destructor call or verbose syntax error message
           before the lookahead is translated.  */
        YY_SYMBOL_PRINT("-> $$ =", yyr1[yyn], &yyval, &yyloc);

        YYPOPSTACK(yylen);
        yylen = 0;
        YY_STACK_PRINT(yyss, yyssp);

        *++yyvsp = yyval;
        *++yylsp = yyloc;

        /* Now 'shift' the result of the reduction.  Determine what state
           that goes to, based on the state we popped back to and the rule
           number reduced by.  */

        yyn = yyr1[yyn];

        yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
        if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
                yystate = yytable[yystate];
        else
                yystate = yydefgoto[yyn - YYNTOKENS];

        goto yynewstate;

/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
        /* Make sure we have latest lookahead translation.  See comments at
           user semantic actions for why this is necessary.  */
        yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE(yychar);

        /* If not already recovering from an error, report this error.  */
        if (!yyerrstatus) {
                ++yynerrs;
#if !YYERROR_VERBOSE
                yyerror(YY_("syntax error"));
#else
#define YYSYNTAX_ERROR yysyntax_error(&yymsg_alloc, &yymsg, yyssp, yytoken)
                {
                        char const *yymsgp = YY_("syntax error");
                        int yysyntax_error_status;
                        yysyntax_error_status = YYSYNTAX_ERROR;
                        if (yysyntax_error_status == 0)
                                yymsgp = yymsg;
                        else if (yysyntax_error_status == 1) {
                                if (yymsg != yymsgbuf) YYSTACK_FREE(yymsg);
                                yymsg = (char *)YYSTACK_ALLOC(yymsg_alloc);
                                if (!yymsg) {
                                        yymsg = yymsgbuf;
                                        yymsg_alloc = sizeof yymsgbuf;
                                        yysyntax_error_status = 2;
                                } else {
                                        yysyntax_error_status = YYSYNTAX_ERROR;
                                        yymsgp = yymsg;
                                }
                        }
                        yyerror(yymsgp);
                        if (yysyntax_error_status == 2) goto yyexhaustedlab;
                }
#undef YYSYNTAX_ERROR
#endif
        }

        yyerror_range[1] = yylloc;

        if (yyerrstatus == 3) {
                /* If just tried and failed to reuse lookahead token after an
                   error, discard it.  */

                if (yychar <= YYEOF) {
                        /* Return failure if at end of input.  */
                        if (yychar == YYEOF) YYABORT;
                } else {
                        yydestruct("Error: discarding", yytoken, &yylval,
                                   &yylloc);
                        yychar = YYEMPTY;
                }
        }

        /* Else will try to reuse lookahead token after shifting the error
           token.  */
        goto yyerrlab1;

/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

        /* Pacify compilers like GCC when the user code never invokes
           YYERROR and the label yyerrorlab therefore never appears in user
           code.  */
        if (/*CONSTCOND*/ 0) goto yyerrorlab;

        /* Do not reclaim the symbols of the rule whose action triggered
           this YYERROR.  */
        YYPOPSTACK(yylen);
        yylen = 0;
        YY_STACK_PRINT(yyss, yyssp);
        yystate = *yyssp;
        goto yyerrlab1;

/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
        yyerrstatus = 3; /* Each real token shifted decrements this.  */

        for (;;) {
                yyn = yypact[yystate];
                if (!yypact_value_is_default(yyn)) {
                        yyn += YYTERROR;
                        if (0 <= yyn && yyn <= YYLAST &&
                            yycheck[yyn] == YYTERROR) {
                                yyn = yytable[yyn];
                                if (0 < yyn) break;
                        }
                }

                /* Pop the current state because it cannot handle the error
                 * token.  */
                if (yyssp == yyss) YYABORT;

                yyerror_range[1] = *yylsp;
                yydestruct("Error: popping", yystos[yystate], yyvsp, yylsp);
                YYPOPSTACK(1);
                yystate = *yyssp;
                YY_STACK_PRINT(yyss, yyssp);
        }

        YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
        *++yyvsp = yylval;
        YY_IGNORE_MAYBE_UNINITIALIZED_END

        yyerror_range[2] = yylloc;
        /* Using YYLLOC is tempting, but would change the location of
           the lookahead.  YYLOC is available though.  */
        YYLLOC_DEFAULT(yyloc, yyerror_range, 2);
        *++yylsp = yyloc;

        /* Shift the error token.  */
        YY_SYMBOL_PRINT("Shifting", yystos[yyn], yyvsp, yylsp);

        yystate = yyn;
        goto yynewstate;

/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
        yyresult = 0;
        goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
        yyresult = 1;
        goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
        yyerror(YY_("memory exhausted"));
        yyresult = 2;
        /* Fall through.  */
#endif

yyreturn:
        if (yychar != YYEMPTY) {
                /* Make sure we have latest lookahead translation.  See comments
                   at user semantic actions for why this is necessary.  */
                yytoken = YYTRANSLATE(yychar);
                yydestruct("Cleanup: discarding lookahead", yytoken, &yylval,
                           &yylloc);
        }
        /* Do not reclaim the symbols of the rule whose action triggered
           this YYABORT or YYACCEPT.  */
        YYPOPSTACK(yylen);
        YY_STACK_PRINT(yyss, yyssp);
        while (yyssp != yyss) {
                yydestruct("Cleanup: popping", yystos[*yyssp], yyvsp, yylsp);
                YYPOPSTACK(1);
        }
#ifndef yyoverflow
        if (yyss != yyssa) YYSTACK_FREE(yyss);
#endif
#if YYERROR_VERBOSE
        if (yymsg != yymsgbuf) YYSTACK_FREE(yymsg);
#endif
        return yyresult;
}
#line 380 "bone.y" /* yacc.c:1907  */
#pragma clang diagnostic pop