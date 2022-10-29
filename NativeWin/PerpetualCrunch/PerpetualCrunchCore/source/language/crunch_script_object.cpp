
#include "../crunch_global.h"

#include "crunch_script_compiler.h"
#include "crunch_script_instruction.h"
#include "crunch_script_executable.h"
#include "syntax tree/crunch_syntax_node_function_declaration.h"

#include "crunch_script_object.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#define CRUNCH_SCRIPT_OBJECT_VERSION 1

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

ScriptObject::ScriptObject( const crunch::core::FixedString64& name ) :
  mName( name )
{
  // empty
}

ScriptObject::~ScriptObject()
{
  destroy();
}

void ScriptObject::destroy()
{
  mName = "";
  clear();
}

void ScriptObject::clear()
{
  clearProperties();
  clearFunctions();
}

bool ScriptObject::isAllowedPropertyType( ScriptTypes::ScriptType t )
{
  switch ( t )
  {
    case ScriptTypes::ScriptType::TYPE_STRING:
    case ScriptTypes::ScriptType::TYPE_I64:
    case ScriptTypes::ScriptType::TYPE_U64:
    case ScriptTypes::ScriptType::TYPE_F64:
      return true;

    default:
      // empty
    break;
  }

  return false;
}

bool ScriptObject::hasProperty( const crunch::core::FixedString64& name ) const
{
  return ( mProperties.find( PropertyInfo( name ) ) != mProperties.end() );
}

bool ScriptObject::addProperty( const crunch::core::FixedString64& name, const ScriptValue& v )
{
  if ( !( isAllowedPropertyType( v.getType() ) ) )
    return false;

  return mProperties.insert( PropertyInfo( name, v ) ).second;
}

bool ScriptObject::removeProperty( const crunch::core::FixedString64& name )
{
  PropertyInfoSetType::iterator it = mProperties.find( PropertyInfo( name ) );
  if ( it == mProperties.end() )
    return false;

  mProperties.erase( it );

  return true;
}

bool ScriptObject::getPropertyValue( const crunch::core::FixedString64& name, ScriptValue *v ) const
{
  if ( v == nullptr )
    return false;

  PropertyInfoSetType::const_iterator it = mProperties.find( PropertyInfo( name ) );
  if ( it != mProperties.end() )
  {
    *v = it->mValue;
    return true;
  }

  return false;
}

bool ScriptObject::setPropertyValue( const crunch::core::FixedString64& name, const ScriptValue& v )
{
  if ( !( isAllowedPropertyType( v.getType() ) ) )
    return false;

  PropertyInfoSetType::iterator it = mProperties.find( PropertyInfo( name ) );
  if ( it != mProperties.end() )
  {
    PropertyInfo *myInfo = (PropertyInfo*)( &( *it ) );
    myInfo->mValue = v;
    return true;
  }

  return false;
}

void ScriptObject::clearProperties()
{
  mProperties.clear();
}

bool ScriptObject::isAllowedFunctionName( const crunch::core::String& name )
{
  return ( name.size() > 0 && int( name.find( "(" ) ) < 0 && int( name.find( ")" ) ) < 0 );
}

bool ScriptObject::hasFunctionSignature( const crunch::core::String& signature ) const
{
  FunctionInfo findMe;
  findMe.mSignature = signature;

  return ( mFunctions.find( &findMe ) != mFunctions.end() );
}

bool ScriptObject::addFunction( const crunch::core::String& name, 
                                unsigned int offset, 
                                const std::vector< ScriptTypes::ScriptTypeDescription >& parameterTypes, 
                                ScriptTypes::ScriptTypeDescription returnType,
                                SyntaxNodeFunctionDeclaration *declaration
                                )
{
  return ( add( name, offset, parameterTypes, returnType, declaration ) != nullptr );
}

