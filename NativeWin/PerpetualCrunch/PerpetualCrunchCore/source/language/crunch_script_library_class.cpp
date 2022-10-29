
#include "../crunch_global.h"

#include "crunch_script_executable.h"
#include "crunch_script_library_function.h"

#include "crunch_script_library_class.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

ScriptLibraryClass::ScriptLibraryClass( const crunch::core::FixedString64& name, unsigned int dataStackSize ) :
  mName( name ),
  mDataStackSize( dataStackSize )
{
  assert( dataStackSize <= ScriptTypes::TYPE_MAX_SIZE_BYTES && "need to update TYPE_MAX_SIZE_BYTES" );
}

ScriptLibraryClass::~ScriptLibraryClass()
{
  destroy();
}

void ScriptLibraryClass::destroy()
{
  // delete the functions
  {
    FunctionSetType::iterator it = mFunctions.begin();
    FunctionSetType::iterator itEnd = mFunctions.end();
    for ( ; it != itEnd; ++it )
      delete *it;
    mFunctions.clear();
  }

  mName = "";
  mDataStackSize = 0;
}

bool ScriptLibraryClass::hasFunctionSignature( const crunch::core::String& signature ) const
{
  ScriptLibraryFunction findMe;
  findMe.setSignature( signature );

  return ( mFunctions.find( &findMe ) != mFunctions.end() );
}

bool ScriptLibraryClass::add( ScriptLibraryFunction *function )
{
  if ( function == nullptr || function->getOwner() != nullptr )
    return false;

  bool retVal = mFunctions.insert( function ).second;

  if ( !retVal )
  {
    corePrint( "[ERROR] failed to add library function '%s' to class '%s'", function->getSignature().c_str(), mName.c_str() );
    delete function;
  }
  else
    function->setOwner( this );

  // corePrint( "added library function '%s' to class '%s' with return type '%s'", function->getSignature().c_str(), mName.c_str(), function->getReturnType().getTypeDescription().c_str() );
  return retVal;
}

const ScriptLibraryFunction* ScriptLibraryClass::findFunction( const crunch::core::String& signature ) const
{
  ScriptLibraryFunction findMe;
  findMe.setSignature( signature );

  FunctionSetType::const_iterator findIt = mFunctions.find( &findMe );

  if ( findIt != mFunctions.end() )
    return (const ScriptLibraryFunction*)( *findIt );

  return nullptr;
}

bool ScriptLibraryClass::setMemberInfo( const crunch::core::String& memberName, ScriptTypes::ScriptType v, const crunch::core::String& typeIdentifier, unsigned int offset, unsigned int size )
{
  if ( memberName == "" )
    return false;

  MemberInfo m;
  m.mMemberName = memberName;
  m.mType = v;
  m.mTypeIdentifier = typeIdentifier;
  m.mOffset = offset;
  m.mSize = size;

  std::pair< MemberInfoSetType::iterator, bool > res = mMembers.insert( m );

  if ( !( res.second ) )
  {
    corePrint( "[ERROR] failed to add member '%s' to library class '%s'", memberName.c_str(), mName.c_str() );
  }

  return res.second;
}

bool ScriptLibraryClass::getMemberInfo( const crunch::core::String& memberName, ScriptTypes::ScriptType *v, crunch::core::String *typeIdentifier, unsigned int *offset, unsigned int *size ) const
{
  MemberInfo findMe;
  findMe.mMemberName = memberName;

  MemberInfoSetType::iterator findIt = mMembers.find( findMe );

  if ( findIt != mMembers.end() )
  {
    *v = findIt->mType;
    *typeIdentifier = findIt->mTypeIdentifier;
    *offset = findIt->mOffset;
    *size = findIt->mSize;
    return true;
  }

  return false;
}

bool ScriptLibraryClass::hasMembers() const
{
  return ( !( mMembers.empty() ) );
}

crunch::core::String ScriptLibraryClass::generateMemberMetaInfo() const
{
  crunch::core::String r;

  MemberInfoSetType::const_iterator it = mMembers.cbegin();
  MemberInfoSetType::const_iterator itEnd = mMembers.cend();
  for ( ; it != itEnd; ++it )
  {
    const MemberInfo& m = *it;

    if ( m.mTypeIdentifier != "" )
      r += crunch::core::String::fmt( "    %s %s;\n", m.mTypeIdentifier.c_str(), m.mMemberName.c_str() );
    else
      r += crunch::core::String::fmt( "    %s %s;\n", ScriptTypes::getScriptTypeKeyword( m.mType ).c_str(), m.mMemberName.c_str() );
  }

  return r;
}

#if defined CRUNCH_LIBRARY_ENABLE_DOCUMENTATION
void ScriptLibraryClass::consolePrintAllDocumentation() const
{
  printf( "///////////////////////////////////////////////////////////////////////////////////////////////////\n" );
  printf( "class %s\n", mName.c_str() );
  printf( "  %s\n\n", mDocumentation.c_str() );

  FunctionSetType::const_iterator it = mFunctions.cbegin();
  FunctionSetType::const_iterator itEnd = mFunctions.cend();

  for ( ; it != itEnd; ++it )
  {
    printf( "  %s %s\n", ( *it )->getReturnType().getDisplayTypeDescription().c_str(), ( *it )->getDisplaySignature().c_str() );
    printf( "    %s\n\n", ( *it )->getDocumentation().c_str() );
  }
}
#endif

#if defined CRUNCH_LIBRARY_ENABLE_DOCUMENTATION
void ScriptLibraryClass::consolePrintFunctionDocumentation( const crunch::core::String& functionName ) const
{
  FunctionSetType::const_iterator it = mFunctions.cbegin();
  FunctionSetType::const_iterator itEnd = mFunctions.cend();

  for ( ; it != itEnd; ++it )
  {
    if ( ( *it )->findName() == functionName )
    {
      printf( "%s %s\n", ( *it )->getReturnType().getDisplayTypeDescription().c_str(), ( *it )->getDisplaySignature().c_str() );
      printf( "  %s\n\n", ( *it )->getDocumentation().c_str() );
    }
  }  
}
#endif

#if defined CRUNCH_LIBRARY_ENABLE_DOCUMENTATION
void ScriptLibraryClass::consolePrintFunctionSignatureDocumentation( const crunch::core::String& functionSignature ) const
{
  const ScriptLibraryFunction *foundFunction = findFunction( functionSignature );

  if ( foundFunction != nullptr )
  {
    printf( "%s %s\n", foundFunction->getReturnType().getDisplayTypeDescription().c_str(), foundFunction->getDisplaySignature().c_str() );
    printf( "  %s\n\n", foundFunction->getDocumentation().c_str() );
  }
}
#endif

} }
