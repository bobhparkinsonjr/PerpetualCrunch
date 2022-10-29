
#include "../crunch_global.h"

#include "crunch_link_table.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

LinkTable::LinkTable( ScriptInstruction::Opcode linkOpcode ) :
  mLinkOpcode( linkOpcode )
{
  // empty
}

LinkTable::~LinkTable()
{
  // empty
}

void LinkTable::appendAssembler( crunch::core::String *dest ) const
{
  if ( dest == nullptr )
    return;

  *dest += crunch::core::String::fmt( "LINK TABLE '%s'\n", ScriptInstruction::getOpcodeName( mLinkOpcode ).c_str() );
}

bool LinkTable::read( crunch::core::BinaryBuffer *source )
{
  if ( source == nullptr )
    return false;

  mLinkOpcode = (ScriptInstruction::Opcode)( source->read_int32() );
  
  return true;
}

bool LinkTable::write( crunch::core::BinaryBuffer *dest ) const
{
  if ( dest == nullptr )
    return false;

  dest->append_int32( (int32_t)( mLinkOpcode ) );

  return true;
}

} }
