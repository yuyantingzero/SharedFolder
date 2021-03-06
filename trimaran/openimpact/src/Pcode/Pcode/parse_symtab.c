/* A Bison parser, made by GNU Bison 1.875.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     DCL = 258,
     DCL_FUNC = 259,
     INCLUDE_NAME = 260,
     IP_SYM_TAB_ENT_HEADER = 261,
     IP_SYM_TAB_ENT_SOURCE = 262,
     SYMBOL_TABLE_BEGIN = 263,
     SYMBOL_TABLE_END = 264,
     SYM_TAB_ENTRY_BLOCK = 265,
     SYM_TAB_ENTRY_END = 266,
     SYM_TAB_ENTRY_FUNC = 267,
     SYM_TAB_ENTRY_OTHER = 268,
     SYM_TAB_ENTRY_SCOPE = 269
   };
#endif
#define DCL 258
#define DCL_FUNC 259
#define INCLUDE_NAME 260
#define IP_SYM_TAB_ENT_HEADER 261
#define IP_SYM_TAB_ENT_SOURCE 262
#define SYMBOL_TABLE_BEGIN 263
#define SYMBOL_TABLE_END 264
#define SYM_TAB_ENTRY_BLOCK 265
#define SYM_TAB_ENTRY_END 266
#define SYM_TAB_ENTRY_FUNC 267
#define SYM_TAB_ENTRY_OTHER 268
#define SYM_TAB_ENTRY_SCOPE 269




/* Copy the first part of user declarations.  */
#line 1 "parse_symtab.y"


/*****************************************************************************\
 *
 *		      Illinois Open Source License
 *                     University of Illinois/NCSA
 *                         Open Source License
 *
 * Copyright (c) 2004, The University of Illinois at Urbana-Champaign.
 * All rights reserved.
 *
 * Developed by:             
 *
 *		IMPACT Research Group
 *
 *		University of Illinois at Urbana-Champaign
 *
 *              http://www.crhc.uiuc.edu/IMPACT
 *              http://www.gelato.org
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal with the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimers.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimers in
 * the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the names of the IMPACT Research Group, the University of
 * Illinois, nor the names of its contributors may be used to endorse
 * or promote products derived from this Software without specific
 * prior written permission.  THE SOFTWARE IS PROVIDED "AS IS",
 * WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS WITH THE SOFTWARE.
 *
\*****************************************************************************/
/*****************************************************************************
 *	File:	 parse_symtab.y
 *      Authors: Robert Kidd and Wen-mei Hwu
 * 	Copyright (c) 2003 Robert Kidd, Wen-mei Hwu
 *		 and The Board of Trustees of the University of Illinois.
 *		 All rights reserved.
 *      License Agreement specifies the terms and conditions for 
 *      redistribution.
 *****************************************************************************/

#include <config.h>
#include <stdio.h>
#include <library/i_list.h>
#include <library/block_sparse_array.h>
#include <Pcode/pcode.h>
#include <Pcode/struct.h>
#include <Pcode/symtab.h>
#include <Pcode/read_symtab.h>
#if 0
#include <Pcode/lex_symtab.h>
#endif
#include <Pcode/parse_prefix_symtab.h>

#define YYPARSE_VERBOSE 1
#define YYPARSE_PARAM ret

