
#include "../crunch_global.h"

#include "crunch_script_compiler.h"
#include "crunch_script_instruction.h"
#include "crunch_script_executable.h"

#include "crunch_script_value.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

ScriptValue::ScriptValue()
{
  mValue.i = 0;
  mType = ScriptTypes::ScriptType::TYPE_NONE;
}

ScriptValue::ScriptValue( ScriptCompiler *compiler, ScriptTypes::ScriptType t )
{
  mValue.i = 0;
  mType = t;

  switch ( mType )
  {
    case ScriptTypes::ScriptType::TYPE_STRING:
      mValue.s = compiler->stringTableMergeString( "", false );
    break;

    case ScriptTypes::ScriptType::TYPE_I64:
      mValue.i = 0;
    break;

    case ScriptTypes::ScriptType::TYPE_U64:
      mValue.u = 0;
    break;

    case ScriptTypes::ScriptType::TYPE_F64:
      mValue.f = 0.0;
    break;

    case ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS:
      // TODO
      // NOTE: compiler/vm both expected to have same architecture (eg. 64 bit)
      mValue.p = nullptr;
    break;

    default:
      // empty
    break;
  }
}

ScriptValue::ScriptValue( crunch_i64 i )
{
  mValue.i = i;
  mType = ScriptTypes::ScriptType::TYPE_I64;
}

ScriptValue::ScriptValue( crunch_u64 u )
{
  mValue.u = u;
  mType = ScriptTypes::ScriptType::TYPE_U64;
}

ScriptValue::ScriptValue( crunch_f64 f )
{
  mValue.f = f;
  mType = ScriptTypes::ScriptType::TYPE_F64;
}

ScriptValue::ScriptValue( ScriptCompiler *compiler, const crunch_string *s )
{
  if ( s != nullptr )
    mValue.s = compiler->stringTableMergeString( s->c_str(), false );
  else
    mValue.s = compiler->stringTableMergeString( "", false );

  mType = ScriptTypes::ScriptType::TYPE_STRING;
}

ScriptValue::ScriptValue( char c )
{
  mValue.c = crunch_char( c );
  mType = ScriptTypes::ScriptType::TYPE_CHAR;
}

ScriptValue::~ScriptValue()
{
  // empty
}

ScriptValue::ScriptValue( const ScriptValue& other ) :
  mValue( other.mValue ),
  mType( other.mType )
{
  // empty
}

ScriptValue& ScriptValue::operator = ( const ScriptValue& other )
{
  if ( &other == this )
    return *this;

  mValue = other.mValue;
  mType = other.mType;

  return *this;
}

crunch::core::String ScriptValue::getSourceDescription() const
{
  crunch::core::String r;

  switch ( mType )
  {
    case ScriptTypes::ScriptType::TYPE_STRING:
      if ( mValue.s != nullptr )
        r = crunch::core::String::fmt( "\"%s\"", mValue.s->c_str() );
      else
        r = "\"nullptr\"";
    break;

    case ScriptTypes::ScriptType::TYPE_CHAR:
      r = crunch::core::String::fmt( "%c", char( mValue.c ) );
    break;

    case ScriptTypes::ScriptType::TYPE_I64:
      r = crunch::core::String::i64tos( mValue.i );
    break;

    case ScriptTypes::ScriptType::TYPE_U64:
      r = crunch::core::String::u64tos( mValue.u );
    break;

    case ScriptTypes::ScriptType::TYPE_F64:
      r = crunch::core::String::dtos( mValue.f );
    break;

    default:
      // empty
    break;
  }

  return r;
}

ScriptValue& ScriptValue::negate()
{
  switch ( mType )
  {
    case ScriptTypes::ScriptType::TYPE_I64:
      mValue.i = -( mValue.i );
    break;

    case ScriptTypes::ScriptType::TYPE_U64:
      mValue.i = -( (crunch_i64)( mValue.u ) );
      mType = ScriptTypes::ScriptType::TYPE_I64;
    break;

    case ScriptTypes::ScriptType::TYPE_F64:
      mValue.f = -( mValue.f );
    break;

    default:
      mType = ScriptTypes::ScriptType::TYPE_NONE;
    break;
  }

  return *this;
}

