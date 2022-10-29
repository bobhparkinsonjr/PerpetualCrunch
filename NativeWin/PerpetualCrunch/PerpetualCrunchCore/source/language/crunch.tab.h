/* A Bison parser, made by GNU Bison 3.7.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_CRUNCH_CRUNCH_TAB_H_INCLUDED
# define YY_CRUNCH_CRUNCH_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef CRUNCHDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define CRUNCHDEBUG 1
#  else
#   define CRUNCHDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define CRUNCHDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined CRUNCHDEBUG */
#if CRUNCHDEBUG
extern int crunchdebug;
#endif

/* Token kinds.  */
#ifndef CRUNCHTOKENTYPE
# define CRUNCHTOKENTYPE
  enum crunchtokentype
  {
    CRUNCHEMPTY = -2,
    CRUNCHEOF = 0,                 /* "end of file"  */
    CRUNCHerror = 256,             /* error  */
    CRUNCHUNDEF = 257,             /* "invalid token"  */
    TOKEN_ID = 258,                /* TOKEN_ID  */
    TOKEN_RESERVED_STRING = 259,   /* TOKEN_RESERVED_STRING  */
    TOKEN_RESERVED_CHAR = 260,     /* TOKEN_RESERVED_CHAR  */
    TOKEN_RESERVED_I64 = 261,      /* TOKEN_RESERVED_I64  */
    TOKEN_RESERVED_U64 = 262,      /* TOKEN_RESERVED_U64  */
    TOKEN_RESERVED_F64 = 263,      /* TOKEN_RESERVED_F64  */
    TOKEN_RESERVED_VOID = 264,     /* TOKEN_RESERVED_VOID  */
    TOKEN_RESERVED_BOOL = 265,     /* TOKEN_RESERVED_BOOL  */
    TOKEN_RESERVED_RETURN = 266,   /* TOKEN_RESERVED_RETURN  */
    TOKEN_RESERVED_INCLUDE = 267,  /* TOKEN_RESERVED_INCLUDE  */
    TOKEN_RESERVED_IF = 268,       /* TOKEN_RESERVED_IF  */
    TOKEN_RESERVED_ELSE = 269,     /* TOKEN_RESERVED_ELSE  */
    TOKEN_RESERVED_SWITCH = 270,   /* TOKEN_RESERVED_SWITCH  */
    TOKEN_RESERVED_CASE = 271,     /* TOKEN_RESERVED_CASE  */
    TOKEN_RESERVED_BREAK = 272,    /* TOKEN_RESERVED_BREAK  */
    TOKEN_RESERVED_DEFAULT = 273,  /* TOKEN_RESERVED_DEFAULT  */
    TOKEN_RESERVED_WHILE = 274,    /* TOKEN_RESERVED_WHILE  */
    TOKEN_SEMICOLON = 275,         /* TOKEN_SEMICOLON  */
    TOKEN_COMMA = 276,             /* TOKEN_COMMA  */
    TOKEN_COLON = 277,             /* TOKEN_COLON  */
    TOKEN_DOT = 278,               /* TOKEN_DOT  */
    TOKEN_BIT_OR = 279,            /* TOKEN_BIT_OR  */
    TOKEN_BIT_AND = 280,           /* TOKEN_BIT_AND  */
    TOKEN_BIT_COMP = 281,          /* TOKEN_BIT_COMP  */
    TOKEN_BOOL_AND = 282,          /* TOKEN_BOOL_AND  */
    TOKEN_BOOL_OR = 283,           /* TOKEN_BOOL_OR  */
    TOKEN_BOOL_NOT = 284,          /* TOKEN_BOOL_NOT  */
    TOKEN_GT = 285,                /* TOKEN_GT  */
    TOKEN_GE = 286,                /* TOKEN_GE  */
    TOKEN_LT = 287,                /* TOKEN_LT  */
    TOKEN_LE = 288,                /* TOKEN_LE  */
    TOKEN_EE = 289,                /* TOKEN_EE  */
    TOKEN_NE = 290,                /* TOKEN_NE  */
    TOKEN_ASSIGN = 291,            /* TOKEN_ASSIGN  */
    TOKEN_I64 = 292,               /* TOKEN_I64  */
    TOKEN_U64 = 293,               /* TOKEN_U64  */
    TOKEN_F64 = 294,               /* TOKEN_F64  */
    TOKEN_STRING = 295,            /* TOKEN_STRING  */
    TOKEN_CHAR = 296,              /* TOKEN_CHAR  */
    TOKEN_TRUE = 297,              /* TOKEN_TRUE  */
    TOKEN_FALSE = 298,             /* TOKEN_FALSE  */
    TOKEN_PLUS = 299,              /* TOKEN_PLUS  */
    TOKEN_MINUS = 300,             /* TOKEN_MINUS  */
    TOKEN_MULTIPLY = 301,          /* TOKEN_MULTIPLY  */
    TOKEN_DIVIDE = 302,            /* TOKEN_DIVIDE  */
    TOKEN_OPEN_PARAN = 303,        /* TOKEN_OPEN_PARAN  */
    TOKEN_CLOSE_PARAN = 304,       /* TOKEN_CLOSE_PARAN  */
    TOKEN_OPEN_CURLY = 305,        /* TOKEN_OPEN_CURLY  */
    TOKEN_CLOSE_CURLY = 306,       /* TOKEN_CLOSE_CURLY  */
    TOKEN_OPEN_BRACKET = 307,      /* TOKEN_OPEN_BRACKET  */
    TOKEN_CLOSE_BRACKET = 308      /* TOKEN_CLOSE_BRACKET  */
  };
  typedef enum crunchtokentype crunchtoken_kind_t;
#endif

/* Value type.  */
#if ! defined CRUNCHSTYPE && ! defined CRUNCHSTYPE_IS_DECLARED
union CRUNCHSTYPE
{
#line 46 "crunch.y"

  const crunch::language::crunch_string *text;
  crunch::language::crunch_char cval;
	crunch::language::crunch_i64 ival;
	crunch::language::crunch_f64 fval;
  crunch::language::crunch_u64 uval;
  crunch::language::SyntaxNode *node;

#line 134 "crunch.tab.h"

};
typedef union CRUNCHSTYPE CRUNCHSTYPE;
# define CRUNCHSTYPE_IS_TRIVIAL 1
# define CRUNCHSTYPE_IS_DECLARED 1
#endif


extern CRUNCHSTYPE crunchlval;

int crunchparse (void);

#endif /* !YY_CRUNCH_CRUNCH_TAB_H_INCLUDED  */
