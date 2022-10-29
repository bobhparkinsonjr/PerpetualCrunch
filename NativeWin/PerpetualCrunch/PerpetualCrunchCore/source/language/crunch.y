
%define api.prefix {crunch}

%{

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

%}

%union
{
  const crunch::language::crunch_string *text;
  crunch::language::crunch_char cval;
	crunch::language::crunch_i64 ival;
	crunch::language::crunch_f64 fval;
  crunch::language::crunch_u64 uval;
  crunch::language::SyntaxNode *node;
}

%expect 1

%token< text > TOKEN_ID

%token TOKEN_RESERVED_STRING
%token TOKEN_RESERVED_CHAR
%token TOKEN_RESERVED_I64
%token TOKEN_RESERVED_U64
%token TOKEN_RESERVED_F64
%token TOKEN_RESERVED_VOID
%token TOKEN_RESERVED_BOOL

%token TOKEN_RESERVED_RETURN
%token TOKEN_RESERVED_INCLUDE

%token TOKEN_RESERVED_IF
%token TOKEN_RESERVED_ELSE

%token TOKEN_RESERVED_SWITCH
%token TOKEN_RESERVED_CASE
%token TOKEN_RESERVED_BREAK
%token TOKEN_RESERVED_DEFAULT

%token TOKEN_RESERVED_WHILE

%token TOKEN_SEMICOLON
%token TOKEN_COMMA
%token TOKEN_COLON
%token TOKEN_DOT

%token TOKEN_BIT_OR
%token TOKEN_BIT_AND
%token TOKEN_BIT_COMP

%token TOKEN_BOOL_AND
%token TOKEN_BOOL_OR
%token TOKEN_BOOL_NOT

%token TOKEN_GT
%token TOKEN_GE
%token TOKEN_LT
%token TOKEN_LE
%token TOKEN_EE
%token TOKEN_NE

%token TOKEN_ASSIGN

%token< ival > TOKEN_I64
%token< uval > TOKEN_U64
%token< fval > TOKEN_F64
%token< text > TOKEN_STRING
%token< cval > TOKEN_CHAR
%token< ival > TOKEN_TRUE
%token< ival > TOKEN_FALSE

%token TOKEN_PLUS TOKEN_MINUS TOKEN_MULTIPLY TOKEN_DIVIDE TOKEN_OPEN_PARAN TOKEN_CLOSE_PARAN TOKEN_OPEN_CURLY TOKEN_CLOSE_CURLY TOKEN_OPEN_BRACKET TOKEN_CLOSE_BRACKET
%left TOKEN_BIT_COMP
%left TOKEN_BIT_AND TOKEN_BIT_OR
%left TOKEN_BOOL_NOT
%left TOKEN_BOOL_AND TOKEN_BOOL_OR
%left TOKEN_PLUS TOKEN_MINUS
%left TOKEN_MULTIPLY TOKEN_DIVIDE
%left TOKEN_EE TOKEN_NE
%left TOKEN_GT TOKEN_GE TOKEN_LT TOKEN_LE
%left TOKEN_DOT
%left TOKEN_OPEN_BRACKET TOKEN_CLOSE_BRACKET
%left TOKEN_OPEN_PARAN TOKEN_CLOSE_PARAN

%type< node > root
%type< node > root_object
%type< node > include_statement
%type< node > declaration_statement
%type< node > type_identifier
%type< node > basic_type
%type< node > expression
%type< node > literal_type
%type< node > statement
%type< node > statement_list
%type< node > function_body
%type< node > function_parameter
%type< node > function_parameter_list
%type< node > function_declaration
%type< node > function_call
%type< node > expression_list
%type< node > return_statement
%type< node > variable
%type< node > assignment_statement
%type< node > switch_header
%type< node > switch_case
%type< node > switch_case_list
%type< node > switch_default
%type< node > switch_body
%type< node > switch_statement
%type< node > expression_statement
%type< node > if_header
%type< node > if_statement
%type< node > break_statement
%type< node > statement_block
%type< node > while_statement

%start root

%%

root : {}
  | root root_object { sRoot->append( $2 ); }
  ;

root_object : function_declaration { $$ = $1; }
  | include_statement { $$ = $1; }
  ;

function_declaration : type_identifier TOKEN_ID TOKEN_OPEN_PARAN function_parameter_list TOKEN_CLOSE_PARAN function_body { $$ = new crunch::language::SyntaxNodeFunctionDeclaration( _st_params, $1, *$2, $4, $6 ); }
  ;