void yyerror (char *s);
extern int yylex (void *lvalp);


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 83 "parse_symtab.y"
typedef union YYSTYPE {
  char *_includename;
  Dcl _dcl;
  List _list;
  SymTabEntry _symtabentry;
  IPSymTabEnt _ipsymtabent;
  SymbolTable _symboltable;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 191 "src/Pcode/Pcode/parse_s"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */
#line 203 "src/Pcode/Pcode/parse_s"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  20
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   31

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  15
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  11
/* YYNRULES -- Number of rules. */
#define YYNRULES  22
/* YYNRULES -- Number of states. */
#define YYNSTATES  32

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   269

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     5,     9,    13,    14,    17,    20,    24,
      26,    28,    30,    32,    34,    36,    40,    43,    46,    47,
      50,    53,    56
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      21,     0,    -1,     3,    -1,    12,    25,     4,    -1,     6,
      25,    11,    -1,    -1,    19,     6,    -1,    19,     7,    -1,
       8,    19,     9,    -1,    20,    -1,    24,    -1,    23,    -1,
      18,    -1,    17,    -1,    16,    -1,    14,    25,    11,    -1,
       7,    22,    -1,     5,    23,    -1,    -1,    25,    10,    -1,
      25,    12,    -1,    25,    13,    -1,    25,    14,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   120,   120,   240,   279,   304,   307,   311,   321,   343,
     347,   351,   355,   359,   363,   371,   384,   401,   410,   413,
     417,   421,   425
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "DCL", "DCL_FUNC", "INCLUDE_NAME", 
  "IP_SYM_TAB_ENT_HEADER", "IP_SYM_TAB_ENT_SOURCE", "SYMBOL_TABLE_BEGIN", 
  "SYMBOL_TABLE_END", "SYM_TAB_ENTRY_BLOCK", "SYM_TAB_ENTRY_END", 
  "SYM_TAB_ENTRY_FUNC", "SYM_TAB_ENTRY_OTHER", "SYM_TAB_ENTRY_SCOPE", 
  "$accept", "dcl", "func", "header_file", "ip_symbol_table_list", 
  "ip_table", "pcode_file", "scope", "source_body", "source_file", 
  "sym_tab_entry_list", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    15,    16,    17,    18,    19,    19,    19,    20,    21,
      21,    21,    21,    21,    21,    22,    23,    24,    25,    25,
      25,    25,    25
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     3,     3,     0,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     3,     2,     2,     0,     2,
       2,     2,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     2,     0,    18,     0,     5,    18,    14,    13,    12,
       9,     0,    11,    10,    17,     0,    18,    16,     0,     0,
       1,    19,     4,    20,    21,    22,     0,     6,     7,     8,
       3,    15
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     7,     8,     9,    18,    10,    11,    17,    12,    13,
      15
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -13
static const yysigned_char yypact[] =
{
       9,   -13,     6,   -13,   -12,   -13,   -13,   -13,   -13,   -13,
     -13,     3,   -13,   -13,   -13,    12,   -13,   -13,    -2,    -4,
     -13,   -13,   -13,   -13,   -13,   -13,    17,   -13,   -13,   -13,
     -13,   -13
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -13,   -13,   -13,   -13,   -13,   -13,   -13,   -13,    16,   -13,
      -5
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned char yytable[] =
{
      30,    19,    16,    20,    27,    28,    21,    29,    23,    24,
      25,    26,     1,     4,     2,     3,     4,     5,    14,     0,
       0,     6,    21,    22,    23,    24,    25,    21,    31,    23,
      24,    25
};

static const yysigned_char yycheck[] =
{
       4,     6,    14,     0,     6,     7,    10,     9,    12,    13,
      14,    16,     3,     7,     5,     6,     7,     8,     2,    -1,
      -1,    12,    10,    11,    12,    13,    14,    10,    11,    12,
      13,    14
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     3,     5,     6,     7,     8,    12,    16,    17,    18,
      20,    21,    23,    24,    23,    25,    14,    22,    19,    25,
       0,    10,    11,    12,    13,    14,    25,     6,     7,     9,
       4,    11
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1

/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  /* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 121 "parse_symtab.y"
    {
		  SymbolTable table;
		  SymTabEntry entry;
		  Key key;

		  /* We must have a table to read a DCL. */
		  if (!(table = ((SymTabParserArg *)ret)->table))
		    YYABORT;

		  switch (P_GetDclType (yyvsp[0]._dcl))
		    {
		    case TT_FUNC:
		      P_punt ("parse_symtab.y:dcl:%d TT_FUNC seen in DCL",
			      __LINE__ - 1);
		      break;
		    case TT_TYPE:
		      {
			TypeDcl t;

			t = P_GetDclTypeDcl (yyvsp[0]._dcl);
			P_SetDclTypeDcl (yyvsp[0]._dcl, NULL);

			key = P_GetTypeDclKey (t);
			    
			entry = PST_GetSymTabEntryFromMem (table, key);
			P_SetSymTabEntryTypeDcl (entry, t);
		      }
		      break;
		    case TT_VAR:
		      {
			VarDcl v;

			v = P_GetDclVarDcl (yyvsp[0]._dcl);
			P_SetDclVarDcl (yyvsp[0]._dcl, NULL);

			key = P_GetVarDclKey (v);
			    
			entry = PST_GetSymTabEntryFromMem (table, key);
			P_SetSymTabEntryVarDcl (entry, v);
		      }
		      break;
		    case TT_STRUCT:
		      {
			StructDcl s;

			s = P_GetDclStructDcl (yyvsp[0]._dcl);
			P_SetDclStructDcl (yyvsp[0]._dcl, NULL);

			key = P_GetStructDclKey (s);
			    
			entry = PST_GetSymTabEntryFromMem (table, key);
			P_SetSymTabEntryStructDcl (entry, s);

			/* Link fields inside the StructDcl to their
			 * SymTabEntries. */
			PST_LinkSymbolsStruct (table, s);
		      }
		      break;
		    case TT_UNION:
		      {
			UnionDcl u;

			u = P_GetDclUnionDcl (yyvsp[0]._dcl);
			P_SetDclUnionDcl (yyvsp[0]._dcl, NULL);

			key = P_GetUnionDclKey (u);

			entry = PST_GetSymTabEntryFromMem (table, key);
			P_SetSymTabEntryUnionDcl (entry, u);

			/* Link fields inside the UnionDcl to their
			 * SymTabEntries. */
			PST_LinkSymbolsUnion (table, u);
		      }
		      break;
		    case TT_ENUM:
		      {
			EnumDcl e;

			e = P_GetDclEnumDcl (yyvsp[0]._dcl);
			P_SetDclEnumDcl (yyvsp[0]._dcl, NULL);

			key = P_GetEnumDclKey (e);

			entry = PST_GetSymTabEntryFromMem (table, key);
			P_SetSymTabEntryEnumDcl (entry, e);

			/* Link fields inside the EnumDcl to their
			 * SymTabEntries. */
			PST_LinkSymbolsEnum (table, e);
		      }
		      break;
		    case TT_ASM:
		      {
			AsmDcl a;

			a = P_GetDclAsmDcl (yyvsp[0]._dcl);
			P_SetDclAsmDcl (yyvsp[0]._dcl, NULL);

			key = P_GetAsmDclKey (a);

			entry = PST_GetSymTabEntryFromMem (table, key);
			P_SetSymTabEntryAsmDcl (entry, a);
		      }
		      break;
		    default:
		      P_punt ("parse_symtab.y:header_body:%d Unhandled Dcl "
			      "type %d", __LINE__ - 1, P_GetDclType (yyvsp[0]._dcl));
		      break;
		    }

		  yyvsp[0]._dcl = P_RemoveDcl (yyvsp[0]._dcl);

		  ((SymTabParserArg *)ret)->key = key;

		  yyval._symboltable = table;
		}
    break;

  case 3:
#line 241 "parse_symtab.y"
    {
		  SymTabEntry func_entry, entry;
		  SymbolTable table;
		  FuncDcl func_dcl;
		  Key key;

		  /* We must have a table supplied to read a FuncDcl. */
		  if (!(table = ((SymTabParserArg *)ret)->table))
		    YYABORT;

		  key = P_GetSymTabEntryKey (yyvsp[-2]._symtabentry);

		  /* $1 (the SYM_TAB_ENTRY_FUNC) is a copy of the one
		   * that appears in the scope.  It can be removed now. */
		  yyvsp[-2]._symtabentry = P_RemoveSymTabEntry (yyvsp[-2]._symtabentry);

		  func_entry = PST_GetSymTabEntryFromMem (table, key);

		  List_start (yyvsp[-1]._list);
		  while ((entry = (SymTabEntry)List_next (yyvsp[-1]._list)))
		    PST_AddSymTabEntry (table, entry);
		  yyvsp[-1]._list = P_RemoveList (yyvsp[-1]._list, NULL);

		  func_dcl = P_GetDclFuncDcl (yyvsp[0]._dcl);
		  P_SetDclFuncDcl (yyvsp[0]._dcl, NULL);
		  yyvsp[0]._dcl = P_RemoveDcl (yyvsp[0]._dcl);

		  P_SetSymTabEntryFuncDcl (func_entry, func_dcl);
		  PST_LinkSymbolsFunc (table, func_dcl);

		  ((SymTabParserArg *)ret)->key = key;

		  yyval._symboltable = table;
		}
    break;

  case 4:
#line 281 "parse_symtab.y"
    {
		  IPSymTabEnt ipste;
		  SymTabEntry entry;

		  ipste = yyvsp[-2]._ipsymtabent;
		  ((SymTabParserArg *)ret)->ipste = ipste;

		  List_start (yyvsp[-1]._list);
		  while ((entry = (SymTabEntry)List_next (yyvsp[-1]._list)))
		    PST_AddIPSymTabEntEntry (ipste, entry);
		  yyvsp[-1]._list = P_RemoveList (yyvsp[-1]._list, NULL);

		  /* Header files are always stored in the IP symbol table's
		   * file. */
		  P_SetIPSymTabEntFlags (ipste, IPSTEF_EMBEDDED);
		  
		  yyvsp[0]._symtabentry = P_RemoveSymTabEntry (yyvsp[0]._symtabentry);

		  yyval._ipsymtabent = ipste;
		}
    break;

  case 5:
#line 304 "parse_symtab.y"
    {
		  yyval._list = NULL;
		}
    break;

  case 6:
#line 308 "parse_symtab.y"
    {
		  yyval._list = List_insert_last (yyvsp[-1]._list, yyvsp[0]._ipsymtabent);
		}
    break;

  case 7:
#line 312 "parse_symtab.y"
    {
		  yyval._list = List_insert_last (yyvsp[-1]._list, yyvsp[0]._ipsymtabent);
		}
    break;

  case 8:
#line 323 "parse_symtab.y"
    {
		  SymbolTable table;
		  IPSymTabEnt ipste;
		  int file_key;

		  table = yyvsp[-2]._symboltable;
		  ((SymTabParserArg *)ret)->table = table;

		  List_start (yyvsp[-1]._list);
		  while ((ipste = (IPSymTabEnt)List_next (yyvsp[-1]._list)))
		    {
		      file_key = P_GetIPSymTabEntKey (ipste);
		      PST_SetFile (table, file_key, ipste);
		    }
		  yyvsp[-1]._list = P_RemoveList (yyvsp[-1]._list, NULL);

		  yyval._symboltable = table;
		}
    break;

  case 9:
#line 344 "parse_symtab.y"
    {
		  YYACCEPT;
		}
    break;

  case 10:
#line 348 "parse_symtab.y"
    {
		  YYACCEPT;
		}
    break;

  case 11:
#line 352 "parse_symtab.y"
    {
		  YYACCEPT;
		}
    break;

  case 12:
#line 356 "parse_symtab.y"
    {
		  YYACCEPT;
		}
    break;

  case 13:
#line 360 "parse_symtab.y"
    {
		  YYACCEPT;
		}
    break;

  case 14:
#line 364 "parse_symtab.y"
    {
		  YYACCEPT;
		}
    break;

  case 15:
#line 372 "parse_symtab.y"
    {
		  List scope = NULL;

		  scope = List_insert_last (scope, yyvsp[-2]._symtabentry);
		  scope = List_append (scope, yyvsp[-1]._list);

		  yyvsp[0]._symtabentry = P_RemoveSymTabEntry (yyvsp[0]._symtabentry);

		  yyval._list = scope;
		}
    break;

  case 16:
#line 385 "parse_symtab.y"
    {
		  IPSymTabEnt ipste;
		  SymTabEntry entry;

		  ipste = yyvsp[-1]._ipsymtabent;
		  ((SymTabParserArg *)ret)->ipste = ipste;

		  List_start (yyvsp[0]._list);
		  while ((entry = (SymTabEntry)List_next (yyvsp[0]._list)))
		    PST_AddIPSymTabEntEntry (ipste, entry);
		  yyvsp[0]._list = P_RemoveList (yyvsp[0]._list, NULL);

		  yyval._ipsymtabent = ipste;
		}
    break;

  case 17:
#line 402 "parse_symtab.y"
    {
		  ((SymTabParserArg *)ret)->include_name = yyvsp[-1]._includename;
		  
		  yyval._ipsymtabent = yyvsp[0]._ipsymtabent;
		}
    break;

  case 18:
#line 410 "parse_symtab.y"
    {
		  yyval._list = NULL;
		}
    break;

  case 19:
#line 414 "parse_symtab.y"
    {
		  yyval._list = List_insert_last (yyvsp[-1]._list, yyvsp[0]._symtabentry);
		}
    break;

  case 20:
#line 418 "parse_symtab.y"
    {
		  yyval._list = List_insert_last (yyvsp[-1]._list, yyvsp[0]._symtabentry);
		}
    break;

  case 21:
#line 422 "parse_symtab.y"
    {
		  yyval._list = List_insert_last (yyvsp[-1]._list, yyvsp[0]._symtabentry);
		}
    break;

  case 22:
#line 426 "parse_symtab.y"
    {
		  yyval._list = List_insert_last (yyvsp[-1]._list, yyvsp[0]._symtabentry);
		}
    break;


    }

/* Line 991 of yacc.c.  */
#line 1444 "src/Pcode/Pcode/parse_s"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab2;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:

  /* Suppress GCC warning that yyerrlab1 is unused when no action
     invokes YYERROR.  */
#if defined (__GNUC_MINOR__) && 2093 <= (__GNUC__ * 1000 + __GNUC_MINOR__)
  __attribute__ ((__unused__))
#endif


  goto yyerrlab2;


/*---------------------------------------------------------------.
| yyerrlab2 -- pop states until the error token can be shifted.  |
`---------------------------------------------------------------*/
yyerrlab2:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


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

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 430 "parse_symtab.y"


void
yyerror (char *s)
{
  P_warn ("parse_symtab.y: %s\n", s);
}





