
#include "../crunch_global.h"

#include "crunch_string_table.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#define CRUNCH_STRING_TABLE_VERSION 1

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

///////////////////////////////////////////////////////////////////////////////////////////////////


StringTable::StringTable()
{
  // empty
}

StringTable::~StringTable()
{
  destroy();
}

void StringTable::destroy()
{
  unsigned int total = (unsigned int)( mPool.size() );

  // corePrint( "deleting %u strings ...", total );

  for ( unsigned int i = 0; i < total; ++i )
    delete mPool[i];

  mPool.clear();
}

crunch::core::String* StringTable::mergeString( const char *v, bool literalString, unsigned int *index )
{
  crunch::core::String findMe( v );

  if ( literalString )
  {
    if ( findMe.size() >= 2 && findMe[0] == '\"' && findMe[findMe.size() - 1] == '\"' )
      findMe = findMe.substr( 1, findMe.size() - 2 ); // strip the surrounding double quotes for string literals
    else
    if ( findMe.size() >= 2 && findMe[0] == '\'' && findMe[findMe.size() - 1] == '\'' )
      findMe = findMe.substr( 1, findMe.size() - 2 ); // strip the surrounding single quotes for characters (will also work if single quote strings are allowed)
  }

  unsigned int total = (unsigned int)( mPool.size() );

  for ( unsigned int i = 0; i < total; ++i )
  {
    if ( *( mPool[i] ) == findMe )
    {
      if ( index != nullptr )
        *index = i;

      return mPool[i];
    }
  }

  if ( index != nullptr )
    *index = (unsigned int)( mPool.size() );

  mPool.push_back( new crunch::core::String( findMe ) );

  return mPool.back();
}

unsigned int StringTable::getIndex( const crunch::core::String *s ) const
{
  if ( s == nullptr )
    return UINT_MAX;

  unsigned int total = (unsigned int)( mPool.size() );

  for ( unsigned int i = 0; i < total; ++i )
  {
    if ( mPool[i] == s )
      return i;
  }

  return UINT_MAX;
}

const crunch::core::String* StringTable::getString( unsigned int index ) const
{
  if ( index >= (unsigned int)( mPool.size() ) )
    return nullptr;

  return mPool[index];
}

bool StringTable::read( crunch::core::BinaryBuffer *source )
{
  if ( source == nullptr )
    return false;

  destroy();

  unsigned int version = source->read_uint32();

  if ( version < 1 || version > CRUNCH_STRING_TABLE_VERSION )
    return false;

  unsigned int total = source->read_uint32();

  mPool.resize( total );

  for ( unsigned int i = 0; i < total; ++i )
  {
    crunch::core::String s = source->read_String();
    mPool[i] = new crunch::core::String( s );
  }

  return true;
}

bool StringTable::write( crunch::core::BinaryBuffer *dest ) const
{
  if ( dest == nullptr )
    return false;

  dest->append_uint32( CRUNCH_STRING_TABLE_VERSION );

  unsigned int total = (unsigned int)( mPool.size() );

  dest->append_uint32( total );

  for ( unsigned int i = 0; i < total; ++i )
    dest->append_String( mPool[i]->c_str() );

  return true;
}

void StringTable::appendAssembler( crunch::core::String *dest ) const
{
  if ( dest == nullptr )
    return;

  unsigned int total = (unsigned int)( mPool.size() );

  *dest += crunch::core::String::fmt( "STRING TABLE (total: %u)\n", total );

  for ( unsigned int i = 0; i < total; ++i )
    *dest += crunch::core::String::fmt( "  %06u : \"%s\"\n", i, mPool[i]->c_str() );

  *dest += "\n";
}

void StringTable::toCorePrint() const
{
  crunch::core::String msg;
  appendAssembler( &msg );
  corePrint( msg.c_str() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////

} }
