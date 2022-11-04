/* A Bison parser, made by GNU Bison 3.7.1.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.7.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         CRUNCHSTYPE
/* Substitute the variable and function names.  */
#define yyparse         crunchparse
#define yylex           crunchlex
#define yyerror         cruncherror
#define yydebug         crunchdebug
#define yynerrs         crunchnerrs
#define yylval          crunchlval
#define yychar          crunchchar

/* First part of user prologue.  */
#line 4 "crunch.y"


#include "../crunch_global.h"

#include <stdlib.h>
#include <stdio.h>

#include "syntax tree/crunch_syntax_nodes.h"

#include "crunch_script_compiler.h"
#include "crunch_script_types.h"

extern int crunchlex();
extern int crunchparse();
extern FILE* crunchin;

typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern YY_BUFFER_STATE crunch_scan_string( const char *str );
extern void crunch_delete_buffer( YY_BUFFER_STATE buffer );
extern void crunch_switch_to_buffer( YY_BUFFER_STATE new_buffer );

extern void crunchpush_buffer_state( YY_BUFFER_STATE new_buffer );
extern void crunchpop_buffer_state( void );

extern void crunchrestart( FILE *input_file );

extern int crunchlineno;

void cruncherror( const char* s );

static crunch::language::SyntaxNodeRoot *sRoot = nullptr;
static crunch::language::ScriptCompiler *sCompiler = nullptr;

crunch::language::ScriptCompiler* getParseCompiler() { return sCompiler; }
crunch::language::SyntaxNodeRoot* getParseRoot() { return sRoot; }
unsigned int getParseLineNumber() { return (unsigned int)( crunchlineno ); }

#define _st_params getParseCompiler(), getParseRoot(), getParseLineNumber()

#define _st_op( _op ) crunch::language::SyntaxNodeExpression::Operator::OPERATOR_##_op
#define _st_bop( _op ) crunch::language::SyntaxNodeBinaryExpression::Operator::OPERATOR_##_op


