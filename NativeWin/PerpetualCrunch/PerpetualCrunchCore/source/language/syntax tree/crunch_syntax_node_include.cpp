
#include "../../crunch_global.h"

#include "../crunch_script_compiler.h"

#include "crunch_syntax_node_root.h"

#include "crunch_syntax_node_include.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

SyntaxNodeInclude::SyntaxNodeInclude( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber, const crunch::core::String& includeItem ) :
  SyntaxNode( compiler, root, lineNumber ),
  mIncludeItem( includeItem ),
  mIncludeRoot( nullptr )
{
  // empty
}

SyntaxNodeInclude::~SyntaxNodeInclude()
{
  if ( mIncludeRoot != nullptr )
    delete mIncludeRoot;
}

crunch::core::String SyntaxNodeInclude::getSyntaxTreeDescription( const crunch::core::String& indent ) const
{
  crunch::core::String d = crunch::core::String::fmt( "%s%u: INCLUDE \"%s\"\n", indent.c_str(), mLineNumber, mIncludeItem.c_str() );

  if ( mIncludeRoot != nullptr )
    d += mIncludeRoot->getSyntaxTreeDescription( indent + mIndentStep );

  return d;
}

void SyntaxNodeInclude::applyConstantFolding()
{
  if ( mIncludeRoot != nullptr )
    mIncludeRoot->applyConstantFolding();
}

bool SyntaxNodeInclude::generateCode( crunch::core::BinaryBuffer *dest )
{
  if ( mIncludeRoot != nullptr )
    return mIncludeRoot->generateCode( dest );

  reportInternalError( "generateCode : include node missing include root" );
  return false;
}

void SyntaxNodeInclude::queryAliveStatus()
{
  if ( mIncludeRoot != nullptr )
    mIncludeRoot->queryAliveStatus();
  else
    reportInternalError( "queryAliveStatus : include node missing include root" );
}

void SyntaxNodeInclude::parseIncludeItem()
{
  if ( mIncludeRoot != nullptr )
    return;

  mCompiler->parseIncludeItem( mIncludeItem, mRoot, &mIncludeRoot );

  if ( mIncludeRoot != nullptr )
  {
    // corePrint( "found an include root" );
    mIncludeRoot->parseIncludeNodes();
  }
}

} }