bool ScriptObject::getFunctionInfo( const crunch::core::String& signature, 
                                    unsigned int *offset, 
                                    ScriptTypes::ScriptTypeDescription *returnType, 
                                    unsigned int *paramsStackSize,
                                    SyntaxNodeFunctionDeclaration **declaration
                                    ) const
{
  FunctionInfo findMe;
  findMe.mSignature = signature;

  FunctionInfoSetType::const_iterator it = mFunctions.find( &findMe );
  if ( it != mFunctions.end() )
  {
    if ( offset != nullptr )
      *offset = ( *it )->mOffset;

    if ( returnType != nullptr )
      *returnType = ( *it )->mReturnType;

    if ( paramsStackSize != nullptr )
    {
      *paramsStackSize = 0;
      
      unsigned int totalParams = (unsigned int)( ( *it )->mParameterTypes.size() );
      for ( unsigned int i = 0; i < totalParams; ++i )
        *paramsStackSize += ( ScriptTypes::getDataStackSize( ( *it )->mParameterTypes[i] ) );
    }

    if ( declaration != nullptr )
      *declaration = ( *it )->mDeclaration;

    return true;
  }

  // corePrint( "function '%s' not found", signature.c_str() );
  return false;
}

bool ScriptObject::addFunctionDependencies( const crunch::core::String& signature, ScriptObject *sourceObject, const crunch::core::String& sourceFunctionSignature )
{
  if ( sourceObject == nullptr )
    return false;

  FunctionInfo findSource;
  findSource.mSignature = sourceFunctionSignature;

  FunctionInfoSetType::const_iterator sit = sourceObject->mFunctions.find( &findSource );
  if ( sit == sourceObject->mFunctions.end() )
    return false;

  return true;
}

void ScriptObject::clearFunctions()
{
  FunctionInfoSetType::iterator it = mFunctions.begin();
  FunctionInfoSetType::iterator itEnd = mFunctions.end();
  for ( ; it != itEnd; ++it )
    delete ( *it );

  mFunctions.clear();
}

bool ScriptObject::read( const ScriptExecutable *se, crunch::core::BinaryBuffer *source )
{
  if ( source == nullptr )
    return false;

  unsigned int version = source->read_uint32();

  if ( version < 1 || version > CRUNCH_SCRIPT_OBJECT_VERSION )
    return false;

  mName = source->read_FixedString64();

  // properties
  {
    mProperties.clear();

    unsigned int totalProperties = source->read_uint32();

    for ( unsigned int propIndex = 0; propIndex < totalProperties; ++propIndex )
    {
      crunch::core::FixedString64 name = source->read_FixedString64();
      ScriptValue v;
      if ( !( v.read( se, source ) ) )
        return false;

      mProperties.insert( PropertyInfo( name, v ) );
    }
  }

  // functions
  {
    mFunctions.clear();

    unsigned int totalFunctions = source->read_uint32();

    for ( unsigned int funcIndex = 0; funcIndex < totalFunctions; ++funcIndex )
    {
      crunch::core::String name = source->read_String();
      unsigned int offset = source->read_uint32();

      std::vector< ScriptTypes::ScriptTypeDescription > parameterTypes;

      unsigned int totalParameters = source->read_uint32();
      parameterTypes.resize( totalParameters );
      for ( unsigned int paramIndex = 0; paramIndex < totalParameters; ++paramIndex )
      {
        parameterTypes[paramIndex].mScriptType = (ScriptTypes::ScriptType)( source->read_int32() );
        parameterTypes[paramIndex].mIdentifier = source->read_String();
      }

      ScriptTypes::ScriptTypeDescription returnType;
      returnType.mScriptType = (ScriptTypes::ScriptType)( source->read_int32() );
      returnType.mIdentifier = source->read_String();

      FunctionInfo *fi = add( name, offset, parameterTypes, returnType );

      if ( fi == nullptr )
        return false;
    }
  }

  return true;
}

