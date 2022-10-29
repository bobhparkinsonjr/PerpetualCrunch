
#include "../../crunch_global.h"

#include "../crunch_script_compiler.h"

#include "crunch_syntax_node_literal_type.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

SyntaxNodeLiteralType::SyntaxNodeLiteralType( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber ) :
  SyntaxNode( compiler, root, lineNumber ),
  mType( LiteralType::LITERAL_NONE )
{
  mValue.i = 0;
}

SyntaxNodeLiteralType::SyntaxNodeLiteralType( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber, crunch_i64 i ) :
  SyntaxNode( compiler, root, lineNumber ),
  mType( LiteralType::LITERAL_I64 )
{
  mValue.i = i;
}

SyntaxNodeLiteralType::SyntaxNodeLiteralType( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber, crunch_u64 u ) :
  SyntaxNode( compiler, root, lineNumber ),
  mType( LiteralType::LITERAL_U64 )
{
  mValue.u = u;
}

SyntaxNodeLiteralType::SyntaxNodeLiteralType( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber, crunch_f64 f ) :
  SyntaxNode( compiler, root, lineNumber ),
  mType( LiteralType::LITERAL_F64 )
{
  mValue.f = f;
}

SyntaxNodeLiteralType::SyntaxNodeLiteralType( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber, const crunch_string *s ) :
  SyntaxNode( compiler, root, lineNumber ),
  mType( LiteralType::LITERAL_STRING )
{
  mValue.s = s;
}

SyntaxNodeLiteralType::SyntaxNodeLiteralType( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber, char c ) :
  SyntaxNode( compiler, root, lineNumber ),
  mType( LiteralType::LITERAL_CHAR )
{
  mValue.c = crunch_char( c );
}

SyntaxNodeLiteralType::SyntaxNodeLiteralType( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber, const ScriptValue& v ) :
  SyntaxNode( compiler, root, lineNumber )
{
  switch ( v.getType() )
  {
    case ScriptTypes::ScriptType::TYPE_STRING:
      mType = LiteralType::LITERAL_STRING;
      mValue.s = v.getString();
    break;

    case ScriptTypes::ScriptType::TYPE_CHAR:
      mType = LiteralType::LITERAL_CHAR;
      mValue.c = v.getChar();
    break;

    case ScriptTypes::ScriptType::TYPE_I64:
      mType = LiteralType::LITERAL_I64;
      mValue.i = v.getI64();
    break;

    case ScriptTypes::ScriptType::TYPE_U64:
      mType = LiteralType::LITERAL_U64;
      mValue.u = v.getU64();
    break;

    case ScriptTypes::ScriptType::TYPE_F64:
      mType = LiteralType::LITERAL_F64;
      mValue.f = v.getF64();
    break;

    default:
      reportInternalError( "unallowed or unknown type for literal node" );
    break;
  }
}

SyntaxNodeLiteralType::~SyntaxNodeLiteralType()
{
  // empty
}

crunch::core::String SyntaxNodeLiteralType::getSyntaxTreeDescription( const crunch::core::String& indent ) const
{
  crunch::core::String r = crunch::core::String::fmt( "%s%u: LITERAL TYPE ", indent.c_str(), mLineNumber );

  switch ( mType )
  {
    case LiteralType::LITERAL_I64:
      r += crunch::core::String::i64tos( mValue.i );
    break;

    case LiteralType::LITERAL_U64:
      r += ( crunch::core::String::u64tos( mValue.u ) + "u" );
    break;

    case LiteralType::LITERAL_F64:
      r += ( crunch::core::String::dtos( mValue.f ) + "f" );
    break;

    case LiteralType::LITERAL_STRING:
      r += ( "\"" + *( mValue.s ) + "\"" );
    break;

    case LiteralType::LITERAL_CHAR:
      r += crunch::core::String::fmt( "\'%c\'", char( mValue.c ) );
    break;

    default:
      // empty
    break;
  }

  return ( r + "\n" );
}

crunch::core::String SyntaxNodeLiteralType::getSourceDescription() const
{
  crunch::core::String r;

  switch ( mType )
  {
    case LiteralType::LITERAL_I64:
      r = crunch::core::String::i64tos( mValue.i );
    break;

    case LiteralType::LITERAL_U64:
      r = ( crunch::core::String::u64tos( mValue.u ) + "u" );
    break;

    case LiteralType::LITERAL_F64:
      r = crunch::core::String::dtos( mValue.f );
    break;

    case LiteralType::LITERAL_STRING:
      r = ( "\"" + *( mValue.s ) + "\"" );
    break;

    case LiteralType::LITERAL_CHAR:
      r = crunch::core::String::fmt( "\'%c\'", char( mValue.c ) );
    break;

    default:
      // empty
    break;
  }

  return r;
}

bool SyntaxNodeLiteralType::generateCode( crunch::core::BinaryBuffer *dest )
{
  if ( mCompiler->getContextStack()->getContext() == ScriptContextStack::Context::FETCH )
    return getScriptValue().generateCode( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() );

  reportInternalError( "invalid script context for literal" );
  return false;
}

ScriptTypes::ScriptTypeDescription SyntaxNodeLiteralType::getScriptTypeDescription() const
{
  switch ( mType )
  {
    case LiteralType::LITERAL_I64:
      return ScriptTypes::ScriptTypeDescription( ScriptTypes::ScriptType::TYPE_I64 );

    case LiteralType::LITERAL_U64:
      return ScriptTypes::ScriptTypeDescription( ScriptTypes::ScriptType::TYPE_U64 );

    case LiteralType::LITERAL_F64:
      return ScriptTypes::ScriptTypeDescription( ScriptTypes::ScriptType::TYPE_F64 );

    case LiteralType::LITERAL_STRING:
      return ScriptTypes::ScriptTypeDescription( ScriptTypes::ScriptType::TYPE_STRING );

    case LiteralType::LITERAL_CHAR:
      return ScriptTypes::ScriptTypeDescription( ScriptTypes::ScriptType::TYPE_CHAR );

    default:
      // empty
    break;
  }
  
  return ScriptTypes::ScriptTypeDescription();
}

ScriptValue SyntaxNodeLiteralType::getScriptValue() const
{
  switch ( mType )
  {
    case LiteralType::LITERAL_I64:
      return ScriptValue( mValue.i );

    case LiteralType::LITERAL_U64:
      return ScriptValue( mValue.u );

    case LiteralType::LITERAL_F64:
      return ScriptValue( mValue.f );

    case LiteralType::LITERAL_STRING:
      return ScriptValue( mCompiler, mValue.s );

    case LiteralType::LITERAL_CHAR:
      return ScriptValue( char( mValue.c ) );

    default:
      // empty
    break;
  }
  
  return ScriptValue();
}

} }
