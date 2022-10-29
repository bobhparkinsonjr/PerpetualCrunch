
#include "../crunch_global.h"

#include "crunch_script_compiler.h"
#include "crunch_script_instruction.h"
#include "crunch_script_vm.h"
#include "crunch_script_thread.h"

#include "crunch_script_executable.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#define CRUNCH_EXECUTABLE_FILE_VERSION         1
#define CRUNCH_EXECUTABLE_FILE_VERSION_INVALID 0xFFFFFFFF

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

ScriptExecutable::ScriptExecutable() :
  mGlobalObject( CRUNCH_GLOBAL_SCRIPT_OBJECT_NAME ),
  mLinkTable_LIB_GFUNC_CALL( ScriptInstruction::Opcode::OP_LIB_GFUNC_CALL ),
  mLinkTable_PUSH_FETCH_GLIB_CLS( ScriptInstruction::Opcode::OP_PUSH_FETCH_GLIB_CLS ),
  mLinkTable_LIB_FUNC_CALL( ScriptInstruction::Opcode::OP_LIB_FUNC_CALL )
{
  // empty
}

ScriptExecutable::~ScriptExecutable()
{
  destroy();
}

void ScriptExecutable::destroy()
{
  mStringTable.destroy();

  mVM = nullptr;

  mLinkTable_LIB_GFUNC_CALL.clear();
  mLinkTable_PUSH_FETCH_GLIB_CLS.clear();
  mLinkTable_LIB_FUNC_CALL.clear();

  mGlobalObject.clear();

  // objects
  {
    ScriptObjectSetType::iterator it = mObjects.begin();
    ScriptObjectSetType::iterator itEnd = mObjects.end();
    for ( ; it != itEnd; ++it )
      delete *it;

    mObjects.clear();
  }

  mByteCodeBuffer.destroy();
}

crunch::core::String* ScriptExecutable::stringTableMergeString( const char *v, bool literalString, unsigned int *index ) const
{
  return mStringTable.mergeString( v, literalString, index );
}

unsigned int ScriptExecutable::stringTableGetIndex( const crunch::core::String *s ) const
{
  return mStringTable.getIndex( s );
}

const crunch::core::String* ScriptExecutable::stringTableGetString( unsigned int index ) const
{
  return mStringTable.getString( index );
}

void ScriptExecutable::stringTableAppendAssembler( crunch::core::String *dest ) const
{
  mStringTable.appendAssembler( dest );
}

bool ScriptExecutable::link( ScriptVM *vm )
{
  if ( mVM != nullptr )
    return ( mVM == vm );

  mVM = vm;

  if ( !( mLinkTable_LIB_GFUNC_CALL.link( vm, this ) ) )
    return false;

  if ( !( mLinkTable_PUSH_FETCH_GLIB_CLS.link( vm, this ) ) )
    return false;

  if ( !( mLinkTable_LIB_FUNC_CALL.link( vm, this ) ) )
    return false;

  return true;
}

void ScriptExecutable::appendLinkTablesAssembler( crunch::core::String *dest ) const
{
  if ( dest == nullptr )
    return;

  mLinkTable_LIB_GFUNC_CALL.appendAssembler( dest );
  mLinkTable_PUSH_FETCH_GLIB_CLS.appendAssembler( dest );
  mLinkTable_LIB_FUNC_CALL.appendAssembler( dest );
}

bool ScriptExecutable::isAllowedObjectName( const crunch::core::String& name )
{
  return ( name.size() < 64 );
}

const ScriptObject* ScriptExecutable::findObject( const crunch::core::String& name ) const
{
  if ( name == "" )
    return nullptr;

  if ( crunch::core::FixedString64( name.c_str() ) == mGlobalObject.getName() )
    return &mGlobalObject;

  ScriptObject findMe( name.c_str() );

  ScriptObjectSetType::const_iterator findIt = mObjects.find( &findMe );
  if ( findIt != mObjects.end() )
    return *findIt;

  return nullptr;
}

ScriptObject* ScriptExecutable::addObject( const crunch::core::String& name )
{
  if ( !( isAllowedObjectName( name ) ) && crunch::core::FixedString64( name.c_str() ) != mGlobalObject.getName() )
    return nullptr;

  ScriptObject *myObject = new ScriptObject( name.c_str() );

  std::pair< ScriptObjectSetType::iterator, bool > insertResult = mObjects.insert( myObject );

  if ( insertResult.second )
    return myObject;

  delete myObject;

  return nullptr;
}

bool ScriptExecutable::removeObject( const crunch::core::String& name )
{
  ScriptObject findMe( name.c_str() );

  ScriptObjectSetType::iterator it = mObjects.find( &findMe );
  if ( it == mObjects.end() )
    return false;

  mObjects.erase( it );

  return true;
}

