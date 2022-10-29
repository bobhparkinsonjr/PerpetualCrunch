
#include "../../crunch_global.h"

#include "../crunch_script_compiler.h"

#include "crunch_syntax_node_expression_list.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

SyntaxNodeExpressionList::SyntaxNodeExpressionList( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber ) :
  SyntaxNode( compiler, root, lineNumber )
{
  // empty
}

SyntaxNodeExpressionList::~SyntaxNodeExpressionList()
{
  // empty
}

crunch::core::String SyntaxNodeExpressionList::getSyntaxTreeDescription( const crunch::core::String& indent ) const
{
  crunch::core::String d = crunch::core::String::fmt( "%s%u: EXPRESSION LIST (children: %u)\n", indent.c_str(), mLineNumber, (unsigned int)( mChildren.size() ) );
  d += getSyntaxTreeChildrenDescription( indent + mIndentStep );
  return d;
}

void SyntaxNodeExpressionList::applyConstantFolding()
{
  int totalChildren = int( mChildren.size() );

  for ( int childIndex = 0; childIndex < totalChildren; ++childIndex )
  {
    if ( mChildren[childIndex] == nullptr || mChildren[childIndex]->getType() == NodeType::NODE_LITERAL_TYPE )
      continue;

    ScriptValue v = mChildren[childIndex]->getScriptValue();

    if ( v.isConstantFoldType() )
    {
      unsigned int lineNumber = mChildren[childIndex]->getSourceLineNumber();

      delete mChildren[childIndex];

      mChildren[childIndex] = new SyntaxNodeLiteralType( mCompiler, mRoot, lineNumber, v );
    }
    else
      mChildren[childIndex]->applyConstantFolding();
  }
}

bool SyntaxNodeExpressionList::generateCode( crunch::core::BinaryBuffer *dest )
{
  SCOPED_SCRIPT_CONTEXT( FETCH );
  return generateChildrenCode( dest );
}

} }
