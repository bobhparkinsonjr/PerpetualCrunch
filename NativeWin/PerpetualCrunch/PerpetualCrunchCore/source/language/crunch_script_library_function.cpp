
#include "../crunch_global.h"

#include "crunch_script_executable.h"
#include "crunch_script_library.h"

#include "crunch_script_library_function.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

ScriptLibraryFunction::ScriptLibraryFunction()
{
  // empty
}

ScriptLibraryFunction::~ScriptLibraryFunction()
{
  // empty
}

ScriptLibraryFunction* ScriptLibraryFunction::setup( const ScriptLibraryClass *owner,
                                                     const crunch::core::String& name, 
                                                     const std::vector< ScriptTypes::ScriptTypeDescription >& parameterTypes, 
                                                     ScriptTypes::ScriptTypeDescription returnType
                                                     )
{
  mParameterTypes = parameterTypes;
  mReturnType = returnType;

  if ( !( ScriptExecutable::getFunctionSignature( name, parameterTypes, &mSignature ) ) )
  {
    corePrint( "ERROR: invalid function signature, name: '%s'", name.c_str() );
  }

  #if defined CRUNCH_LIBRARY_ENABLE_DOCUMENTATION
  if ( !( ScriptExecutable::getFunctionDisplaySignature( name, parameterTypes, &mDisplaySignature ) ) )
  {
    corePrint( "ERROR: invalid function display signature, name: '%s'", name.c_str() );
  }
  #endif

  mParametersDataStackSize = ScriptExecutable::getParametersDataStackSize( parameterTypes );

  if ( returnType.mIdentifier != "" )
  {
    const ScriptLibraryClass *slc;

    if ( owner != nullptr && owner->getName() == crunch::core::FixedString64( returnType.mIdentifier.c_str() ) )
      slc = owner;
    else
      slc = ScriptLibrary::findLibraryClass( returnType.mIdentifier.c_str() );

    if ( slc != nullptr )
      mReturnTypeDataStackSize = slc->getDataStackSize();
    else
    {
      corePrint( "ERROR: return type library class '%s' not found for library function '%s'", returnType.mIdentifier.c_str(), mSignature.c_str() );
    }
  }
  else
    mReturnTypeDataStackSize = ScriptTypes::getDataStackSize( mReturnType );

  return this;
}

crunch::core::String ScriptLibraryFunction::findName() const
{
  int i = int( mSignature.find( '(' ) );
  if ( i > 0 )
    return mSignature.substr( 0, i );
  return mSignature;
}

} }