void ScriptExecutable::appendObjectTableAssembler( crunch::core::String *dest ) const
{
  if ( dest == nullptr )
    return;

  *dest += crunch::core::String::fmt( "SCRIPT OBJECTS (total:%u)\n\n", (unsigned int)( mObjects.size() ) + 1 );

  mGlobalObject.appendAssembler( dest );

  *dest += "\n";

  // objects
  {
    ScriptObjectSetType::const_iterator it = mObjects.cbegin();
    ScriptObjectSetType::const_iterator itEnd = mObjects.cend();
    for ( ; it != itEnd; ++it )
    {
      ( *it )->appendAssembler( dest );
      *dest += "\n";
    }
  }
}

bool ScriptExecutable::read( crunch::core::BinaryBuffer *source )
{
  if ( source == nullptr )
    return false;

  destroy();

  unsigned int version = source->read_uint32();
  if ( version < 1 || version > CRUNCH_EXECUTABLE_FILE_VERSION )
    return false;

  if ( !( mStringTable.read( source ) ) )
    return false;

  if ( !( mLinkTable_LIB_GFUNC_CALL.read( source ) ) )
    return false;

  if ( !( mLinkTable_PUSH_FETCH_GLIB_CLS.read( source ) ) )
    return false;

  if ( !( mLinkTable_LIB_FUNC_CALL.read( source ) ) )
    return false;

  if ( !( mGlobalObject.read( this, source ) ) )
    return false;

  // objects
  {
    unsigned int totalObjects = source->read_uint32();

    for ( unsigned int objectIndex = 0; objectIndex < totalObjects; ++objectIndex )
    {
      crunch::core::String name = source->read_String();

      ScriptObject *myObject = addObject( name );
      if ( myObject == nullptr )
        return false;

      if ( !( myObject->read( this, source ) ) )
        return false;
    }
  }

  unsigned int byteCodeBufferLength = source->read_uint32();

  if ( byteCodeBufferLength > 0 )
  {
    if ( !( mByteCodeBuffer.setup( byteCodeBufferLength, 0 ) ) )
      return false;

    memcpy( mByteCodeBuffer.c_str(), source->c_str_offset(), byteCodeBufferLength );
    source->seekFromCurrent( byteCodeBufferLength );
  }

  return true;
}

bool ScriptExecutable::write( crunch::core::BinaryBuffer *dest ) const
{
  if ( dest == nullptr || mVM != nullptr )
    return false;

  unsigned int versionOffset = dest->getBufferLength();
  dest->append_uint32( CRUNCH_EXECUTABLE_FILE_VERSION_INVALID );

  if ( !( mStringTable.write( dest ) ) )
    return false;

  if ( !( mLinkTable_LIB_GFUNC_CALL.write( dest ) ) )
    return false;

  if ( !( mLinkTable_PUSH_FETCH_GLIB_CLS.write( dest ) ) )
    return false;

  if ( !( mLinkTable_LIB_FUNC_CALL.write( dest ) ) )
    return false;

  if ( !( mGlobalObject.write( this, dest ) ) )
    return false;

  // objects
  {
    unsigned int totalObjects = (unsigned int)( mObjects.size() );
    dest->append_uint32( totalObjects );

    ScriptObjectSetType::const_iterator it = mObjects.cbegin();
    ScriptObjectSetType::const_iterator itEnd = mObjects.cend();
    for ( ; it != itEnd; ++it )
    {
      dest->append_String( ( *it )->getName().c_str() );

      if ( !( ( *it )->write( this, dest ) ) )
        return false;
    }
  }

  dest->append_uint32( mByteCodeBuffer.getBufferLength() );

  if ( !( dest->append_buffer( mByteCodeBuffer ) ) )
    return false;

  // update version
  {
    unsigned int currentOffset = dest->getBufferLength();
    dest->setBufferOffset( versionOffset );
    dest->append_uint32( CRUNCH_EXECUTABLE_FILE_VERSION );
    dest->setBufferOffset( currentOffset );
  }

  return true;
}

bool ScriptExecutable::readFile( const crunch::core::WideString& sourceFilePath )
{
  crunch::core::BinaryBuffer source;

  if ( !( source.readFile( sourceFilePath.wc_str() ) ) )
    return false;

  return read( &source );
}

bool ScriptExecutable::writeFile( const crunch::core::WideString& destFilePath ) const
{
  crunch::core::BinaryBuffer dest;

  if ( !( dest.setup( 1024 * 1024 * 16, 1024 * 1024 * 1 ) ) )
    return false;

  if ( !( write( &dest ) ) )
    return false;

  return dest.writeFile( destFilePath.wc_str() );
}

