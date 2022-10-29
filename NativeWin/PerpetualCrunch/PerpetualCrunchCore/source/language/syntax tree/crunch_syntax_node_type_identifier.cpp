
#include "../../crunch_global.h"

#include "../crunch_script_compiler.h"
#include "../crunch_script_library.h"

#include "crunch_syntax_node_type_identifier.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

SyntaxNodeTypeIdentifier::SyntaxNodeTypeIdentifier( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber, ScriptTypes::ScriptType theType, const crunch::core::String& identifier ) :
  SyntaxNode( compiler, root, lineNumber )
{
  mScriptTypeDescription.mScriptType = theType;
  mScriptTypeDescription.mIdentifier = identifier;
}

SyntaxNodeTypeIdentifier::~SyntaxNodeTypeIdentifier()
{
  // empty
}

crunch::core::String SyntaxNodeTypeIdentifier::getSyntaxTreeDescription( const crunch::core::String& indent ) const
{
  crunch::core::String prefix = crunch::core::String::fmt( "%s%u: TYPE IDENTIFIER ", indent.c_str(), mLineNumber );

  if ( mScriptTypeDescription.mIdentifier != "" )
    return ( prefix + mScriptTypeDescription.mIdentifier + "\n" );

  return ( prefix + ScriptTypes::getScriptTypeName( mScriptTypeDescription.mScriptType ) + "\n" );
}

unsigned int SyntaxNodeTypeIdentifier::getDataStackSize() const
{
  resolveScriptType();
  return ScriptTypes::getDataStackSize( mScriptTypeDescription );
}

void SyntaxNodeTypeIdentifier::resolveScriptType() const
{
  if ( mScriptTypeDescription.mScriptType != ScriptTypes::ScriptType::TYPE_NONE || mScriptTypeDescription.mIdentifier == "" )
    return;

  if ( mScriptTypeDescription.mIdentifier.size() > ID_MAX_LENGTH )
  {
    reportError( "type identifier '%s' too long, max length is %d characters", mScriptTypeDescription.mIdentifier.c_str(), ID_MAX_LENGTH );
    return;
  }

  if ( ScriptLibrary::hasLibraryClass( mScriptTypeDescription.mIdentifier.c_str() ) )
    mScriptTypeDescription.mScriptType = ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS;
}

} }