function_body : TOKEN_OPEN_CURLY TOKEN_CLOSE_CURLY { $$ = nullptr; }
  | TOKEN_OPEN_CURLY statement_list TOKEN_CLOSE_CURLY { $$ = $2; }
  ;

variable : TOKEN_ID { $$ = new crunch::language::SyntaxNodeVariable( _st_params, *$1 ); }
  | expression TOKEN_DOT expression { $$ = new crunch::language::SyntaxNodeVariable( _st_params, $1, $3, crunch::language::SyntaxNodeVariable::SubType::SUB_TYPE_MEMBER ); }
  | expression TOKEN_OPEN_BRACKET expression TOKEN_CLOSE_BRACKET { $$ = new crunch::language::SyntaxNodeVariable( _st_params, $1, $3, crunch::language::SyntaxNodeVariable::SubType::SUB_TYPE_SUBSCRIPT ); }
  ;

type_identifier : TOKEN_ID { $$ = new crunch::language::SyntaxNodeTypeIdentifier( _st_params, crunch::language::ScriptTypes::ScriptType::TYPE_NONE, *$1 ); }
  | basic_type { $$ = $1; }
  ;

basic_type : TOKEN_RESERVED_STRING { $$ = new crunch::language::SyntaxNodeTypeIdentifier( _st_params, crunch::language::ScriptTypes::ScriptType::TYPE_STRING ); }
  | TOKEN_RESERVED_CHAR { $$ = new crunch::language::SyntaxNodeTypeIdentifier( _st_params, crunch::language::ScriptTypes::ScriptType::TYPE_CHAR ); }
  | TOKEN_RESERVED_I64 { $$ = new crunch::language::SyntaxNodeTypeIdentifier( _st_params, crunch::language::ScriptTypes::ScriptType::TYPE_I64 ); }
  | TOKEN_RESERVED_U64 { $$ = new crunch::language::SyntaxNodeTypeIdentifier( _st_params, crunch::language::ScriptTypes::ScriptType::TYPE_U64 ); }
  | TOKEN_RESERVED_F64 { $$ = new crunch::language::SyntaxNodeTypeIdentifier( _st_params, crunch::language::ScriptTypes::ScriptType::TYPE_F64 ); }
  | TOKEN_RESERVED_VOID { $$ = new crunch::language::SyntaxNodeTypeIdentifier( _st_params, crunch::language::ScriptTypes::ScriptType::TYPE_VOID ); }
  | TOKEN_RESERVED_BOOL { $$ = new crunch::language::SyntaxNodeTypeIdentifier( _st_params, crunch::language::ScriptTypes::ScriptType::TYPE_I64 ); }
  ;

function_parameter_list : { $$ = nullptr; }
  | function_parameter { $$ = new crunch::language::SyntaxNodeFunctionParameterList( _st_params ); $$->append( $1 ); }
  | function_parameter_list TOKEN_COMMA function_parameter { $$ = $1; $$->append( $3 ); }
  ;

function_parameter : type_identifier TOKEN_ID { $$ = new crunch::language::SyntaxNodeFunctionParameter( _st_params, $1, *$2 ); }
  ;

statement_block : statement { $$ = $1; }
  | TOKEN_OPEN_CURLY TOKEN_CLOSE_CURLY { $$ = nullptr; }
  | TOKEN_OPEN_CURLY statement_list TOKEN_CLOSE_CURLY { ( (crunch::language::SyntaxNodeStatementList*)( $2 ) )->setScoped(); $$ = $2; }
  ;

statement_list : statement { $$ = new crunch::language::SyntaxNodeStatementList( _st_params ); $$->append( $1 ); }
  | statement_list statement { $$ = $1; $$->append( $2 ); }
  ;

statement : assignment_statement { $$ = $1; }
  | declaration_statement { $$ = $1; }
  | expression_statement { $$ = $1; }
  | return_statement { $$ = $1; }
  | if_statement { $$ = $1; }
  | break_statement { $$ = $1; }
  | switch_statement { $$ = $1; }
  | while_statement { $$ = $1; }
  ;

assignment_statement : variable TOKEN_ASSIGN expression TOKEN_SEMICOLON { $$ = new crunch::language::SyntaxNodeAssignmentStatement( _st_params, $1, $3 ); }
  ;

declaration_statement : type_identifier TOKEN_ID TOKEN_ASSIGN expression TOKEN_SEMICOLON { $$ = new crunch::language::SyntaxNodeDeclarationStatement( _st_params, $1, *$2, $4 ); }
  | type_identifier TOKEN_ID TOKEN_SEMICOLON { $$ = new crunch::language::SyntaxNodeDeclarationStatement( _st_params, $1, *$2 ); }
  ;

