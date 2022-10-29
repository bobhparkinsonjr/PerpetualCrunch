
#include "../crunch_global.h"

#include "../core/core_text_file.h"

#include "library/libclass_functions_global.h"
#include "library/libclass_functions_string128.h"
#include "library/libclass_functions_f64v3.h"
#include "library/libclass_functions_f64v4.h"

#include "crunch_script_library_tools.h"
#include "crunch_script_executable.h"

#include "crunch_script_library.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

///////////////////////////////////////////////////////////////////////////////////////////////////

bool ScriptLibrary::mSetup = false;
ScriptLibraryClass* ScriptLibrary::mGlobalLibraryClass = nullptr;
ScriptLibrary::ScriptLibraryClassSetType ScriptLibrary::mLibraryClasses;

///////////////////////////////////////////////////////////////////////////////////////////////////

void ScriptLibrary::setup()
{
  if ( !mSetup )
  {
    mLibraryClasses.insert( new ScriptLibraryClass_string128() );
    mLibraryClasses.insert( new ScriptLibraryClass_f64v3() );
    mLibraryClasses.insert( new ScriptLibraryClass_f64v4() );

    mGlobalLibraryClass = new ScriptLibraryClass_GlobalLibraryClass();

    for ( ScriptLibraryClass *c : mLibraryClasses )
      c->setup();

    mGlobalLibraryClass->setup();

    mSetup = true;
  }
}

void ScriptLibrary::setup( ScriptCompiler *compiler, std::vector< SyntaxNode* > *trackedNodes )
{
  if ( compiler != nullptr && trackedNodes != nullptr )
  {
    for ( ScriptLibraryClass *c : mLibraryClasses )
      c->createGlobalSymbols( compiler, trackedNodes );

    if ( mGlobalLibraryClass != nullptr )
      mGlobalLibraryClass->createGlobalSymbols( compiler, trackedNodes );
  }
}

void ScriptLibrary::destroy()
{
  if ( mSetup )
  {
    // destroy non-global classes
    {
      ScriptLibraryClassSetType::iterator it = mLibraryClasses.begin();
      ScriptLibraryClassSetType::iterator itEnd = mLibraryClasses.end();
      for ( ; it != itEnd; ++it )
        delete *it;
      mLibraryClasses.clear();
    }

    if ( mGlobalLibraryClass != nullptr )
    {
      delete mGlobalLibraryClass;
      mGlobalLibraryClass = nullptr;
    }

    mSetup = false;
  }
}

bool ScriptLibrary::hasLibraryClass( const crunch::core::FixedString64& name )
{
  ScriptLibraryClass findMe;
  findMe.setName( name );

  return ( mLibraryClasses.find( &findMe ) != mLibraryClasses.end() );
}

const ScriptLibraryClass* ScriptLibrary::findLibraryClass( const crunch::core::FixedString64& name )
{
  ScriptLibraryClass findMe;
  findMe.setName( name );

  ScriptLibraryClassSetType::const_iterator findIt = mLibraryClasses.find( &findMe );

  if ( findIt != mLibraryClasses.end() )
  {
    const ScriptLibraryClass *found = (const ScriptLibraryClass*)( *findIt );
    return found;
  }

  return nullptr;
}

void ScriptLibrary::consolePrintAllDocumentation()
{
  #if defined CRUNCH_LIBRARY_ENABLE_DOCUMENTATION

  if ( mGlobalLibraryClass != nullptr )
    mGlobalLibraryClass->consolePrintAllDocumentation();

  ScriptLibraryClassSetType::const_iterator it = mLibraryClasses.cbegin();
  ScriptLibraryClassSetType::const_iterator itEnd = mLibraryClasses.cend();
  for ( ; it != itEnd; ++it )
    ( *it )->consolePrintAllDocumentation();

  #endif
}

void ScriptLibrary::consolePrintClassDocumentation( const crunch::core::String& className )
{
  #if defined CRUNCH_LIBRARY_ENABLE_DOCUMENTATION

  const ScriptLibraryClass *foundClass = findLibraryClass( className.c_str() );

  if ( foundClass != nullptr )
    foundClass->consolePrintAllDocumentation();

  #endif
}

void ScriptLibrary::consolePrintFunctionDocumentation( const crunch::core::String& className, const crunch::core::String& functionName )
{
  #if defined CRUNCH_LIBRARY_ENABLE_DOCUMENTATION

  const ScriptLibraryClass *foundClass = findLibraryClass( className.c_str() );

  if ( foundClass != nullptr )
    foundClass->consolePrintFunctionDocumentation( functionName );

  #endif
}

void ScriptLibrary::consolePrintFunctionSignatureDocumentation( const crunch::core::String& className, const crunch::core::String& functionSignature )
{
  #if defined CRUNCH_LIBRARY_ENABLE_DOCUMENTATION

  const ScriptLibraryClass *foundClass = findLibraryClass( className.c_str() );

  if ( foundClass != nullptr )
    foundClass->consolePrintFunctionSignatureDocumentation( functionSignature );

  #endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////

} }