bool ScriptObject::write( const ScriptExecutable *se, crunch::core::BinaryBuffer *dest ) const
{
  if ( dest == nullptr )
    return false;

  dest->append_uint32( CRUNCH_SCRIPT_OBJECT_VERSION );

  dest->append_FixedString64( mName );

  // properties
  {
    unsigned int totalProperties = (unsigned int)( mProperties.size() );

    dest->append_uint32( totalProperties );

    PropertyInfoSetType::const_iterator it = mProperties.cbegin();
    PropertyInfoSetType::const_iterator itEnd = mProperties.cend();

    for ( ; it != itEnd; ++it )
    {
      dest->append_FixedString64( it->mName );
      if ( !( it->mValue.write( se, dest ) ) )
        return false;
    }
  }

  // functions
  {
    unsigned int totalFunctions = (unsigned int)( mFunctions.size() );

    dest->append_uint32( totalFunctions );

    FunctionInfoSetType::const_iterator it = mFunctions.cbegin();
    FunctionInfoSetType::const_iterator itEnd = mFunctions.cend();

    for ( ; it != itEnd; ++it )
    {
      FunctionInfo *info = *it;

      dest->append_String( info->mName.c_str() );
      dest->append_uint32( info->mOffset );

      unsigned int totalParameters = (unsigned int)( info->mParameterTypes.size() );
      dest->append_uint32( totalParameters );
      for ( unsigned int paramIndex = 0; paramIndex < totalParameters; ++paramIndex )
      {
        dest->append_int32( (int)( info->mParameterTypes[paramIndex].mScriptType ) );
        dest->append_String( info->mParameterTypes[paramIndex].mIdentifier.c_str() );
      }

      dest->append_int32( (int)( info->mReturnType.mScriptType ) );
      dest->append_String( info->mReturnType.mIdentifier.c_str() );
    }
  }

  return true;
}

void ScriptObject::appendAssembler( crunch::core::String *dest ) const
{
  if ( dest == nullptr )
    return;

  *dest += crunch::core::String::fmt( "SCRIPT OBJECT '%s'\n", mName.c_str() );

  // properties
  {
    *dest += crunch::core::String::fmt( "  PROPERTIES (total:%u)\n", (unsigned int)( mProperties.size() ) );

    PropertyInfoSetType::const_iterator it = mProperties.cbegin();
    PropertyInfoSetType::const_iterator itEnd = mProperties.cend();

    for ( ; it != itEnd; ++it )
      *dest += crunch::core::String::fmt( "    '%s' : '%s'\n", it->mName.c_str(), it->mValue.getSourceDescription().c_str() );
  }

  // functions
  {
    *dest += crunch::core::String::fmt( "  FUNCTIONS (total:%u)\n", (unsigned int)( mFunctions.size() ) );

    FunctionInfoSetType::const_iterator it = mFunctions.cbegin();
    FunctionInfoSetType::const_iterator itEnd = mFunctions.cend();

    for ( ; it != itEnd; ++it )
    {
      const FunctionInfo *fi = *it;

      *dest += crunch::core::String::fmt( "    Function '%s' (offset:%u)\n", fi->mSignature.c_str(), fi->mOffset );
    }
  }
}

ScriptObject::FunctionInfo* ScriptObject::add( const crunch::core::String& name, 
                                               unsigned int offset, 
                                               const std::vector< ScriptTypes::ScriptTypeDescription >& parameterTypes, 
                                               ScriptTypes::ScriptTypeDescription returnType,
                                               SyntaxNodeFunctionDeclaration *declaration
                                               )
{
  if ( !( isAllowedFunctionName( name ) ) )
  {
    // corePrint( "not allowed function name: '%s'", name.c_str() );
    return nullptr;
  }

  FunctionInfo *myFunctionInfo = new FunctionInfo();

  if ( !( ScriptExecutable::getFunctionSignature( name, parameterTypes, &( myFunctionInfo->mSignature ) ) ) )
  {
    // corePrint( "failed to generate a function signature for function name: '%s'", name.c_str() );
    delete myFunctionInfo;
    return nullptr;
  }

  myFunctionInfo->mName = name;
  myFunctionInfo->mOffset = offset;
  myFunctionInfo->mParameterTypes = parameterTypes;
  myFunctionInfo->mReturnType = returnType;
  myFunctionInfo->mDeclaration = declaration;

  std::pair< FunctionInfoSetType::iterator, bool > ret = mFunctions.insert( myFunctionInfo );

  if ( !( ret.second ) )
  {
    // corePrint( "failed to insert function info for function name: '%s'", name.c_str() );
    delete myFunctionInfo;
    return nullptr;
  }

  return myFunctionInfo;
}

} }