ScriptValue& ScriptValue::boolNot()
{
  switch ( mType )
  {
    case ScriptTypes::ScriptType::TYPE_I64:
      mValue.i = ( mValue.i != 0 ) ? 0 : 1;
    break;

    case ScriptTypes::ScriptType::TYPE_U64:
      mValue.u = ( mValue.u != 0 ) ? 0 : 1;
    break;

    case ScriptTypes::ScriptType::TYPE_F64:
      mValue.f = ( (crunch_i64)( mValue.f ) != 0 ) ? 0.0 : 1.0;
    break;

    default:
      mType = ScriptTypes::ScriptType::TYPE_NONE;
    break;
  }

  return *this;
}

ScriptValue& ScriptValue::bitwiseComplement()
{
  switch ( mType )
  {
    case ScriptTypes::ScriptType::TYPE_I64:
      mValue.i = ~( mValue.i );
    break;

    case ScriptTypes::ScriptType::TYPE_U64:
      mValue.u = ~( mValue.u );
    break;

    default:
      mType = ScriptTypes::ScriptType::TYPE_NONE;
    break;
  }

  return *this;
}

ScriptValue& ScriptValue::compareEE( const ScriptValue& rhs )
{
  switch ( mType )
  {
    case ScriptTypes::ScriptType::TYPE_STRING:
      if ( rhs.mType == ScriptTypes::ScriptType::TYPE_STRING && mValue.s != nullptr && rhs.mValue.s != nullptr )
      {
        mValue.i = ( *( mValue.s ) == *( rhs.mValue.s ) ) ? 1 : 0;
        mType = ScriptTypes::ScriptType::TYPE_I64;
      }
      else
        mType = ScriptTypes::ScriptType::TYPE_NONE;
    break;

    case ScriptTypes::ScriptType::TYPE_CHAR:
      if ( rhs.mType == ScriptTypes::ScriptType::TYPE_CHAR )
      {
        mValue.i = ( mValue.c == rhs.mValue.c ) ? 1 : 0;
        mType = ScriptTypes::ScriptType::TYPE_I64;
      }
      else
        mType = ScriptTypes::ScriptType::TYPE_NONE;
    break;

    case ScriptTypes::ScriptType::TYPE_I64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = ( mValue.i == rhs.mValue.i ) ? 1 : 0;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.i = ( mValue.i == (crunch_i64)( rhs.mValue.u ) ) ? 1 : 0;
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.i = ( mValue.i == (crunch_i64)( rhs.mValue.f ) ) ? 1 : 0;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    case ScriptTypes::ScriptType::TYPE_U64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = ( mValue.u == (crunch_u64)( rhs.mValue.i ) ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.i = ( mValue.u == rhs.mValue.u ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.i = ( mValue.u == (crunch_u64)( rhs.mValue.f ) ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    case ScriptTypes::ScriptType::TYPE_F64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = ( mValue.f == (crunch_f64)( rhs.mValue.i ) ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.i = ( mValue.f == (crunch_f64)( rhs.mValue.u ) ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.i = ( mValue.f == rhs.mValue.f ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    default:
      mType = ScriptTypes::ScriptType::TYPE_NONE;
    break;
  }

  return *this;
}

ScriptValue& ScriptValue::compareNE( const ScriptValue& rhs )
{
  compareEE( rhs );

  if ( mType == ScriptTypes::ScriptType::TYPE_I64 )
    mValue.i = ( mValue.i != 0 ) ? 0 : 1;

  return *this;
}

ScriptValue& ScriptValue::compareGT( const ScriptValue& rhs )
{
  switch ( mType )
  {
    case ScriptTypes::ScriptType::TYPE_STRING:
      if ( rhs.mType == ScriptTypes::ScriptType::TYPE_STRING && mValue.s != nullptr && rhs.mValue.s != nullptr )
      {
        mValue.i = ( *( mValue.s ) > *( rhs.mValue.s ) ) ? 1 : 0;
        mType = ScriptTypes::ScriptType::TYPE_I64;
      }
      else
        mType = ScriptTypes::ScriptType::TYPE_NONE;
    break;

    case ScriptTypes::ScriptType::TYPE_CHAR:
      if ( rhs.mType == ScriptTypes::ScriptType::TYPE_CHAR )
      {
        mValue.i = ( mValue.c > rhs.mValue.c ) ? 1 : 0;
        mType = ScriptTypes::ScriptType::TYPE_I64;
      }
      else
        mType = ScriptTypes::ScriptType::TYPE_NONE;
    break;

    case ScriptTypes::ScriptType::TYPE_I64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = ( mValue.i > rhs.mValue.i ) ? 1 : 0;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.i = ( mValue.i > (crunch_i64)( rhs.mValue.u ) ) ? 1 : 0;
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.i = ( mValue.i > (crunch_i64)( rhs.mValue.f ) ) ? 1 : 0;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    case ScriptTypes::ScriptType::TYPE_U64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = ( mValue.u > (crunch_u64)( rhs.mValue.i ) ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.i = ( mValue.u > rhs.mValue.u ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.i = ( mValue.u > (crunch_u64)( rhs.mValue.f ) ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    case ScriptTypes::ScriptType::TYPE_F64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = ( mValue.f > (crunch_f64)( rhs.mValue.i ) ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.i = ( mValue.f > (crunch_f64)( rhs.mValue.u ) ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.i = ( mValue.f > rhs.mValue.f ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    default:
      mType = ScriptTypes::ScriptType::TYPE_NONE;
    break;
  }

  return *this;
}

ScriptValue& ScriptValue::compareGE( const ScriptValue& rhs )
{
  switch ( mType )
  {
    case ScriptTypes::ScriptType::TYPE_STRING:
      if ( rhs.mType == ScriptTypes::ScriptType::TYPE_STRING && mValue.s != nullptr && rhs.mValue.s != nullptr )
      {
        mValue.i = ( *( mValue.s ) >= *( rhs.mValue.s ) ) ? 1 : 0;
        mType = ScriptTypes::ScriptType::TYPE_I64;
      }
      else
        mType = ScriptTypes::ScriptType::TYPE_NONE;
    break;

    case ScriptTypes::ScriptType::TYPE_CHAR:
      if ( rhs.mType == ScriptTypes::ScriptType::TYPE_CHAR )
      {
        mValue.i = ( mValue.c >= rhs.mValue.c ) ? 1 : 0;
        mType = ScriptTypes::ScriptType::TYPE_I64;
      }
      else
        mType = ScriptTypes::ScriptType::TYPE_NONE;
    break;

    case ScriptTypes::ScriptType::TYPE_I64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = ( mValue.i >= rhs.mValue.i ) ? 1 : 0;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.i = ( mValue.i >= (crunch_i64)( rhs.mValue.u ) ) ? 1 : 0;
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.i = ( mValue.i >= (crunch_i64)( rhs.mValue.f ) ) ? 1 : 0;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    case ScriptTypes::ScriptType::TYPE_U64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = ( mValue.u >= (crunch_u64)( rhs.mValue.i ) ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.i = ( mValue.u >= rhs.mValue.u ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.i = ( mValue.u >= (crunch_u64)( rhs.mValue.f ) ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    case ScriptTypes::ScriptType::TYPE_F64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = ( mValue.f >= (crunch_f64)( rhs.mValue.i ) ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.i = ( mValue.f >= (crunch_f64)( rhs.mValue.u ) ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.i = ( mValue.f >= rhs.mValue.f ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    default:
      mType = ScriptTypes::ScriptType::TYPE_NONE;
    break;
  }

  return *this;
}

ScriptValue& ScriptValue::compareLT( const ScriptValue& rhs )
{
  compareGE( rhs );

  if ( mType == ScriptTypes::ScriptType::TYPE_I64 )
    mValue.i = ( mValue.i != 0 ) ? 0 : 1;

  return *this;
}

ScriptValue& ScriptValue::compareLE( const ScriptValue& rhs )
{
  compareGT( rhs );

  if ( mType == ScriptTypes::ScriptType::TYPE_I64 )
    mValue.i = ( mValue.i != 0 ) ? 0 : 1;

  return *this;
}

ScriptValue& ScriptValue::add( const ScriptValue& rhs )
{
  switch ( mType )
  {
    case ScriptTypes::ScriptType::TYPE_I64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = mValue.i + rhs.mValue.i;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.i = mValue.i + (crunch_i64)( rhs.mValue.u );
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.f = (crunch_f64)( mValue.i ) + rhs.mValue.f;
          mType = ScriptTypes::ScriptType::TYPE_F64;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    case ScriptTypes::ScriptType::TYPE_U64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = (crunch_i64)( mValue.u ) + rhs.mValue.i;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.u = mValue.u + rhs.mValue.u;
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.f = (crunch_f64)( mValue.u ) + rhs.mValue.f;
          mType = ScriptTypes::ScriptType::TYPE_F64;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    case ScriptTypes::ScriptType::TYPE_F64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.f = mValue.f + (crunch_f64)( rhs.mValue.i );
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.f = mValue.f + (crunch_f64)( rhs.mValue.u );
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.f = mValue.f + rhs.mValue.f;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    default:
      mType = ScriptTypes::ScriptType::TYPE_NONE;
    break;
  }

  return *this;
}

ScriptValue& ScriptValue::subtract( const ScriptValue& rhs )
{
  return add( ScriptValue( rhs ).negate() );
}

ScriptValue& ScriptValue::multiply( const ScriptValue& rhs )
{
  switch ( mType )
  {
    case ScriptTypes::ScriptType::TYPE_I64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = mValue.i * rhs.mValue.i;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.i = mValue.i * (crunch_i64)( rhs.mValue.u );
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.f = (crunch_f64)( mValue.i ) * rhs.mValue.f;
          mType = ScriptTypes::ScriptType::TYPE_F64;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    case ScriptTypes::ScriptType::TYPE_U64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = (crunch_i64)( mValue.u ) * rhs.mValue.i;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.u = mValue.u * rhs.mValue.u;
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.f = (crunch_f64)( mValue.u ) * rhs.mValue.f;
          mType = ScriptTypes::ScriptType::TYPE_F64;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    case ScriptTypes::ScriptType::TYPE_F64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.f = mValue.f * (crunch_f64)( rhs.mValue.i );
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.f = mValue.f * (crunch_f64)( rhs.mValue.u );
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.f = mValue.f * rhs.mValue.f;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    default:
      mType = ScriptTypes::ScriptType::TYPE_NONE;
    break;
  }

  return *this;
}

ScriptValue& ScriptValue::divide( const ScriptValue& rhs )
{
  switch ( mType )
  {
    case ScriptTypes::ScriptType::TYPE_I64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = mValue.i / rhs.mValue.i;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.i = mValue.i / (crunch_i64)( rhs.mValue.u );
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.f = (crunch_f64)( mValue.i ) / rhs.mValue.f;
          mType = ScriptTypes::ScriptType::TYPE_F64;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    case ScriptTypes::ScriptType::TYPE_U64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = (crunch_i64)( mValue.u ) / rhs.mValue.i;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.u = mValue.u / rhs.mValue.u;
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.f = (crunch_f64)( mValue.u ) / rhs.mValue.f;
          mType = ScriptTypes::ScriptType::TYPE_F64;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    case ScriptTypes::ScriptType::TYPE_F64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.f = mValue.f / (crunch_f64)( rhs.mValue.i );
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.f = mValue.f / (crunch_f64)( rhs.mValue.u );
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.f = mValue.f / rhs.mValue.f;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    default:
      mType = ScriptTypes::ScriptType::TYPE_NONE;
    break;
  }

  return *this;
}

ScriptValue& ScriptValue::boolAnd( const ScriptValue& rhs )
{
  switch ( mType )
  {
    case ScriptTypes::ScriptType::TYPE_I64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = ( mValue.i && rhs.mValue.i ) ? 1 : 0;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.i = ( mValue.i && (crunch_i64)( rhs.mValue.u ) ) ? 1 : 0;
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.i = ( mValue.i && (crunch_i64)( rhs.mValue.f ) ) ? 1 : 0;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    case ScriptTypes::ScriptType::TYPE_U64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = ( mValue.u && (crunch_u64)( rhs.mValue.i ) ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.i = ( mValue.u && rhs.mValue.u ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.i = ( mValue.u && (crunch_u64)( rhs.mValue.f ) ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    case ScriptTypes::ScriptType::TYPE_F64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = ( mValue.f && (crunch_f64)( rhs.mValue.i ) ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.i = ( mValue.f && (crunch_f64)( rhs.mValue.u ) ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.i = ( mValue.f && rhs.mValue.f ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    default:
      mType = ScriptTypes::ScriptType::TYPE_NONE;
    break;
  }

  return *this;
}

ScriptValue& ScriptValue::boolOr( const ScriptValue& rhs )
{
  switch ( mType )
  {
    case ScriptTypes::ScriptType::TYPE_I64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = ( mValue.i || rhs.mValue.i ) ? 1 : 0;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.i = ( mValue.i || (crunch_i64)( rhs.mValue.u ) ) ? 1 : 0;
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.i = ( mValue.i || (crunch_i64)( rhs.mValue.f ) ) ? 1 : 0;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    case ScriptTypes::ScriptType::TYPE_U64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = ( mValue.u || (crunch_u64)( rhs.mValue.i ) ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.i = ( mValue.u || rhs.mValue.u ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.i = ( mValue.u || (crunch_u64)( rhs.mValue.f ) ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    case ScriptTypes::ScriptType::TYPE_F64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = ( mValue.f || (crunch_f64)( rhs.mValue.i ) ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.i = ( mValue.f || (crunch_f64)( rhs.mValue.u ) ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          mValue.i = ( mValue.f || rhs.mValue.f ) ? 1 : 0;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    default:
      mType = ScriptTypes::ScriptType::TYPE_NONE;
    break;
  }

  return *this;
}

ScriptValue& ScriptValue::bitwiseAnd( const ScriptValue& rhs )
{
  switch ( mType )
  {
    case ScriptTypes::ScriptType::TYPE_I64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = mValue.i & rhs.mValue.i;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.i = mValue.i & (crunch_i64)( rhs.mValue.u );
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    case ScriptTypes::ScriptType::TYPE_U64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = (crunch_i64)( mValue.u ) & rhs.mValue.i;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.u = mValue.u & rhs.mValue.u;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    default:
      mType = ScriptTypes::ScriptType::TYPE_NONE;
    break;
  }

  return *this;
}

ScriptValue& ScriptValue::bitwiseOr( const ScriptValue& rhs )
{
  switch ( mType )
  {
    case ScriptTypes::ScriptType::TYPE_I64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = mValue.i | rhs.mValue.i;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.i = mValue.i | (crunch_i64)( rhs.mValue.u );
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    case ScriptTypes::ScriptType::TYPE_U64:
      switch ( rhs.mType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          mValue.i = (crunch_i64)( mValue.u ) | rhs.mValue.i;
          mType = ScriptTypes::ScriptType::TYPE_I64;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          mValue.u = mValue.u | rhs.mValue.u;
        break;

        default:
          mType = ScriptTypes::ScriptType::TYPE_NONE;
        break;
      }
    break;

    default:
      mType = ScriptTypes::ScriptType::TYPE_NONE;
    break;
  }

  return *this;
}

bool ScriptValue::generateCode( ScriptCompiler *compiler, crunch::core::BinaryBuffer *dest, const crunch::core::WideString& sourceFilePath, unsigned int lineNumber ) const
{
  switch ( mType )
  {
    case ScriptTypes::ScriptType::TYPE_STRING:
      ScriptInstruction::write_PUSH_STRING( compiler, dest, sourceFilePath, lineNumber, mValue.s );
    break;

    case ScriptTypes::ScriptType::TYPE_CHAR:
      ScriptInstruction::write_PUSH_I64( compiler, dest, sourceFilePath, lineNumber, mValue.c );
    break;

    case ScriptTypes::ScriptType::TYPE_I64:
      ScriptInstruction::write_PUSH_I64( compiler, dest, sourceFilePath, lineNumber, mValue.i );
    break;

    case ScriptTypes::ScriptType::TYPE_U64:
      ScriptInstruction::write_PUSH_U64( compiler, dest, sourceFilePath, lineNumber, mValue.u );
    break;

    case ScriptTypes::ScriptType::TYPE_F64:
      ScriptInstruction::write_PUSH_F64( compiler, dest, sourceFilePath, lineNumber, mValue.f );
    break;

    case ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS:
      ScriptInstruction::write_PUSH_NULL_PTR( compiler, dest, sourceFilePath, lineNumber );
    break;

    default:
      compiler->reportInternalError( sourceFilePath, lineNumber, "invalid script value type for generate code" );
      return false;
  }

  return true;
}

bool ScriptValue::read( const ScriptExecutable *se, crunch::core::BinaryBuffer *source )
{
  if ( source == nullptr )
    return false;

  mType = (ScriptTypes::ScriptType)( source->read_int32() );

  switch ( mType )
  {
    case ScriptTypes::ScriptType::TYPE_STRING:
    {
      const crunch::core::String *s = se->stringTableGetString( source->read_uint32() );
      if ( s == nullptr )
        return false;
      mValue.s = s;
    }
    break;

    case ScriptTypes::ScriptType::TYPE_CHAR:
      mValue.c = source->read_int64();
    break;

    case ScriptTypes::ScriptType::TYPE_I64:
      mValue.i = source->read_int64();
    break;

    case ScriptTypes::ScriptType::TYPE_U64:
      mValue.u = source->read_uint64();
    break;

    case ScriptTypes::ScriptType::TYPE_F64:
      mValue.f = source->read_double();
    break;

    default:
      // empty
    break;
  }

  return true;
}

bool ScriptValue::write( const ScriptExecutable *se, crunch::core::BinaryBuffer *dest ) const
{
  if ( dest == nullptr )
    return false;

  dest->append_int32( (int)( mType ) );

  switch ( mType )
  {
    case ScriptTypes::ScriptType::TYPE_STRING:
    {
      unsigned int index = se->stringTableGetIndex( mValue.s );
      if ( index == UINT_MAX )
        return false;
      dest->append_uint32( index );
    }
    break;

    case ScriptTypes::ScriptType::TYPE_CHAR:
      dest->append_int64( mValue.c );
    break;

    case ScriptTypes::ScriptType::TYPE_I64:
      dest->append_int64( mValue.i );
    break;

    case ScriptTypes::ScriptType::TYPE_U64:
      dest->append_uint64( mValue.u );
    break;

    case ScriptTypes::ScriptType::TYPE_F64:
      dest->append_double( mValue.f );
    break;

    default:
      // empty
    break;
  }

  return true;
}

} }