include_statement : TOKEN_RESERVED_INCLUDE TOKEN_STRING TOKEN_SEMICOLON { $$ = new crunch::language::SyntaxNodeInclude( _st_params, *$2 ); }
  ;

expression_statement : expression TOKEN_SEMICOLON { $1->setStatementContext( true ); $$ = $1; }
  ;

return_statement : TOKEN_RESERVED_RETURN TOKEN_SEMICOLON { $$ = new crunch::language::SyntaxNodeReturnStatement( _st_params ); }
  | TOKEN_RESERVED_RETURN expression TOKEN_SEMICOLON { $$ = new crunch::language::SyntaxNodeReturnStatement( _st_params, $2 ); }
  ;

if_statement : if_header statement_block { $$ = new crunch::language::SyntaxNodeIfStatement( _st_params, $1, $2 ); }
  | if_header statement_block TOKEN_RESERVED_ELSE statement_block { $$ = new crunch::language::SyntaxNodeIfStatement( _st_params, $1, $2, $4 ); }
  ;

if_header : TOKEN_RESERVED_IF TOKEN_OPEN_PARAN expression TOKEN_CLOSE_PARAN { $$ = $3; }
  ;

while_statement : TOKEN_RESERVED_WHILE TOKEN_OPEN_PARAN expression TOKEN_CLOSE_PARAN statement_block { $$ = new crunch::language::SyntaxNodeWhileStatement( _st_params, $3, $5 ); }

break_statement : TOKEN_RESERVED_BREAK TOKEN_SEMICOLON { $$ = new crunch::language::SyntaxNodeBreakStatement( _st_params ); }
  ;

switch_statement : switch_header switch_body { $$ = $2; if ( $$ == nullptr ) $$ = new crunch::language::SyntaxNodeSwitchBody( _st_params ); $$->append( $1 ); }
  ;

switch_header : TOKEN_RESERVED_SWITCH TOKEN_OPEN_PARAN expression TOKEN_CLOSE_PARAN { $$ = $3; }
  ;

switch_body : TOKEN_OPEN_CURLY switch_case_list TOKEN_CLOSE_CURLY { $$ = $2; }
  ;

switch_case_list : { $$ = nullptr; }
  | switch_case_list switch_case { $$ = $1; if ( $$ == nullptr ) $$ = new crunch::language::SyntaxNodeSwitchBody( _st_params ); $$->append( $2 ); }
  | switch_case_list switch_default { $$ = $1; if ( $$ == nullptr ) $$ = new crunch::language::SyntaxNodeSwitchBody( _st_params ); $$->append( $2 ); }
  ;

switch_case : TOKEN_RESERVED_CASE expression TOKEN_COLON statement_list { $$ = new crunch::language::SyntaxNodeSwitchCase( _st_params, $2, $4 ); }
  ;

switch_default : TOKEN_RESERVED_DEFAULT TOKEN_COLON statement_list { $$ = new crunch::language::SyntaxNodeSwitchCase( _st_params, nullptr, $3 ); }
  ;

expression_list : { $$ = nullptr; }
  | expression { $$ = new crunch::language::SyntaxNodeExpressionList( _st_params ); $$->append( $1 ); }
  | expression_list TOKEN_COMMA expression { $$ = $1; if ( $$ == nullptr ) $$ = new crunch::language::SyntaxNodeExpressionList( _st_params ); $$->append( $3 ); }
  ;

