
#include "../../crunch_global.h"

#include "../crunch_script_compiler.h"

#include "crunch_syntax_node_include.h"

#include "crunch_syntax_node_root.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

SyntaxNodeRoot::SyntaxNodeRoot( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber ) :
  SyntaxNode( compiler, root, lineNumber )
{
  mRoot = this;
}

SyntaxNodeRoot::~SyntaxNodeRoot()
{
  // empty
}

crunch::core::String SyntaxNodeRoot::getSyntaxTreeDescription( const crunch::core::String& indent ) const
{
  crunch::core::String d = crunch::core::String::fmt( "%s%u: ROOT (children: %u)\n", indent.c_str(), mLineNumber, (unsigned int)( mChildren.size() ) );
  d += getSyntaxTreeChildrenDescription( indent + mIndentStep );
  return d;
}

bool SyntaxNodeRoot::generateCode( crunch::core::BinaryBuffer *dest )
{
  unsigned int totalChildren = (unsigned int)( mChildren.size() );

  for ( unsigned int childIndex = 0; childIndex < totalChildren; ++childIndex )
  {
    if ( mChildren[childIndex] == nullptr )
    {
      reportInternalError( "root node contains nullptr child" );
      return false;
    }

    if ( !( mChildren[childIndex]->generateCode( dest ) ) )
      return false;
  }

  return true;
}

void SyntaxNodeRoot::parseIncludeNodes()
{
  unsigned int totalChildren = (unsigned int)( mChildren.size() );

  for ( unsigned int childIndex = 0; childIndex < totalChildren; ++childIndex )
  {
    if ( mChildren[childIndex] != nullptr && mChildren[childIndex]->getType() == NodeType::NODE_INCLUDE )
    {
      SyntaxNodeInclude *includeNode = (SyntaxNodeInclude*)( mChildren[childIndex] );
      includeNode->parseIncludeItem();
    }
  }
}

} }
