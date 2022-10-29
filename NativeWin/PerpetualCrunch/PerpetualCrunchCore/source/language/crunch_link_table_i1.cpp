
#include "../crunch_global.h"

#include "crunch_script_vm.h"
#include "crunch_script_executable.h"
#include "crunch_script_library.h"
#include "crunch_script_library_function.h"

#include "crunch_link_table_i1.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

LinkTableI1::LinkTableI1( ScriptInstruction::Opcode linkOpcode ) :
  LinkTable( linkOpcode )
{
  // empty
}

LinkTableI1::~LinkTableI1()
{
  // empty
}

bool LinkTableI1::link( ScriptVM *vm, ScriptExecutable *se ) const
{
  switch ( mLinkOpcode )
  {
    case ScriptInstruction::Opcode::OP_LIB_GFUNC_CALL:
      return link_LIB_GFUNC_CALL( vm, se );

    default:
      // empty
    break;
  }

  return false;
}

void LinkTableI1::appendAssembler( crunch::core::String *dest ) const
{
  if ( dest == nullptr )
    return;

  LinkTable::appendAssembler( dest );

  unsigned int totalOffsetEntries = (unsigned int)( mOffsetEntries.size() );

  *dest += crunch::core::String::fmt( "  offset entries: %u\n", totalOffsetEntries );

  OffsetEntrySetType::const_iterator it = mOffsetEntries.cbegin();
  OffsetEntrySetType::const_iterator itEnd = mOffsetEntries.cend();

  for ( ; it != itEnd; ++it )
  {
    const OffsetEntry& entry = *it;
    *dest += crunch::core::String::fmt( "  %04u : total offsets: %u\n", entry.mIndex, (unsigned int)( entry.mOffsets.size() ) );
  }

  *dest += "\n";
}

bool LinkTableI1::append( uint32_t index, uint32_t offset )
{
  OffsetEntry entry;
  entry.mIndex = index;

  std::pair< OffsetEntrySetType::iterator, bool > ret = mOffsetEntries.insert( entry );

  OffsetEntry *e = (OffsetEntry*)( &( *( ret.first ) ) );

  e->mOffsets.push_back( offset );

  return true;
}

bool LinkTableI1::read( crunch::core::BinaryBuffer *source )
{
  if ( source == nullptr )
    return false;

  if ( !( LinkTable::read( source ) ) )
    return false;

  mOffsetEntries.clear();

  unsigned int totalOffsetEntries = source->read_uint32();

  for ( unsigned int offsetEntryIndex = 0; offsetEntryIndex < totalOffsetEntries; ++offsetEntryIndex )
  {
    OffsetEntry entry;

    entry.mIndex = (uint32_t)( source->read_uint32() );

    std::pair< OffsetEntrySetType::iterator, bool > ret = mOffsetEntries.insert( entry );

    OffsetEntry *e = (OffsetEntry*)( &( *( ret.first ) ) );

    unsigned int totalOffsets = source->read_uint32();

    e->mOffsets.resize( totalOffsets );

    for ( unsigned int offsetIndex = 0; offsetIndex < totalOffsets; ++offsetIndex )
      e->mOffsets[offsetIndex] = source->read_uint32();
  }

  return true;
}

bool LinkTableI1::write( crunch::core::BinaryBuffer *dest ) const
{
  if ( dest == nullptr )
    return false;

  if ( !( LinkTable::write( dest ) ) )
    return false;

  unsigned int totalOffsetEntries = (unsigned int)( mOffsetEntries.size() );

  dest->append_uint32( totalOffsetEntries );

  OffsetEntrySetType::const_iterator it = mOffsetEntries.cbegin();
  OffsetEntrySetType::const_iterator itEnd = mOffsetEntries.cend();

  for ( ; it != itEnd; ++it )
  {
    const OffsetEntry& entry = *it;

    dest->append_uint32( entry.mIndex );

    unsigned int totalOffsets = (unsigned int)( entry.mOffsets.size() );

    dest->append_uint32( totalOffsets );

    for ( unsigned int i = 0; i < totalOffsets; ++i )
      dest->append_uint32( entry.mOffsets[i] );
  }

  return true;
}

bool LinkTableI1::link_LIB_GFUNC_CALL( ScriptVM *vm, ScriptExecutable *se ) const
{
  if ( se == nullptr )
    return false;

  char *byteCodeBuffer = se->getByteCodeBuffer()->c_str();

  OffsetEntrySetType::const_iterator it = mOffsetEntries.cbegin();
  OffsetEntrySetType::const_iterator itEnd = mOffsetEntries.cend();

  for ( ; it != itEnd; ++it )
  {
    const OffsetEntry& entry = *it;

    const crunch::core::String *functionSignature = se->stringTableGetString( (unsigned int)( entry.mIndex ) );

    const ScriptLibraryFunction *libFunction = ScriptLibrary::getGlobalLibraryClass()->findFunction( *functionSignature );
    if ( libFunction == nullptr )
    {
      corePrint( "[ERROR] failed to resolve global library function | function: '%s'", functionSignature->c_str() );
      return false;
    }

    unsigned int totalOffsets = (unsigned int)( entry.mOffsets.size() );

    for ( unsigned int i = 0; i < totalOffsets; ++i )
    {
      uint32_t offset = entry.mOffsets[i];

      char *pc = &( byteCodeBuffer[offset] );
      
      #if defined _CRUNCH_DEBUG
      {
        uint64_t currentInstruction = *(uint64_t*)( pc );
        ScriptInstruction::Opcode op = (ScriptInstruction::Opcode)( (uint32_t)( currentInstruction & 0xFFFFFFFF ) );
        assert( op == ScriptInstruction::Opcode::OP_LIB_GFUNC_CALL && "unexpected opcode" );
      }
      #endif

      *(uint64_t*)( pc ) = (uint64_t)( ScriptInstruction::Opcode::OP_LIB_GFUNC_CALLX );
      pc += sizeof( uint64_t );

      *(void**)( pc ) = (void*)( libFunction );
    }
  }

  return true;
}

} }