#line 123 "crunch.tab.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "crunch.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TOKEN_ID = 3,                   /* TOKEN_ID  */
  YYSYMBOL_TOKEN_RESERVED_STRING = 4,      /* TOKEN_RESERVED_STRING  */
  YYSYMBOL_TOKEN_RESERVED_CHAR = 5,        /* TOKEN_RESERVED_CHAR  */
  YYSYMBOL_TOKEN_RESERVED_I64 = 6,         /* TOKEN_RESERVED_I64  */
  YYSYMBOL_TOKEN_RESERVED_U64 = 7,         /* TOKEN_RESERVED_U64  */
  YYSYMBOL_TOKEN_RESERVED_F64 = 8,         /* TOKEN_RESERVED_F64  */
  YYSYMBOL_TOKEN_RESERVED_VOID = 9,        /* TOKEN_RESERVED_VOID  */
  YYSYMBOL_TOKEN_RESERVED_BOOL = 10,       /* TOKEN_RESERVED_BOOL  */
  YYSYMBOL_TOKEN_RESERVED_RETURN = 11,     /* TOKEN_RESERVED_RETURN  */
  YYSYMBOL_TOKEN_RESERVED_INCLUDE = 12,    /* TOKEN_RESERVED_INCLUDE  */
  YYSYMBOL_TOKEN_RESERVED_IF = 13,         /* TOKEN_RESERVED_IF  */
  YYSYMBOL_TOKEN_RESERVED_ELSE = 14,       /* TOKEN_RESERVED_ELSE  */
  YYSYMBOL_TOKEN_RESERVED_SWITCH = 15,     /* TOKEN_RESERVED_SWITCH  */
  YYSYMBOL_TOKEN_RESERVED_CASE = 16,       /* TOKEN_RESERVED_CASE  */
  YYSYMBOL_TOKEN_RESERVED_BREAK = 17,      /* TOKEN_RESERVED_BREAK  */
  YYSYMBOL_TOKEN_RESERVED_DEFAULT = 18,    /* TOKEN_RESERVED_DEFAULT  */
  YYSYMBOL_TOKEN_RESERVED_WHILE = 19,      /* TOKEN_RESERVED_WHILE  */
  YYSYMBOL_TOKEN_SEMICOLON = 20,           /* TOKEN_SEMICOLON  */
  YYSYMBOL_TOKEN_COMMA = 21,               /* TOKEN_COMMA  */
  YYSYMBOL_TOKEN_COLON = 22,               /* TOKEN_COLON  */
  YYSYMBOL_TOKEN_DOT = 23,                 /* TOKEN_DOT  */
  YYSYMBOL_TOKEN_BIT_OR = 24,              /* TOKEN_BIT_OR  */
  YYSYMBOL_TOKEN_BIT_AND = 25,             /* TOKEN_BIT_AND  */
  YYSYMBOL_TOKEN_BIT_COMP = 26,            /* TOKEN_BIT_COMP  */
  YYSYMBOL_TOKEN_BOOL_AND = 27,            /* TOKEN_BOOL_AND  */
  YYSYMBOL_TOKEN_BOOL_OR = 28,             /* TOKEN_BOOL_OR  */
  YYSYMBOL_TOKEN_BOOL_NOT = 29,            /* TOKEN_BOOL_NOT  */
  YYSYMBOL_TOKEN_GT = 30,                  /* TOKEN_GT  */
  YYSYMBOL_TOKEN_GE = 31,                  /* TOKEN_GE  */
  YYSYMBOL_TOKEN_LT = 32,                  /* TOKEN_LT  */
  YYSYMBOL_TOKEN_LE = 33,                  /* TOKEN_LE  */
  YYSYMBOL_TOKEN_EE = 34,                  /* TOKEN_EE  */
  YYSYMBOL_TOKEN_NE = 35,                  /* TOKEN_NE  */
  YYSYMBOL_TOKEN_ASSIGN = 36,              /* TOKEN_ASSIGN  */
  YYSYMBOL_TOKEN_PLUS_EQUALS = 37,         /* TOKEN_PLUS_EQUALS  */
  YYSYMBOL_TOKEN_MINUS_EQUALS = 38,        /* TOKEN_MINUS_EQUALS  */
  YYSYMBOL_TOKEN_MULTIPLY_EQUALS = 39,     /* TOKEN_MULTIPLY_EQUALS  */
  YYSYMBOL_TOKEN_DIVIDE_EQUALS = 40,       /* TOKEN_DIVIDE_EQUALS  */
  YYSYMBOL_TOKEN_I64 = 41,                 /* TOKEN_I64  */
  YYSYMBOL_TOKEN_U64 = 42,                 /* TOKEN_U64  */
  YYSYMBOL_TOKEN_F64 = 43,                 /* TOKEN_F64  */
  YYSYMBOL_TOKEN_STRING = 44,              /* TOKEN_STRING  */
  YYSYMBOL_TOKEN_CHAR = 45,                /* TOKEN_CHAR  */
  YYSYMBOL_TOKEN_TRUE = 46,                /* TOKEN_TRUE  */
  YYSYMBOL_TOKEN_FALSE = 47,               /* TOKEN_FALSE  */
  YYSYMBOL_TOKEN_PLUS = 48,                /* TOKEN_PLUS  */
  YYSYMBOL_TOKEN_MINUS = 49,               /* TOKEN_MINUS  */
  YYSYMBOL_TOKEN_MULTIPLY = 50,            /* TOKEN_MULTIPLY  */
  YYSYMBOL_TOKEN_DIVIDE = 51,              /* TOKEN_DIVIDE  */
  YYSYMBOL_TOKEN_OPEN_PARAN = 52,          /* TOKEN_OPEN_PARAN  */
  YYSYMBOL_TOKEN_CLOSE_PARAN = 53,         /* TOKEN_CLOSE_PARAN  */
  YYSYMBOL_TOKEN_OPEN_CURLY = 54,          /* TOKEN_OPEN_CURLY  */
  YYSYMBOL_TOKEN_CLOSE_CURLY = 55,         /* TOKEN_CLOSE_CURLY  */
  YYSYMBOL_TOKEN_OPEN_BRACKET = 56,        /* TOKEN_OPEN_BRACKET  */
  YYSYMBOL_TOKEN_CLOSE_BRACKET = 57,       /* TOKEN_CLOSE_BRACKET  */
  YYSYMBOL_YYACCEPT = 58,                  /* $accept  */
  YYSYMBOL_root = 59,                      /* root  */
  YYSYMBOL_root_object = 60,               /* root_object  */
  YYSYMBOL_function_declaration = 61,      /* function_declaration  */
  YYSYMBOL_function_body = 62,             /* function_body  */
  YYSYMBOL_variable = 63,                  /* variable  */
  YYSYMBOL_type_identifier = 64,           /* type_identifier  */
  YYSYMBOL_basic_type = 65,                /* basic_type  */
  YYSYMBOL_function_parameter_list = 66,   /* function_parameter_list  */
  YYSYMBOL_function_parameter = 67,        /* function_parameter  */
  YYSYMBOL_statement_block = 68,           /* statement_block  */
  YYSYMBOL_statement_list = 69,            /* statement_list  */
  YYSYMBOL_statement = 70,                 /* statement  */
  YYSYMBOL_assignment_statement = 71,      /* assignment_statement  */
  YYSYMBOL_declaration_statement = 72,     /* declaration_statement  */
  YYSYMBOL_include_statement = 73,         /* include_statement  */
  YYSYMBOL_expression_statement = 74,      /* expression_statement  */
  YYSYMBOL_return_statement = 75,          /* return_statement  */
  YYSYMBOL_if_statement = 76,              /* if_statement  */
  YYSYMBOL_if_header = 77,                 /* if_header  */
  YYSYMBOL_while_statement = 78,           /* while_statement  */
  YYSYMBOL_break_statement = 79,           /* break_statement  */
  YYSYMBOL_switch_statement = 80,          /* switch_statement  */
  YYSYMBOL_switch_header = 81,             /* switch_header  */
  YYSYMBOL_switch_body = 82,               /* switch_body  */
  YYSYMBOL_switch_case_list = 83,          /* switch_case_list  */
  YYSYMBOL_switch_case = 84,               /* switch_case  */
  YYSYMBOL_switch_default = 85,            /* switch_default  */
  YYSYMBOL_expression_list = 86,           /* expression_list  */
  YYSYMBOL_expression = 87,                /* expression  */
  YYSYMBOL_literal_type = 88,              /* literal_type  */
  YYSYMBOL_function_call = 89              /* function_call  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined CRUNCHSTYPE_IS_TRIVIAL && CRUNCHSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1106

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  58
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  93
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  169

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   312


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
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
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57
};

#if CRUNCHDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   166,   166,   167,   170,   171,   174,   177,   178,   181,
     182,   183,   186,   187,   190,   191,   192,   193,   194,   195,
     196,   199,   200,   201,   204,   207,   208,   209,   212,   213,
     216,   217,   218,   219,   220,   221,   222,   223,   226,   227,
     228,   229,   230,   233,   234,   237,   240,   243,   244,   247,
     248,   251,   254,   256,   259,   262,   265,   268,   269,   270,
     273,   276,   279,   280,   281,   284,   285,   286,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   307,   308,   309,   310,
     311,   312,   313,   316
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if CRUNCHDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TOKEN_ID",
  "TOKEN_RESERVED_STRING", "TOKEN_RESERVED_CHAR", "TOKEN_RESERVED_I64",
  "TOKEN_RESERVED_U64", "TOKEN_RESERVED_F64", "TOKEN_RESERVED_VOID",
  "TOKEN_RESERVED_BOOL", "TOKEN_RESERVED_RETURN", "TOKEN_RESERVED_INCLUDE",
  "TOKEN_RESERVED_IF", "TOKEN_RESERVED_ELSE", "TOKEN_RESERVED_SWITCH",
  "TOKEN_RESERVED_CASE", "TOKEN_RESERVED_BREAK", "TOKEN_RESERVED_DEFAULT",
  "TOKEN_RESERVED_WHILE", "TOKEN_SEMICOLON", "TOKEN_COMMA", "TOKEN_COLON",
  "TOKEN_DOT", "TOKEN_BIT_OR", "TOKEN_BIT_AND", "TOKEN_BIT_COMP",
  "TOKEN_BOOL_AND", "TOKEN_BOOL_OR", "TOKEN_BOOL_NOT", "TOKEN_GT",
  "TOKEN_GE", "TOKEN_LT", "TOKEN_LE", "TOKEN_EE", "TOKEN_NE",
  "TOKEN_ASSIGN", "TOKEN_PLUS_EQUALS", "TOKEN_MINUS_EQUALS",
  "TOKEN_MULTIPLY_EQUALS", "TOKEN_DIVIDE_EQUALS", "TOKEN_I64", "TOKEN_U64",
  "TOKEN_F64", "TOKEN_STRING", "TOKEN_CHAR", "TOKEN_TRUE", "TOKEN_FALSE",
  "TOKEN_PLUS", "TOKEN_MINUS", "TOKEN_MULTIPLY", "TOKEN_DIVIDE",
  "TOKEN_OPEN_PARAN", "TOKEN_CLOSE_PARAN", "TOKEN_OPEN_CURLY",
  "TOKEN_CLOSE_CURLY", "TOKEN_OPEN_BRACKET", "TOKEN_CLOSE_BRACKET",
  "$accept", "root", "root_object", "function_declaration",
  "function_body", "variable", "type_identifier", "basic_type",
  "function_parameter_list", "function_parameter", "statement_block",
  "statement_list", "statement", "assignment_statement",
  "declaration_statement", "include_statement", "expression_statement",
  "return_statement", "if_statement", "if_header", "while_statement",
  "break_statement", "switch_statement", "switch_header", "switch_body",
  "switch_case_list", "switch_case", "switch_default", "expression_list",
  "expression", "literal_type", "function_call", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312
};
#endif

#define YYPACT_NINF (-123)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-13)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -123,   276,  -123,  -123,  -123,  -123,  -123,  -123,  -123,  -123,
    -123,   -33,  -123,  -123,     9,  -123,  -123,    -7,   -38,  -123,
     184,    14,    -6,  -123,  -123,   184,   -34,  -123,    29,  -123,
       0,   435,   -26,   -24,     5,   -23,   462,   462,  -123,  -123,
    -123,  -123,  -123,  -123,  -123,   462,   462,  -123,    55,     1,
     196,  -123,  -123,  -123,  -123,  -123,  -123,   355,  -123,  -123,
    -123,     6,   492,  -123,  -123,  -123,  -123,   -21,   526,   462,
     462,  -123,   462,   962,   992,  1050,   826,   462,   462,   462,
     462,   462,   -15,   462,  -123,  -123,   249,    27,  -123,  -123,
    -123,  -123,   462,   462,   462,   462,   462,   462,   462,   462,
     462,   462,   462,   462,   462,   462,   462,   462,  -123,   860,
     894,   928,  -123,   560,   594,   628,   662,   696,  -123,   462,
      -3,   962,  -123,   302,   355,   -10,    -5,   992,   992,  1021,
    1021,   -13,   -13,   -13,   -13,    75,    75,  1050,  1050,    34,
      34,   760,  -123,  -123,   355,  -123,  -123,  -123,  -123,  -123,
     730,   462,  -123,  -123,  -123,   462,    32,  -123,  -123,  -123,
    -123,  -123,  -123,   962,   796,   407,   407,   407,   407
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     1,    12,    14,    15,    16,    17,    18,    19,
      20,     0,     3,     4,     0,    13,     5,     0,     0,    45,
      21,     0,     0,    22,    24,     0,     0,    23,     0,     6,
       9,     0,     0,     0,     0,     0,     0,     0,    86,    87,
      88,    89,    90,    91,    92,     0,     0,     7,    85,     0,
       0,    28,    30,    31,    32,    33,    34,     0,    37,    35,
      36,     0,     0,    65,    84,    47,    85,     0,     0,     0,
       0,    53,     0,    82,    79,    78,     0,     0,     0,     0,
       0,     0,     0,    62,     8,    29,     0,    49,    25,    57,
      54,    46,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    48,     0,
       0,     0,    83,     0,     0,     0,     0,     0,    44,     0,
       0,    63,    26,     0,     0,     0,    10,    81,    80,    76,
      77,    68,    69,    70,    71,    66,    67,    72,    73,    74,
      75,     0,    51,    55,     0,    38,    39,    40,    41,    42,
       0,     0,    93,    27,    50,     0,     0,    56,    58,    59,
      11,    52,    43,    64,     0,     0,     0,    61,    60
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -123,  -123,  -123,  -123,  -123,   -27,    -1,  -123,  -123,    36,
    -122,   -79,   -41,  -123,  -123,  -123,  -123,  -123,  -123,  -123,
    -123,  -123,  -123,  -123,  -123,  -123,  -123,  -123,  -123,    79,
    -123,  -123
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    12,    13,    29,    66,    67,    15,    22,    23,
      87,    50,    51,    52,    53,    16,    54,    55,    56,    57,
      58,    59,    60,    61,    90,   125,   158,   159,   120,    62,
      63,    64
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      14,    48,   154,   -12,    82,   118,   155,   123,   156,    85,
      92,    17,    18,    19,    20,    25,    88,    24,   151,    21,
      28,   119,   161,    48,    21,    71,    69,    49,    70,    72,
      48,    83,    30,     4,     5,     6,     7,     8,     9,    10,
      31,   124,    32,   107,    33,   157,    34,    26,    35,    49,
     152,   107,   -12,    83,   165,    36,    49,    92,    37,    48,
      89,    27,     0,     0,    97,    98,    99,   100,   101,   102,
      38,    39,    40,    41,    42,    43,    44,     0,    45,     0,
       0,    46,    85,    88,    47,    49,   167,   168,     0,     0,
     107,    77,    78,    79,    80,    81,    48,    48,    92,     0,
       0,     0,     0,    88,     0,    97,    98,    99,   100,     0,
      68,     0,     0,     0,     0,    73,    74,    48,     0,     0,
       0,     0,    49,    49,    75,    76,    85,    85,     0,     0,
       0,   107,     0,     0,     0,     0,     0,     0,    48,    48,
      48,    48,     0,    49,     0,     0,     0,     0,   109,   110,
       0,   111,     0,     0,     0,     0,   113,   114,   115,   116,
     117,     0,   121,     0,    49,    49,    49,    49,     0,     0,
       0,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,     3,     4,     5,
       6,     7,     8,     9,    10,     0,     0,     0,   150,    30,
       4,     5,     6,     7,     8,     9,    10,    31,     0,    32,
       0,    33,     0,    34,     0,    35,     0,     0,     0,     0,
       0,     0,    36,     0,     0,    37,     0,     0,     0,     0,
     163,     0,     0,     0,   164,     0,     0,    38,    39,    40,
      41,    42,    43,    44,     0,    45,     0,     0,    46,     0,
       0,    84,    30,     4,     5,     6,     7,     8,     9,    10,
      31,     0,    32,     0,    33,     0,    34,     0,    35,     0,
       0,     0,     0,     0,     0,    36,     2,     0,    37,     3,
       4,     5,     6,     7,     8,     9,    10,     0,    11,     0,
      38,    39,    40,    41,    42,    43,    44,     0,    45,     0,
       0,    46,     0,     0,   122,    30,     4,     5,     6,     7,
       8,     9,    10,    31,     0,    32,     0,    33,     0,    34,
       0,    35,     0,     0,     0,     0,     0,     0,    36,     0,
       0,    37,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    38,    39,    40,    41,    42,    43,    44,
       0,    45,     0,     0,    46,     0,     0,   153,    30,     4,
       5,     6,     7,     8,     9,    10,    31,     0,    32,     0,
      33,     0,    34,     0,    35,     0,     0,     0,     0,     0,
       0,    36,     0,     0,    37,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    38,    39,    40,    41,
      42,    43,    44,     0,    45,     0,     0,    46,     0,    86,
      30,     4,     5,     6,     7,     8,     9,    10,    31,     0,
      32,     0,    33,     0,    34,     0,    35,     0,     0,     0,
       0,     0,     0,    36,     0,     0,    37,     0,    30,     4,
       5,     6,     7,     8,     9,    10,     0,     0,    38,    39,
      40,    41,    42,    43,    44,    65,    45,     0,     0,    46,
       0,    36,     0,     0,    37,    30,     4,     5,     6,     7,
       8,     9,    10,     0,     0,     0,    38,    39,    40,    41,
      42,    43,    44,     0,    45,     0,     0,    46,    36,     0,
       0,    37,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    38,    39,    40,    41,    42,    43,    44,
       0,    45,    91,     0,    46,    92,    93,    94,     0,    95,
      96,     0,    97,    98,    99,   100,   101,   102,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     103,   104,   105,   106,     0,     0,   108,     0,   107,    92,
      93,    94,     0,    95,    96,     0,    97,    98,    99,   100,
     101,   102,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   103,   104,   105,   106,     0,     0,
     145,     0,   107,    92,    93,    94,     0,    95,    96,     0,
      97,    98,    99,   100,   101,   102,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   103,   104,
     105,   106,     0,     0,   146,     0,   107,    92,    93,    94,
       0,    95,    96,     0,    97,    98,    99,   100,   101,   102,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   103,   104,   105,   106,     0,     0,   147,     0,
     107,    92,    93,    94,     0,    95,    96,     0,    97,    98,
      99,   100,   101,   102,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   103,   104,   105,   106,
       0,     0,   148,     0,   107,    92,    93,    94,     0,    95,
      96,     0,    97,    98,    99,   100,   101,   102,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     103,   104,   105,   106,     0,     0,   149,     0,   107,    92,
      93,    94,     0,    95,    96,     0,    97,    98,    99,   100,
     101,   102,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   103,   104,   105,   106,     0,     0,
     162,     0,   107,    92,    93,    94,     0,    95,    96,     0,
      97,    98,    99,   100,   101,   102,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   103,   104,
     105,   106,     0,    92,    93,    94,   107,    95,    96,     0,
      97,    98,    99,   100,   101,   102,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   103,   104,
     105,   106,     0,     0,     0,     0,   107,   160,   166,    92,
      93,    94,     0,    95,    96,     0,    97,    98,    99,   100,
     101,   102,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   103,   104,   105,   106,     0,    92,
      93,    94,   107,    95,    96,     0,    97,    98,    99,   100,
     101,   102,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   103,   104,   105,   106,     0,   112,
       0,     0,   107,    92,    93,    94,     0,    95,    96,     0,
      97,    98,    99,   100,   101,   102,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   103,   104,
     105,   106,     0,   142,     0,     0,   107,    92,    93,    94,
       0,    95,    96,     0,    97,    98,    99,   100,   101,   102,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   103,   104,   105,   106,     0,   143,     0,     0,
     107,    92,    93,    94,     0,    95,    96,     0,    97,    98,
      99,   100,   101,   102,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   103,   104,   105,   106,
       0,   144,     0,     0,   107,    92,    93,    94,     0,    95,
      96,     0,    97,    98,    99,   100,   101,   102,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     103,   104,   105,   106,     0,    92,     0,     0,   107,    95,
      96,     0,    97,    98,    99,   100,   101,   102,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     103,   104,   105,   106,    92,     0,     0,     0,   107,     0,
       0,    97,    98,    99,   100,   101,   102,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   103,
     104,   105,   106,    92,     0,     0,     0,   107,     0,     0,
      97,    98,    99,   100,   101,   102,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     105,   106,     0,     0,     0,     0,   107
};

static const yytype_int16 yycheck[] =
{
       1,    28,   124,     3,     3,    20,    16,    86,    18,    50,
      23,    44,     3,    20,    52,    21,    57,     3,    21,    20,
      54,    36,   144,    50,    25,    20,    52,    28,    52,    52,
      57,    52,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    14,    13,    56,    15,    55,    17,    53,    19,    50,
      53,    56,    52,    52,    22,    26,    57,    23,    29,    86,
      54,    25,    -1,    -1,    30,    31,    32,    33,    34,    35,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    -1,
      -1,    52,   123,   124,    55,    86,   165,   166,    -1,    -1,
      56,    36,    37,    38,    39,    40,   123,   124,    23,    -1,
      -1,    -1,    -1,   144,    -1,    30,    31,    32,    33,    -1,
      31,    -1,    -1,    -1,    -1,    36,    37,   144,    -1,    -1,
      -1,    -1,   123,   124,    45,    46,   167,   168,    -1,    -1,
      -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,   165,   166,
     167,   168,    -1,   144,    -1,    -1,    -1,    -1,    69,    70,
      -1,    72,    -1,    -1,    -1,    -1,    77,    78,    79,    80,
      81,    -1,    83,    -1,   165,   166,   167,   168,    -1,    -1,
      -1,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,   119,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    -1,    13,
      -1,    15,    -1,    17,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,    -1,    29,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,   155,    -1,    -1,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    13,    -1,    15,    -1,    17,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    26,     0,    -1,    29,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    12,    -1,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    -1,    13,    -1,    15,    -1,    17,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    -1,    -1,    52,    -1,    -1,    55,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    -1,    13,    -1,
      15,    -1,    17,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,    -1,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    -1,    -1,    52,    -1,    54,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    -1,
      13,    -1,    15,    -1,    17,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    26,    -1,    -1,    29,    -1,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    41,    42,
      43,    44,    45,    46,    47,    20,    49,    -1,    -1,    52,
      -1,    26,    -1,    -1,    29,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    -1,    -1,    52,    26,    -1,
      -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    20,    -1,    52,    23,    24,    25,    -1,    27,
      28,    -1,    30,    31,    32,    33,    34,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    51,    -1,    -1,    20,    -1,    56,    23,
      24,    25,    -1,    27,    28,    -1,    30,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    -1,    -1,
      20,    -1,    56,    23,    24,    25,    -1,    27,    28,    -1,
      30,    31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    -1,    -1,    20,    -1,    56,    23,    24,    25,
      -1,    27,    28,    -1,    30,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    -1,    -1,    20,    -1,
      56,    23,    24,    25,    -1,    27,    28,    -1,    30,    31,
      32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,
      -1,    -1,    20,    -1,    56,    23,    24,    25,    -1,    27,
      28,    -1,    30,    31,    32,    33,    34,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    51,    -1,    -1,    20,    -1,    56,    23,
      24,    25,    -1,    27,    28,    -1,    30,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    -1,    -1,
      20,    -1,    56,    23,    24,    25,    -1,    27,    28,    -1,
      30,    31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    -1,    23,    24,    25,    56,    27,    28,    -1,
      30,    31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    -1,    -1,    -1,    -1,    56,    57,    22,    23,
      24,    25,    -1,    27,    28,    -1,    30,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    -1,    23,
      24,    25,    56,    27,    28,    -1,    30,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    -1,    53,
      -1,    -1,    56,    23,    24,    25,    -1,    27,    28,    -1,
      30,    31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    -1,    53,    -1,    -1,    56,    23,    24,    25,
      -1,    27,    28,    -1,    30,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    -1,    53,    -1,    -1,
      56,    23,    24,    25,    -1,    27,    28,    -1,    30,    31,
      32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,
      -1,    53,    -1,    -1,    56,    23,    24,    25,    -1,    27,
      28,    -1,    30,    31,    32,    33,    34,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    51,    -1,    23,    -1,    -1,    56,    27,
      28,    -1,    30,    31,    32,    33,    34,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    49,    50,    51,    23,    -1,    -1,    -1,    56,    -1,
      -1,    30,    31,    32,    33,    34,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      49,    50,    51,    23,    -1,    -1,    -1,    56,    -1,    -1,
      30,    31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    51,    -1,    -1,    -1,    -1,    56
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    59,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    12,    60,    61,    64,    65,    73,    44,     3,    20,
      52,    64,    66,    67,     3,    21,    53,    67,    54,    62,
       3,    11,    13,    15,    17,    19,    26,    29,    41,    42,
      43,    44,    45,    46,    47,    49,    52,    55,    63,    64,
      69,    70,    71,    72,    74,    75,    76,    77,    78,    79,
      80,    81,    87,    88,    89,    20,    63,    64,    87,    52,
      52,    20,    52,    87,    87,    87,    87,    36,    37,    38,
      39,    40,     3,    52,    55,    70,    54,    68,    70,    54,
      82,    20,    23,    24,    25,    27,    28,    30,    31,    32,
      33,    34,    35,    48,    49,    50,    51,    56,    20,    87,
      87,    87,    53,    87,    87,    87,    87,    87,    20,    36,
      86,    87,    55,    69,    14,    83,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    53,    53,    53,    20,    20,    20,    20,    20,
      87,    21,    53,    55,    68,    16,    18,    55,    84,    85,
      57,    68,    20,    87,    87,    22,    22,    69,    69
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    58,    59,    59,    60,    60,    61,    62,    62,    63,
      63,    63,    64,    64,    65,    65,    65,    65,    65,    65,
      65,    66,    66,    66,    67,    68,    68,    68,    69,    69,
      70,    70,    70,    70,    70,    70,    70,    70,    71,    71,
      71,    71,    71,    72,    72,    73,    74,    75,    75,    76,
      76,    77,    78,    79,    80,    81,    82,    83,    83,    83,
      84,    85,    86,    86,    86,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    88,    88,    88,    88,
      88,    88,    88,    89
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     6,     2,     3,     1,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     1,     3,     2,     1,     2,     3,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       4,     4,     4,     5,     3,     3,     2,     2,     3,     2,
       4,     4,     5,     2,     2,     4,     3,     0,     2,     2,
       4,     3,     0,     1,     3,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       3,     3,     2,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = CRUNCHEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == CRUNCHEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use CRUNCHerror or CRUNCHUNDEF. */
