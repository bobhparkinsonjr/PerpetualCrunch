
#include "../crunch_global.h"

#include "crunch_script_vm.h"
#include "crunch_script_executable.h"
#include "crunch_script_library.h"
#include "crunch_script_library_function.h"

#include "crunch_link_table_i2.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

LinkTableI2::LinkTableI2( ScriptInstruction::Opcode linkOpcode ) :
  LinkTable( linkOpcode )
{
  // empty
}

LinkTableI2::~LinkTableI2()
{
  // empty
}

bool LinkTableI2::link( ScriptVM *vm, ScriptExecutable *se ) const
{
  switch ( mLinkOpcode )
  {
    case ScriptInstruction::Opcode::OP_PUSH_FETCH_GLIB_CLS:
      return link_PUSH_FETCH_GLIB_CLS( vm, se );

    case ScriptInstruction::Opcode::OP_LIB_FUNC_CALL:
      return link_LIB_FUNC_CALL( vm, se );

    default:
      // empty
    break;
  }

  return false;
}

void LinkTableI2::appendAssembler( crunch::core::String *dest ) const
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
    *dest += crunch::core::String::fmt( "  < %04u, %04u > : total offsets: %u\n", entry.mIndex, entry.mIndex2, (unsigned int)( entry.mOffsets.size() ) );
  }

  *dest += "\n";
}

bool LinkTableI2::append( uint32_t index, uint64_t index2, uint32_t offset )
{
  OffsetEntry entry;
  entry.mIndex = index;
  entry.mIndex2 = index2;

  std::pair< OffsetEntrySetType::iterator, bool > ret = mOffsetEntries.insert( entry );

  OffsetEntry *e = (OffsetEntry*)( &( *( ret.first ) ) );

  e->mOffsets.push_back( offset );

  return true;
}

bool LinkTableI2::read( crunch::core::BinaryBuffer *source )
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
    entry.mIndex2 = source->read_uint64();

    std::pair< OffsetEntrySetType::iterator, bool > ret = mOffsetEntries.insert( entry );

    OffsetEntry *e = (OffsetEntry*)( &( *( ret.first ) ) );

    unsigned int totalOffsets = source->read_uint32();

    e->mOffsets.resize( totalOffsets );

    for ( unsigned int offsetIndex = 0; offsetIndex < totalOffsets; ++offsetIndex )
      e->mOffsets[offsetIndex] = source->read_uint32();
  }

  return true;
}

bool LinkTableI2::write( crunch::core::BinaryBuffer *dest ) const
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
    dest->append_uint64( entry.mIndex2 );

    unsigned int totalOffsets = (unsigned int)( entry.mOffsets.size() );

    dest->append_uint32( totalOffsets );

    for ( unsigned int i = 0; i < totalOffsets; ++i )
      dest->append_uint32( entry.mOffsets[i] );
  }

  return true;
}

bool LinkTableI2::link_PUSH_FETCH_GLIB_CLS( ScriptVM *vm, ScriptExecutable *se ) const
{
  if ( se == nullptr )
    return false;

  char *byteCodeBuffer = se->getByteCodeBuffer()->c_str();

  OffsetEntrySetType::const_iterator it = mOffsetEntries.cbegin();
  OffsetEntrySetType::const_iterator itEnd = mOffsetEntries.cend();

  for ( ; it != itEnd; ++it )
  {
    const OffsetEntry& entry = *it;

    unsigned int totalOffsets = (unsigned int)( entry.mOffsets.size() );
    if ( totalOffsets == 0 )
      continue;

    const crunch::core::String *instanceName = se->stringTableGetString( (unsigned int)( entry.mIndex ) );
    const crunch::core::String *className = se->stringTableGetString( (unsigned int)( entry.mIndex2 ) );

    void *glibInstance;

    if ( !( vm->findGlobalLibraryClassProperty( *className, *instanceName, &glibInstance ) ) )
    {
      corePrint( "[ERROR] failed to resolve global library class instance | class: '%s' | instance: '%s'", className->c_str(), instanceName->c_str() );
      return false;
    }

    for ( unsigned int i = 0; i < totalOffsets; ++i )
    {
      uint32_t offset = entry.mOffsets[i];

      char *pc = &( byteCodeBuffer[offset] );

      #if defined _CRUNCH_DEBUG
      {
        uint64_t currentInstruction = *(uint64_t*)( pc );
        ScriptInstruction::Opcode op = (ScriptInstruction::Opcode)( (uint32_t)( currentInstruction & 0xFFFFFFFF ) );
        assert( op == ScriptInstruction::Opcode::OP_PUSH_FETCH_GLIB_CLS && "unexpected opcode" );
      }
      #endif

      *(uint64_t*)( pc ) = (uint64_t)( ScriptInstruction::Opcode::OP_PUSH_FETCH_GLIB_CLSX );
      pc += sizeof( uint64_t );

      *(void**)( pc ) = glibInstance;
    }
  }

  return true;
}

bool LinkTableI2::link_LIB_FUNC_CALL( ScriptVM *vm, ScriptExecutable *se ) const
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
    const crunch::core::String *className = se->stringTableGetString( (unsigned int)( entry.mIndex2 ) );

    const ScriptLibraryClass *libClass = ScriptLibrary::findLibraryClass( className->c_str() );
    if ( libClass == nullptr )
    {
      corePrint( "[ERROR] failed to resolve library class | class: '%s'", className->c_str() );
      return false;
    }

    const ScriptLibraryFunction *libFunction = libClass->findFunction( *functionSignature );
    if ( libFunction == nullptr )
    {
      corePrint( "[ERROR] failed to resolve library function | class: '%s' | function: '%s'", className->c_str(), functionSignature->c_str() );
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
        assert( op == ScriptInstruction::Opcode::OP_LIB_FUNC_CALL && "unexpected opcode" );
      }
      #endif

      *(uint64_t*)( pc ) = (uint64_t)( ScriptInstruction::Opcode::OP_LIB_FUNC_CALLX );
      pc += sizeof( uint64_t );

      *(void**)( pc ) = (void*)( libFunction );
    }
  }

  return true;
}

} }