expression : literal_type { $$ = new crunch::language::SyntaxNodeExpression( _st_params, $1 ); }
  | expression TOKEN_EE expression { $$ = new crunch::language::SyntaxNodeBinaryExpression( _st_params, $1, crunch::language::SyntaxNodeBinaryExpression::Operator::OPERATOR_EE, $3 ); }
  | expression TOKEN_NE expression { $$ = new crunch::language::SyntaxNodeBinaryExpression( _st_params, $1, crunch::language::SyntaxNodeBinaryExpression::Operator::OPERATOR_NE, $3 ); }
  | expression TOKEN_GT expression { $$ = new crunch::language::SyntaxNodeBinaryExpression( _st_params, $1, crunch::language::SyntaxNodeBinaryExpression::Operator::OPERATOR_GT, $3 ); }
  | expression TOKEN_GE expression { $$ = new crunch::language::SyntaxNodeBinaryExpression( _st_params, $1, crunch::language::SyntaxNodeBinaryExpression::Operator::OPERATOR_GE, $3 ); }
  | expression TOKEN_LT expression { $$ = new crunch::language::SyntaxNodeBinaryExpression( _st_params, $1, crunch::language::SyntaxNodeBinaryExpression::Operator::OPERATOR_LT, $3 ); }
  | expression TOKEN_LE expression { $$ = new crunch::language::SyntaxNodeBinaryExpression( _st_params, $1, crunch::language::SyntaxNodeBinaryExpression::Operator::OPERATOR_LE, $3 ); }
  | expression TOKEN_PLUS expression { $$ = new crunch::language::SyntaxNodeBinaryExpression( _st_params, $1, crunch::language::SyntaxNodeBinaryExpression::Operator::OPERATOR_PLUS, $3 ); }
  | expression TOKEN_MINUS expression { $$ = new crunch::language::SyntaxNodeBinaryExpression( _st_params, $1, crunch::language::SyntaxNodeBinaryExpression::Operator::OPERATOR_MINUS, $3 ); }
  | expression TOKEN_MULTIPLY expression { $$ = new crunch::language::SyntaxNodeBinaryExpression( _st_params, $1, crunch::language::SyntaxNodeBinaryExpression::Operator::OPERATOR_MULTIPLY, $3 ); }
  | expression TOKEN_DIVIDE expression { $$ = new crunch::language::SyntaxNodeBinaryExpression( _st_params, $1, crunch::language::SyntaxNodeBinaryExpression::Operator::OPERATOR_DIVIDE, $3 ); }
  | expression TOKEN_BOOL_AND expression { $$ = new crunch::language::SyntaxNodeBinaryExpression( _st_params, $1, crunch::language::SyntaxNodeBinaryExpression::Operator::OPERATOR_BOOL_AND, $3 ); }
  | expression TOKEN_BOOL_OR expression { $$ = new crunch::language::SyntaxNodeBinaryExpression( _st_params, $1, crunch::language::SyntaxNodeBinaryExpression::Operator::OPERATOR_BOOL_OR, $3 ); }
  | TOKEN_MINUS expression { $$ = new crunch::language::SyntaxNodeExpression( _st_params, $2, crunch::language::SyntaxNodeExpression::Operator::OPERATOR_MINUS ); }
  | TOKEN_BOOL_NOT expression { $$ = new crunch::language::SyntaxNodeExpression( _st_params, $2, crunch::language::SyntaxNodeExpression::Operator::OPERATOR_BOOL_NOT ); }
  | expression TOKEN_BIT_AND expression { $$ = new crunch::language::SyntaxNodeBinaryExpression( _st_params, $1, crunch::language::SyntaxNodeBinaryExpression::Operator::OPERATOR_BIT_AND, $3 ); }
  | expression TOKEN_BIT_OR expression { $$ = new crunch::language::SyntaxNodeBinaryExpression( _st_params, $1, crunch::language::SyntaxNodeBinaryExpression::Operator::OPERATOR_BIT_OR, $3 ); }
  | TOKEN_BIT_COMP expression { $$ = new crunch::language::SyntaxNodeExpression( _st_params, $2, crunch::language::SyntaxNodeExpression::Operator::OPERATOR_BIT_COMP ); }
  | TOKEN_OPEN_PARAN expression TOKEN_CLOSE_PARAN { $$ = $2; }
  | function_call { $$ = new crunch::language::SyntaxNodeExpression( _st_params, $1 ); }
  | variable { $$ = new crunch::language::SyntaxNodeExpression( _st_params, $1 ); }
  ;

literal_type : TOKEN_I64 { $$ = new crunch::language::SyntaxNodeLiteralType( _st_params, $1 ); }
  | TOKEN_U64 { $$ = new crunch::language::SyntaxNodeLiteralType( _st_params, $1 ); }
  | TOKEN_F64 { $$ = new crunch::language::SyntaxNodeLiteralType( _st_params, $1 ); }
  | TOKEN_STRING { $$ = new crunch::language::SyntaxNodeLiteralType( _st_params, $1 ); }
  | TOKEN_CHAR { $$ = new crunch::language::SyntaxNodeLiteralType( _st_params, $1 ); }
  | TOKEN_TRUE { $$ = new crunch::language::SyntaxNodeLiteralType( _st_params, $1 ); }
  | TOKEN_FALSE { $$ = new crunch::language::SyntaxNodeLiteralType( _st_params, $1 ); }
  ;

function_call : type_identifier TOKEN_OPEN_PARAN expression_list TOKEN_CLOSE_PARAN { $$ = new crunch::language::SyntaxNodeFunctionCall( _st_params, $1, $3 ); }
  ;

%%

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