#define YYERRCODE CRUNCHUNDEF


/* Enable debugging if requested.  */
#if CRUNCHDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !CRUNCHDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !CRUNCHDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = CRUNCHEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == CRUNCHEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= CRUNCHEOF)
    {
      yychar = CRUNCHEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == CRUNCHerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = CRUNCHUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = CRUNCHEMPTY;
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
| yyreduce -- do a reduction.  |
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
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* root: %empty  */
#line 166 "crunch.y"
       {}
#line 1515 "crunch.tab.cpp"
    break;

  case 3: /* root: root root_object  */
#line 167 "crunch.y"
                     { sRoot->append( (yyvsp[0]. node ) ); }
#line 1521 "crunch.tab.cpp"
    break;

  case 4: /* root_object: function_declaration  */
#line 170 "crunch.y"
                                   { (yyval. node ) = (yyvsp[0]. node ); }
#line 1527 "crunch.tab.cpp"
    break;

  case 5: /* root_object: include_statement  */
#line 171 "crunch.y"
                      { (yyval. node ) = (yyvsp[0]. node ); }
#line 1533 "crunch.tab.cpp"
    break;

  case 6: /* function_declaration: type_identifier TOKEN_ID TOKEN_OPEN_PARAN function_parameter_list TOKEN_CLOSE_PARAN function_body  */
#line 174 "crunch.y"
                                                                                                                         { (yyval. node ) = new crunch::language::SyntaxNodeFunctionDeclaration( _st_params, (yyvsp[-5]. node ), *(yyvsp[-4]. text ), (yyvsp[-2]. node ), (yyvsp[0]. node ) ); }
#line 1539 "crunch.tab.cpp"
    break;

  case 7: /* function_body: TOKEN_OPEN_CURLY TOKEN_CLOSE_CURLY  */
#line 177 "crunch.y"
                                                   { (yyval. node ) = nullptr; }
#line 1545 "crunch.tab.cpp"
    break;

  case 8: /* function_body: TOKEN_OPEN_CURLY statement_list TOKEN_CLOSE_CURLY  */
#line 178 "crunch.y"
                                                      { (yyval. node ) = (yyvsp[-1]. node ); }
#line 1551 "crunch.tab.cpp"
    break;

  case 9: /* variable: TOKEN_ID  */
#line 181 "crunch.y"
                    { (yyval. node ) = new crunch::language::SyntaxNodeVariable( _st_params, *(yyvsp[0]. text ) ); }
#line 1557 "crunch.tab.cpp"
    break;

  case 10: /* variable: expression TOKEN_DOT expression  */
#line 182 "crunch.y"
                                    { (yyval. node ) = new crunch::language::SyntaxNodeVariable( _st_params, (yyvsp[-2]. node ), (yyvsp[0]. node ), crunch::language::SyntaxNodeVariable::SubType::SUB_TYPE_MEMBER ); }
#line 1563 "crunch.tab.cpp"
    break;

  case 11: /* variable: expression TOKEN_OPEN_BRACKET expression TOKEN_CLOSE_BRACKET  */
#line 183 "crunch.y"
                                                                 { (yyval. node ) = new crunch::language::SyntaxNodeVariable( _st_params, (yyvsp[-3]. node ), (yyvsp[-1]. node ), crunch::language::SyntaxNodeVariable::SubType::SUB_TYPE_SUBSCRIPT ); }
#line 1569 "crunch.tab.cpp"
    break;

  case 12: /* type_identifier: TOKEN_ID  */
#line 186 "crunch.y"
                           { (yyval. node ) = new crunch::language::SyntaxNodeTypeIdentifier( _st_params, crunch::language::ScriptTypes::ScriptType::TYPE_NONE, *(yyvsp[0]. text ) ); }
#line 1575 "crunch.tab.cpp"
    break;

  case 13: /* type_identifier: basic_type  */
#line 187 "crunch.y"
               { (yyval. node ) = (yyvsp[0]. node ); }
#line 1581 "crunch.tab.cpp"
    break;

  case 14: /* basic_type: TOKEN_RESERVED_STRING  */
#line 190 "crunch.y"
                                   { (yyval. node ) = new crunch::language::SyntaxNodeTypeIdentifier( _st_params, crunch::language::ScriptTypes::ScriptType::TYPE_STRING ); }
#line 1587 "crunch.tab.cpp"
    break;

  case 15: /* basic_type: TOKEN_RESERVED_CHAR  */
#line 191 "crunch.y"
                        { (yyval. node ) = new crunch::language::SyntaxNodeTypeIdentifier( _st_params, crunch::language::ScriptTypes::ScriptType::TYPE_CHAR ); }
#line 1593 "crunch.tab.cpp"
    break;

  case 16: /* basic_type: TOKEN_RESERVED_I64  */
#line 192 "crunch.y"
                       { (yyval. node ) = new crunch::language::SyntaxNodeTypeIdentifier( _st_params, crunch::language::ScriptTypes::ScriptType::TYPE_I64 ); }
#line 1599 "crunch.tab.cpp"
    break;

  case 17: /* basic_type: TOKEN_RESERVED_U64  */
#line 193 "crunch.y"
                       { (yyval. node ) = new crunch::language::SyntaxNodeTypeIdentifier( _st_params, crunch::language::ScriptTypes::ScriptType::TYPE_U64 ); }
#line 1605 "crunch.tab.cpp"
    break;

  case 18: /* basic_type: TOKEN_RESERVED_F64  */
#line 194 "crunch.y"
                       { (yyval. node ) = new crunch::language::SyntaxNodeTypeIdentifier( _st_params, crunch::language::ScriptTypes::ScriptType::TYPE_F64 ); }
#line 1611 "crunch.tab.cpp"
    break;

  case 19: /* basic_type: TOKEN_RESERVED_VOID  */
#line 195 "crunch.y"
                        { (yyval. node ) = new crunch::language::SyntaxNodeTypeIdentifier( _st_params, crunch::language::ScriptTypes::ScriptType::TYPE_VOID ); }
#line 1617 "crunch.tab.cpp"
    break;

  case 20: /* basic_type: TOKEN_RESERVED_BOOL  */
#line 196 "crunch.y"
                        { (yyval. node ) = new crunch::language::SyntaxNodeTypeIdentifier( _st_params, crunch::language::ScriptTypes::ScriptType::TYPE_I64 ); }
#line 1623 "crunch.tab.cpp"
    break;

  case 21: /* function_parameter_list: %empty  */
#line 199 "crunch.y"
                          { (yyval. node ) = nullptr; }
#line 1629 "crunch.tab.cpp"
    break;

  case 22: /* function_parameter_list: function_parameter  */
#line 200 "crunch.y"
                       { (yyval. node ) = new crunch::language::SyntaxNodeFunctionParameterList( _st_params ); (yyval. node )->append( (yyvsp[0]. node ) ); }
#line 1635 "crunch.tab.cpp"
    break;

  case 23: /* function_parameter_list: function_parameter_list TOKEN_COMMA function_parameter  */
#line 201 "crunch.y"
                                                           { (yyval. node ) = (yyvsp[-2]. node ); (yyval. node )->append( (yyvsp[0]. node ) ); }
#line 1641 "crunch.tab.cpp"
    break;

  case 24: /* function_parameter: type_identifier TOKEN_ID  */
#line 204 "crunch.y"
                                              { (yyval. node ) = new crunch::language::SyntaxNodeFunctionParameter( _st_params, (yyvsp[-1]. node ), *(yyvsp[0]. text ) ); }
#line 1647 "crunch.tab.cpp"
    break;

  case 25: /* statement_block: statement  */
#line 207 "crunch.y"
                            { (yyval. node ) = (yyvsp[0]. node ); }
#line 1653 "crunch.tab.cpp"
    break;

  case 26: /* statement_block: TOKEN_OPEN_CURLY TOKEN_CLOSE_CURLY  */
#line 208 "crunch.y"
                                       { (yyval. node ) = nullptr; }
#line 1659 "crunch.tab.cpp"
    break;

  case 27: /* statement_block: TOKEN_OPEN_CURLY statement_list TOKEN_CLOSE_CURLY  */
#line 209 "crunch.y"
                                                      { ( (crunch::language::SyntaxNodeStatementList*)( (yyvsp[-1]. node ) ) )->setScoped(); (yyval. node ) = (yyvsp[-1]. node ); }
#line 1665 "crunch.tab.cpp"
    break;

  case 28: /* statement_list: statement  */
#line 212 "crunch.y"
                           { (yyval. node ) = new crunch::language::SyntaxNodeStatementList( _st_params ); (yyval. node )->append( (yyvsp[0]. node ) ); }
#line 1671 "crunch.tab.cpp"
    break;

  case 29: /* statement_list: statement_list statement  */
#line 213 "crunch.y"
                             { (yyval. node ) = (yyvsp[-1]. node ); (yyval. node )->append( (yyvsp[0]. node ) ); }
#line 1677 "crunch.tab.cpp"
    break;

  case 30: /* statement: assignment_statement  */
#line 216 "crunch.y"
                                 { (yyval. node ) = (yyvsp[0]. node ); }
#line 1683 "crunch.tab.cpp"
    break;

  case 31: /* statement: declaration_statement  */
#line 217 "crunch.y"
                          { (yyval. node ) = (yyvsp[0]. node ); }
#line 1689 "crunch.tab.cpp"
    break;

  case 32: /* statement: expression_statement  */
#line 218 "crunch.y"
                         { (yyval. node ) = (yyvsp[0]. node ); }
#line 1695 "crunch.tab.cpp"
    break;

  case 33: /* statement: return_statement  */
#line 219 "crunch.y"
                     { (yyval. node ) = (yyvsp[0]. node ); }
#line 1701 "crunch.tab.cpp"
    break;

  case 34: /* statement: if_statement  */
#line 220 "crunch.y"
                 { (yyval. node ) = (yyvsp[0]. node ); }
#line 1707 "crunch.tab.cpp"
    break;

  case 35: /* statement: break_statement  */
#line 221 "crunch.y"
                    { (yyval. node ) = (yyvsp[0]. node ); }
#line 1713 "crunch.tab.cpp"
    break;

  case 36: /* statement: switch_statement  */
#line 222 "crunch.y"
                     { (yyval. node ) = (yyvsp[0]. node ); }
#line 1719 "crunch.tab.cpp"
    break;

  case 37: /* statement: while_statement  */
#line 223 "crunch.y"
                    { (yyval. node ) = (yyvsp[0]. node ); }
#line 1725 "crunch.tab.cpp"
    break;

  case 38: /* assignment_statement: variable TOKEN_ASSIGN expression TOKEN_SEMICOLON  */
#line 226 "crunch.y"
                                                                        { (yyval. node ) = new crunch::language::SyntaxNodeAssignmentStatement( _st_params, (yyvsp[-3]. node ), (yyvsp[-1]. node ) ); }
#line 1731 "crunch.tab.cpp"
    break;

  case 39: /* assignment_statement: variable TOKEN_PLUS_EQUALS expression TOKEN_SEMICOLON  */
#line 227 "crunch.y"
                                                          { (yyval. node ) = new crunch::language::SyntaxNodeAssignmentStatement( _st_params, (yyvsp[-3]. node )->addReference(), new crunch::language::SyntaxNodeBinaryExpression( _st_params, (yyvsp[-3]. node ), _st_bop( PLUS ), (yyvsp[-1]. node ) ) ); }
#line 1737 "crunch.tab.cpp"
    break;

  case 40: /* assignment_statement: variable TOKEN_MINUS_EQUALS expression TOKEN_SEMICOLON  */
#line 228 "crunch.y"
                                                           { (yyval. node ) = new crunch::language::SyntaxNodeAssignmentStatement( _st_params, (yyvsp[-3]. node )->addReference(), new crunch::language::SyntaxNodeBinaryExpression( _st_params, (yyvsp[-3]. node ), _st_bop( MINUS ), (yyvsp[-1]. node ) ) ); }
#line 1743 "crunch.tab.cpp"
    break;

  case 41: /* assignment_statement: variable TOKEN_MULTIPLY_EQUALS expression TOKEN_SEMICOLON  */
#line 229 "crunch.y"
                                                              { (yyval. node ) = new crunch::language::SyntaxNodeAssignmentStatement( _st_params, (yyvsp[-3]. node )->addReference(), new crunch::language::SyntaxNodeBinaryExpression( _st_params, (yyvsp[-3]. node ), _st_bop( MULTIPLY ), (yyvsp[-1]. node ) ) ); }
#line 1749 "crunch.tab.cpp"
    break;

  case 42: /* assignment_statement: variable TOKEN_DIVIDE_EQUALS expression TOKEN_SEMICOLON  */
#line 230 "crunch.y"
                                                            { (yyval. node ) = new crunch::language::SyntaxNodeAssignmentStatement( _st_params, (yyvsp[-3]. node )->addReference(), new crunch::language::SyntaxNodeBinaryExpression( _st_params, (yyvsp[-3]. node ), _st_bop( DIVIDE ), (yyvsp[-1]. node ) ) ); }
#line 1755 "crunch.tab.cpp"
    break;

  case 43: /* declaration_statement: type_identifier TOKEN_ID TOKEN_ASSIGN expression TOKEN_SEMICOLON  */
#line 233 "crunch.y"
                                                                                         { (yyval. node ) = new crunch::language::SyntaxNodeDeclarationStatement( _st_params, (yyvsp[-4]. node ), *(yyvsp[-3]. text ), (yyvsp[-1]. node ) ); }
#line 1761 "crunch.tab.cpp"
    break;

  case 44: /* declaration_statement: type_identifier TOKEN_ID TOKEN_SEMICOLON  */
#line 234 "crunch.y"
                                             { (yyval. node ) = new crunch::language::SyntaxNodeDeclarationStatement( _st_params, (yyvsp[-2]. node ), *(yyvsp[-1]. text ) ); }
#line 1767 "crunch.tab.cpp"
    break;

  case 45: /* include_statement: TOKEN_RESERVED_INCLUDE TOKEN_STRING TOKEN_SEMICOLON  */
#line 237 "crunch.y"
                                                                        { (yyval. node ) = new crunch::language::SyntaxNodeInclude( _st_params, *(yyvsp[-1]. text ) ); }
#line 1773 "crunch.tab.cpp"
    break;

  case 46: /* expression_statement: expression TOKEN_SEMICOLON  */
#line 240 "crunch.y"
                                                  { (yyvsp[-1]. node )->setStatementContext( true ); (yyval. node ) = (yyvsp[-1]. node ); }
#line 1779 "crunch.tab.cpp"
    break;

  case 47: /* return_statement: TOKEN_RESERVED_RETURN TOKEN_SEMICOLON  */
#line 243 "crunch.y"
                                                         { (yyval. node ) = new crunch::language::SyntaxNodeReturnStatement( _st_params ); }
#line 1785 "crunch.tab.cpp"
    break;

  case 48: /* return_statement: TOKEN_RESERVED_RETURN expression TOKEN_SEMICOLON  */
#line 244 "crunch.y"
                                                     { (yyval. node ) = new crunch::language::SyntaxNodeReturnStatement( _st_params, (yyvsp[-1]. node ) ); }
#line 1791 "crunch.tab.cpp"
    break;

  case 49: /* if_statement: if_header statement_block  */
#line 247 "crunch.y"
                                         { (yyval. node ) = new crunch::language::SyntaxNodeIfStatement( _st_params, (yyvsp[-1]. node ), (yyvsp[0]. node ) ); }
#line 1797 "crunch.tab.cpp"
    break;

  case 50: /* if_statement: if_header statement_block TOKEN_RESERVED_ELSE statement_block  */
#line 248 "crunch.y"
                                                                  { (yyval. node ) = new crunch::language::SyntaxNodeIfStatement( _st_params, (yyvsp[-3]. node ), (yyvsp[-2]. node ), (yyvsp[0]. node ) ); }
#line 1803 "crunch.tab.cpp"
    break;

  case 51: /* if_header: TOKEN_RESERVED_IF TOKEN_OPEN_PARAN expression TOKEN_CLOSE_PARAN  */
#line 251 "crunch.y"
                                                                            { (yyval. node ) = (yyvsp[-1]. node ); }
#line 1809 "crunch.tab.cpp"
    break;

  case 52: /* while_statement: TOKEN_RESERVED_WHILE TOKEN_OPEN_PARAN expression TOKEN_CLOSE_PARAN statement_block  */
#line 254 "crunch.y"
                                                                                                     { (yyval. node ) = new crunch::language::SyntaxNodeWhileStatement( _st_params, (yyvsp[-2]. node ), (yyvsp[0]. node ) ); }
#line 1815 "crunch.tab.cpp"
    break;

  case 53: /* break_statement: TOKEN_RESERVED_BREAK TOKEN_SEMICOLON  */
#line 256 "crunch.y"
                                                       { (yyval. node ) = new crunch::language::SyntaxNodeBreakStatement( _st_params ); }
#line 1821 "crunch.tab.cpp"
    break;

  case 54: /* switch_statement: switch_header switch_body  */
#line 259 "crunch.y"
                                             { (yyval. node ) = (yyvsp[0]. node ); if ( (yyval. node ) == nullptr ) (yyval. node ) = new crunch::language::SyntaxNodeSwitchBody( _st_params ); (yyval. node )->append( (yyvsp[-1]. node ) ); }
#line 1827 "crunch.tab.cpp"
    break;

  case 55: /* switch_header: TOKEN_RESERVED_SWITCH TOKEN_OPEN_PARAN expression TOKEN_CLOSE_PARAN  */
#line 262 "crunch.y"
                                                                                    { (yyval. node ) = (yyvsp[-1]. node ); }
#line 1833 "crunch.tab.cpp"
    break;

  case 56: /* switch_body: TOKEN_OPEN_CURLY switch_case_list TOKEN_CLOSE_CURLY  */
#line 265 "crunch.y"
                                                                  { (yyval. node ) = (yyvsp[-1]. node ); }
#line 1839 "crunch.tab.cpp"
    break;

  case 57: /* switch_case_list: %empty  */
#line 268 "crunch.y"
                   { (yyval. node ) = nullptr; }
#line 1845 "crunch.tab.cpp"
    break;

  case 58: /* switch_case_list: switch_case_list switch_case  */
#line 269 "crunch.y"
                                 { (yyval. node ) = (yyvsp[-1]. node ); if ( (yyval. node ) == nullptr ) (yyval. node ) = new crunch::language::SyntaxNodeSwitchBody( _st_params ); (yyval. node )->append( (yyvsp[0]. node ) ); }
#line 1851 "crunch.tab.cpp"
    break;

  case 59: /* switch_case_list: switch_case_list switch_default  */
#line 270 "crunch.y"
                                    { (yyval. node ) = (yyvsp[-1]. node ); if ( (yyval. node ) == nullptr ) (yyval. node ) = new crunch::language::SyntaxNodeSwitchBody( _st_params ); (yyval. node )->append( (yyvsp[0]. node ) ); }
#line 1857 "crunch.tab.cpp"
    break;

  case 60: /* switch_case: TOKEN_RESERVED_CASE expression TOKEN_COLON statement_list  */
#line 273 "crunch.y"
                                                                        { (yyval. node ) = new crunch::language::SyntaxNodeSwitchCase( _st_params, (yyvsp[-2]. node ), (yyvsp[0]. node ) ); }
#line 1863 "crunch.tab.cpp"
    break;

  case 61: /* switch_default: TOKEN_RESERVED_DEFAULT TOKEN_COLON statement_list  */
#line 276 "crunch.y"
                                                                   { (yyval. node ) = new crunch::language::SyntaxNodeSwitchCase( _st_params, nullptr, (yyvsp[0]. node ) ); }
#line 1869 "crunch.tab.cpp"
    break;

  case 62: /* expression_list: %empty  */
#line 279 "crunch.y"
                  { (yyval. node ) = nullptr; }
#line 1875 "crunch.tab.cpp"
    break;

  case 63: /* expression_list: expression  */
#line 280 "crunch.y"
               { (yyval. node ) = new crunch::language::SyntaxNodeExpressionList( _st_params ); (yyval. node )->append( (yyvsp[0]. node ) ); }
#line 1881 "crunch.tab.cpp"
    break;

  case 64: /* expression_list: expression_list TOKEN_COMMA expression  */
#line 281 "crunch.y"
                                           { (yyval. node ) = (yyvsp[-2]. node ); if ( (yyval. node ) == nullptr ) (yyval. node ) = new crunch::language::SyntaxNodeExpressionList( _st_params ); (yyval. node )->append( (yyvsp[0]. node ) ); }
#line 1887 "crunch.tab.cpp"
    break;

  case 65: /* expression: literal_type  */
#line 284 "crunch.y"
                          { (yyval. node ) = new crunch::language::SyntaxNodeExpression( _st_params, (yyvsp[0]. node ) ); }
#line 1893 "crunch.tab.cpp"
    break;

  case 66: /* expression: expression TOKEN_EE expression  */
#line 285 "crunch.y"
                                   { (yyval. node ) = new crunch::language::SyntaxNodeBinaryExpression( _st_params, (yyvsp[-2]. node ), _st_bop( EE ), (yyvsp[0]. node ) ); }
#line 1899 "crunch.tab.cpp"
    break;

  case 67: /* expression: expression TOKEN_NE expression  */
#line 286 "crunch.y"
                                   { (yyval. node ) = new crunch::language::SyntaxNodeBinaryExpression( _st_params, (yyvsp[-2]. node ), _st_bop( NE ), (yyvsp[0]. node ) ); }
#line 1905 "crunch.tab.cpp"
    break;

  case 68: /* expression: expression TOKEN_GT expression  */
#line 287 "crunch.y"
                                   { (yyval. node ) = new crunch::language::SyntaxNodeBinaryExpression( _st_params, (yyvsp[-2]. node ), _st_bop( GT ), (yyvsp[0]. node ) ); }
#line 1911 "crunch.tab.cpp"
    break;

  case 69: /* expression: expression TOKEN_GE expression  */
#line 288 "crunch.y"
                                   { (yyval. node ) = new crunch::language::SyntaxNodeBinaryExpression( _st_params, (yyvsp[-2]. node ), _st_bop( GE ), (yyvsp[0]. node ) ); }
#line 1917 "crunch.tab.cpp"
    break;

  case 70: /* expression: expression TOKEN_LT expression  */
#line 289 "crunch.y"
                                   { (yyval. node ) = new crunch::language::SyntaxNodeBinaryExpression( _st_params, (yyvsp[-2]. node ), _st_bop( LT ), (yyvsp[0]. node ) ); }
#line 1923 "crunch.tab.cpp"
    break;

  case 71: /* expression: expression TOKEN_LE expression  */
#line 290 "crunch.y"
                                   { (yyval. node ) = new crunch::language::SyntaxNodeBinaryExpression( _st_params, (yyvsp[-2]. node ), _st_bop( LE ), (yyvsp[0]. node ) ); }
#line 1929 "crunch.tab.cpp"
    break;

  case 72: /* expression: expression TOKEN_PLUS expression  */
#line 291 "crunch.y"
                                     { (yyval. node ) = new crunch::language::SyntaxNodeBinaryExpression( _st_params, (yyvsp[-2]. node ), _st_bop( PLUS ), (yyvsp[0]. node ) ); }
#line 1935 "crunch.tab.cpp"
    break;

  case 73: /* expression: expression TOKEN_MINUS expression  */
#line 292 "crunch.y"
                                      { (yyval. node ) = new crunch::language::SyntaxNodeBinaryExpression( _st_params, (yyvsp[-2]. node ), _st_bop( MINUS ), (yyvsp[0]. node ) ); }
#line 1941 "crunch.tab.cpp"
    break;

  case 74: /* expression: expression TOKEN_MULTIPLY expression  */
#line 293 "crunch.y"
                                         { (yyval. node ) = new crunch::language::SyntaxNodeBinaryExpression( _st_params, (yyvsp[-2]. node ), _st_bop( MULTIPLY ), (yyvsp[0]. node ) ); }
#line 1947 "crunch.tab.cpp"
    break;

  case 75: /* expression: expression TOKEN_DIVIDE expression  */
#line 294 "crunch.y"
                                       { (yyval. node ) = new crunch::language::SyntaxNodeBinaryExpression( _st_params, (yyvsp[-2]. node ), _st_bop( DIVIDE ), (yyvsp[0]. node ) ); }
#line 1953 "crunch.tab.cpp"
    break;

  case 76: /* expression: expression TOKEN_BOOL_AND expression  */
#line 295 "crunch.y"
                                         { (yyval. node ) = new crunch::language::SyntaxNodeBinaryExpression( _st_params, (yyvsp[-2]. node ), _st_bop( BOOL_AND ), (yyvsp[0]. node ) ); }
#line 1959 "crunch.tab.cpp"
    break;

  case 77: /* expression: expression TOKEN_BOOL_OR expression  */
#line 296 "crunch.y"
                                        { (yyval. node ) = new crunch::language::SyntaxNodeBinaryExpression( _st_params, (yyvsp[-2]. node ), _st_bop( BOOL_OR ), (yyvsp[0]. node ) ); }
#line 1965 "crunch.tab.cpp"
    break;

  case 78: /* expression: TOKEN_MINUS expression  */
#line 297 "crunch.y"
                           { (yyval. node ) = new crunch::language::SyntaxNodeExpression( _st_params, (yyvsp[0]. node ), _st_op( MINUS ) ); }
#line 1971 "crunch.tab.cpp"
    break;

  case 79: /* expression: TOKEN_BOOL_NOT expression  */
#line 298 "crunch.y"
                              { (yyval. node ) = new crunch::language::SyntaxNodeExpression( _st_params, (yyvsp[0]. node ), _st_op( BOOL_NOT ) ); }
#line 1977 "crunch.tab.cpp"
    break;

  case 80: /* expression: expression TOKEN_BIT_AND expression  */
#line 299 "crunch.y"
                                        { (yyval. node ) = new crunch::language::SyntaxNodeBinaryExpression( _st_params, (yyvsp[-2]. node ), _st_bop( BIT_AND ), (yyvsp[0]. node ) ); }
#line 1983 "crunch.tab.cpp"
    break;

  case 81: /* expression: expression TOKEN_BIT_OR expression  */
#line 300 "crunch.y"
                                       { (yyval. node ) = new crunch::language::SyntaxNodeBinaryExpression( _st_params, (yyvsp[-2]. node ), _st_bop( BIT_OR ), (yyvsp[0]. node ) ); }
#line 1989 "crunch.tab.cpp"
    break;

  case 82: /* expression: TOKEN_BIT_COMP expression  */
#line 301 "crunch.y"
                              { (yyval. node ) = new crunch::language::SyntaxNodeExpression( _st_params, (yyvsp[0]. node ), _st_op( BIT_COMP ) ); }
#line 1995 "crunch.tab.cpp"
    break;

  case 83: /* expression: TOKEN_OPEN_PARAN expression TOKEN_CLOSE_PARAN  */
#line 302 "crunch.y"
                                                  { (yyval. node ) = (yyvsp[-1]. node ); }
#line 2001 "crunch.tab.cpp"
    break;

  case 84: /* expression: function_call  */
#line 303 "crunch.y"
                  { (yyval. node ) = new crunch::language::SyntaxNodeExpression( _st_params, (yyvsp[0]. node ) ); }
#line 2007 "crunch.tab.cpp"
    break;

  case 85: /* expression: variable  */
#line 304 "crunch.y"
             { (yyval. node ) = new crunch::language::SyntaxNodeExpression( _st_params, (yyvsp[0]. node ) ); }
#line 2013 "crunch.tab.cpp"
    break;

  case 86: /* literal_type: TOKEN_I64  */
#line 307 "crunch.y"
                         { (yyval. node ) = new crunch::language::SyntaxNodeLiteralType( _st_params, (yyvsp[0]. ival ) ); }
#line 2019 "crunch.tab.cpp"
    break;

  case 87: /* literal_type: TOKEN_U64  */
#line 308 "crunch.y"
              { (yyval. node ) = new crunch::language::SyntaxNodeLiteralType( _st_params, (yyvsp[0]. uval ) ); }
#line 2025 "crunch.tab.cpp"
    break;

  case 88: /* literal_type: TOKEN_F64  */
#line 309 "crunch.y"
              { (yyval. node ) = new crunch::language::SyntaxNodeLiteralType( _st_params, (yyvsp[0]. fval ) ); }
#line 2031 "crunch.tab.cpp"
    break;

  case 89: /* literal_type: TOKEN_STRING  */
#line 310 "crunch.y"
                 { (yyval. node ) = new crunch::language::SyntaxNodeLiteralType( _st_params, (yyvsp[0]. text ) ); }
#line 2037 "crunch.tab.cpp"
    break;

  case 90: /* literal_type: TOKEN_CHAR  */
#line 311 "crunch.y"
               { (yyval. node ) = new crunch::language::SyntaxNodeLiteralType( _st_params, (yyvsp[0]. cval ) ); }
#line 2043 "crunch.tab.cpp"
    break;

  case 91: /* literal_type: TOKEN_TRUE  */
#line 312 "crunch.y"
               { (yyval. node ) = new crunch::language::SyntaxNodeLiteralType( _st_params, (yyvsp[0]. ival ) ); }
#line 2049 "crunch.tab.cpp"
    break;

  case 92: /* literal_type: TOKEN_FALSE  */
#line 313 "crunch.y"
                { (yyval. node ) = new crunch::language::SyntaxNodeLiteralType( _st_params, (yyvsp[0]. ival ) ); }
#line 2055 "crunch.tab.cpp"
    break;

  case 93: /* function_call: type_identifier TOKEN_OPEN_PARAN expression_list TOKEN_CLOSE_PARAN  */
#line 316 "crunch.y"
                                                                                   { (yyval. node ) = new crunch::language::SyntaxNodeFunctionCall( _st_params, (yyvsp[-3]. node ), (yyvsp[-1]. node ) ); }
#line 2061 "crunch.tab.cpp"
    break;


#line 2065 "crunch.tab.cpp"

      default: break;
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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == CRUNCHEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= CRUNCHEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == CRUNCHEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = CRUNCHEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

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


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != CRUNCHEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 319 "crunch.y"


void crunchParseFile( const crunch::core::WideString& filepath, crunch::language::ScriptCompiler *compiler, crunch::language::SyntaxNodeRoot *root )
{
  if ( compiler == nullptr )
    return;

  if ( root == nullptr )
  {
    compiler->reportInternalError( L"", 0, "missing root node" );
    return;
  }

  sCompiler = compiler;
  sRoot = root;

  root->setSourceFilePath( filepath );

  FILE *source = core_fwopen( filepath.wc_str(), L"rt" );
  if ( source == nullptr )
  {
    compiler->reportError( L"", 0, "failed to open '%s'", filepath.toString().c_str() );
    return;
  }

  crunchrestart( source );

  crunchlineno = 1;

  do
  {
    crunchparse();
  } while ( !( feof( crunchin ) ) );

  fclose( source );
}

void crunchParseString( const crunch::core::String& text, crunch::language::ScriptCompiler *compiler, crunch::language::SyntaxNodeRoot *root )
{
  if ( compiler == nullptr )
    return;

  if ( root == nullptr )
  {
    compiler->reportInternalError( L"", 0, "missing root node" );
    return;
  }

  sCompiler = compiler;
  sRoot = root;

  YY_BUFFER_STATE b = crunch_scan_string( text.c_str() );

  crunchlineno = 1;

  crunchparse();

  crunch_delete_buffer( b );
}

void cruncherror( const char *s )
{
  sCompiler->reportError( sRoot->getSourceFilePath(), crunchlineno > 0 ? (unsigned int)( crunchlineno ) : 0, s );
}
