
%option prefix="crunch"
%option noyywrap
%option yylineno

%{

#include "../crunch_global.h"

#include "syntax tree/crunch_syntax_nodes.h"
#include "syntax tree/crunch_syntax_node_root.h"

#include "crunch_string_table.h"
#include "crunch_script_compiler.h"

#include <stdio.h>

#define CRUNCH_DECL int crunchlex()

#include "crunch.tab.h"

// #define crunch_return_token( _tok ) corePrint( "token: " #_tok ); appendCurrentScanLine(); return _tok
#define crunch_return_token( _tok ) appendCurrentScanLine(); return _tok

crunch::core::String* crunchStringPoolGet( const char *v, bool literalString );

void crunchClearCurrentScanLine();
void appendCurrentScanLine();
void reportCurrentScanLine();

extern crunch::language::ScriptCompiler* getParseCompiler();
extern crunch::language::SyntaxNodeRoot* getParseRoot();
extern unsigned int getParseLineNumber();

%}

%x MLINECOMMENT

%%

"/*"                { BEGIN( MLINECOMMENT ); }
<MLINECOMMENT>"*/"  { BEGIN( INITIAL ); }
<MLINECOMMENT>\n    { }
<MLINECOMMENT>.     { }

[ \t\r]	    { appendCurrentScanLine(); } // ignore whitespace

\n          { reportCurrentScanLine(); } // ignore, needed for yylineno

"//".*      ; // ignore line comments

"#include".* ; // ignore c-style #include, useful for informing IDE of meta info for library api

string      { crunch_return_token( TOKEN_RESERVED_STRING ); }
char        { crunch_return_token( TOKEN_RESERVED_CHAR ); }
i64         { crunch_return_token( TOKEN_RESERVED_I64 ); }
u64         { crunch_return_token( TOKEN_RESERVED_U64 ); }
f64         { crunch_return_token( TOKEN_RESERVED_F64 ); }
void        { crunch_return_token( TOKEN_RESERVED_VOID ); }
bool        { crunch_return_token( TOKEN_RESERVED_BOOL ); }

return      { crunch_return_token( TOKEN_RESERVED_RETURN ); }
include     { crunch_return_token( TOKEN_RESERVED_INCLUDE ); }

if          { crunch_return_token( TOKEN_RESERVED_IF ); }
else        { crunch_return_token( TOKEN_RESERVED_ELSE ); }

switch      { crunch_return_token( TOKEN_RESERVED_SWITCH ); }
case        { crunch_return_token( TOKEN_RESERVED_CASE ); }
break       { crunch_return_token( TOKEN_RESERVED_BREAK ); }
default     { crunch_return_token( TOKEN_RESERVED_DEFAULT ); }

while       { crunch_return_token( TOKEN_RESERVED_WHILE ); }

0x[0-9a-fA-F]+  { crunchlval.uval = crunch::core::String::hextou( crunch::core::String( crunchtext ) ); crunch_return_token( TOKEN_U64 ); }

[0-9]+\.[0-9]+  { crunchlval.fval = (crunch::language::crunch_f64)( atof( crunchtext ) ); crunch_return_token( TOKEN_F64 ); }
[0-9]+          { crunchlval.ival = (crunch::language::crunch_i64)( atoi( crunchtext ) ); crunch_return_token( TOKEN_I64 ); }
[0-9]+u         { crunchlval.uval = (crunch::language::crunch_u64)( crunch::core::String::to_u64( crunch::core::String( crunchtext ) ) ); crunch_return_token( TOKEN_U64 ); }

[_]*[a-zA-Z]+[a-zA-Z_0-9]*      { crunchlval.text = crunchStringPoolGet( crunchtext, false ); crunch_return_token( TOKEN_ID ); }

\"[^\"\n\r]*\"                  { crunchlval.text = crunchStringPoolGet( crunchtext, true ); crunch_return_token( TOKEN_STRING ); }
\'[^\"\n\r]\'                   { crunchlval.text = crunchStringPoolGet( crunchtext, true ); crunch_return_token( TOKEN_CHAR ); }

";"   { crunch_return_token( TOKEN_SEMICOLON ); }
","   { crunch_return_token( TOKEN_COMMA ); }
":"   { crunch_return_token( TOKEN_COLON ); }
"."   { crunch_return_token( TOKEN_DOT ); }

"|"   { crunch_return_token( TOKEN_BIT_OR ); }
"&"   { crunch_return_token( TOKEN_BIT_AND ); }
"~"   { crunch_return_token( TOKEN_BIT_COMP ); }

"&&"  { crunch_return_token( TOKEN_BOOL_AND ); }
"||"  { crunch_return_token( TOKEN_BOOL_OR ); }
"!"   { crunch_return_token( TOKEN_BOOL_NOT ); }

">"   { crunch_return_token( TOKEN_GT ); }
">="  { crunch_return_token( TOKEN_GE ); }
"<"   { crunch_return_token( TOKEN_LT ); }
"<="  { crunch_return_token( TOKEN_LE ); }
"=="  { crunch_return_token( TOKEN_EE ); }
"!="  { crunch_return_token( TOKEN_NE ); }

"="   { crunch_return_token( TOKEN_ASSIGN ); }

"+"   { crunch_return_token( TOKEN_PLUS ); }
"-"   { crunch_return_token( TOKEN_MINUS ); }
"*"   { crunch_return_token( TOKEN_MULTIPLY ); }
"/"   { crunch_return_token( TOKEN_DIVIDE ); }

"("   { crunch_return_token( TOKEN_OPEN_PARAN ); }
")"   { crunch_return_token( TOKEN_CLOSE_PARAN ); }

"{"   { crunch_return_token( TOKEN_OPEN_CURLY ); }
"}"   { crunch_return_token( TOKEN_CLOSE_CURLY ); }

"["   { crunch_return_token( TOKEN_OPEN_BRACKET ); }
"]"   { crunch_return_token( TOKEN_CLOSE_BRACKET ); }

%%

static crunch::language::StringTable *mStringTable = nullptr;

crunch::core::String* crunchStringPoolGet( const char *v, bool literalString )
{
  return mStringTable->mergeString( v, literalString );
}

void crunchStringPoolDestroy()
{
  if ( mStringTable != nullptr )
  {
    delete mStringTable;
    mStringTable = nullptr;
  }
}

void crunchStringPoolCreate()
{
  crunchStringPoolDestroy();
  mStringTable = new crunch::language::StringTable();
}

static crunch::core::String mCurrentScanLine;

void crunchClearCurrentScanLine()
{
  mCurrentScanLine = "";
}

void appendCurrentScanLine()
{
  mCurrentScanLine += crunch::core::String( crunchtext );
}

void reportCurrentScanLine()
{
  getParseCompiler()->reportCurrentScanLine( getParseRoot()->getSourceFilePath(), getParseLineNumber() - 1, mCurrentScanLine );
  mCurrentScanLine = "";
}