bool ScriptExecutable::getFunctionSignature( const crunch::core::String& name, 
                                             const std::vector< ScriptTypes::ScriptTypeDescription >& parameterTypes, 
                                             crunch::core::String *signature 
                                             )
{
  crunch::core::String s = crunch::core::String::fmt( "%s(", name.c_str() );

  unsigned int totalParameters = (unsigned int)( parameterTypes.size() );

  if ( totalParameters > 1 )
  {
    unsigned int lastIndex = totalParameters - 2;

    for ( unsigned int i = 0; i <= lastIndex; ++i )
      s += crunch::core::String::fmt( "%s,", parameterTypes[i].getTypeDescription().c_str() );
  }

  if ( totalParameters > 0 )
    s += crunch::core::String::fmt( "%s", parameterTypes[totalParameters - 1].getTypeDescription().c_str() );

  s += ")";

  *signature = s;

  return true;
}

bool ScriptExecutable::getFunctionDisplaySignature( const crunch::core::String& name, 
                                                    const std::vector< ScriptTypes::ScriptTypeDescription >& parameterTypes, 
                                                    crunch::core::String *displaySignature 
                                                    )
{
  crunch::core::String s = crunch::core::String::fmt( "%s(", name.c_str() );

  unsigned int totalParameters = (unsigned int)( parameterTypes.size() );

  if ( totalParameters > 1 )
  {
    unsigned int lastIndex = totalParameters - 2;

    for ( unsigned int i = 0; i <= lastIndex; ++i )
      s += crunch::core::String::fmt( "%s,", parameterTypes[i].getDisplayTypeDescription().c_str() );
  }

  if ( totalParameters > 0 )
    s += crunch::core::String::fmt( "%s", parameterTypes[totalParameters - 1].getDisplayTypeDescription().c_str() );

  s += ")";

  *displaySignature = s;

  return true;
}

unsigned int ScriptExecutable::getParametersDataStackSize( const std::vector< ScriptTypes::ScriptTypeDescription >& parameterTypes )
{
  unsigned int retVal = 0;

  unsigned int total = (unsigned int)( parameterTypes.size() );

  for ( unsigned int i = 0; i < total; ++i )
    retVal += ScriptTypes::getDataStackSize( parameterTypes[i] );

  return retVal;
}

bool ScriptExecutable::run( const crunch::core::String& filepath, 
                            const crunch::core::String& rootFunctionName, 
                            const std::vector< crunch::core::String >& rootFunctionParameterTypes, 
                            const std::vector< crunch::core::String >& rootFunctionParameterValues 
                            )
{
  ScriptExecutable se;

  if ( !( se.readFile( filepath ) ) )
  {
    corePrint( "[ERROR] failed to load script executable '%s'", filepath.c_str() );
    return false;
  }

  ScriptVM vm( &se );

  if ( !( vm.setup() ) )
  {
    corePrint( "[ERROR] failed to setup the vm" );
    return false;
  }

  ScriptThread *t = vm.createThread();
  if ( t == nullptr )
  {
    corePrint( "[ERROR] failed to create script thread" );
    return false;
  }

  std::vector< ScriptTypes::ScriptTypeDescription > parameterTypes;

  if ( !( ScriptTypes::getScriptTypeDescriptions( rootFunctionParameterTypes, &parameterTypes ) ) )
  {
    corePrint( "[ERROR] failed to get the parameter types" );
    return false;
  }

  if ( !( t->setRootFunction( rootFunctionName, parameterTypes ) ) )
  {
    corePrint( "[ERROR] failed to set the root function" );
    return false;
  }

  // push values for root function
  {
    unsigned int totalParams = (unsigned int)( parameterTypes.size() );

    for ( unsigned int paramIndex = 0; paramIndex < totalParams; ++paramIndex )
    {
      switch ( parameterTypes[paramIndex].mScriptType )
      {
        case ScriptTypes::ScriptType::TYPE_STRING:
        {
          crunch::core::String *s = se.stringTableMergeString( rootFunctionParameterValues[paramIndex].c_str(), false );
          t->push_string( (const crunch_string*)( s ) );
        }
        break;

        case ScriptTypes::ScriptType::TYPE_I64:
          t->push_i64( crunch_i64( crunch::core::String::to_i64( rootFunctionParameterValues[paramIndex] ) ) );
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          t->push_u64( crunch_u64( crunch::core::String::to_u64( rootFunctionParameterValues[paramIndex] ) ) );
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          t->push_f64( crunch_f64( crunch::core::String::to_f64( rootFunctionParameterValues[paramIndex] ) ) );
        break;

        default:
          corePrint( "[ERROR] unsupported parameter type for root function at index %u", paramIndex );
          return false;
      }
    }
  }

  if ( !( t->run() ) )
  {
    corePrint( "[ERROR] script thread failed to run for '%s'", filepath.c_str() );
  }

  if ( !( vm.destroyThread( t ) ) )
  {
    corePrint( "[ERROR] failed to destroy thread for '%s'", filepath.c_str() );
    return false;
  }

  return true;
}

} }
