
#include "../../crunch_global.h"

#include "crunch_syntax_node_function_parameter_list.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

SyntaxNodeFunctionParameterList::SyntaxNodeFunctionParameterList( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber ) :
  SyntaxNode( compiler, root, lineNumber )
{
  // empty
}

SyntaxNodeFunctionParameterList::~SyntaxNodeFunctionParameterList()
{
  // empty
}

crunch::core::String SyntaxNodeFunctionParameterList::getSyntaxTreeDescription( const crunch::core::String& indent ) const
{
  crunch::core::String d = crunch::core::String::fmt( "%s%u: FUNCTION PARAMETER LIST (children: %u)\n", indent.c_str(), mLineNumber, (unsigned int)( mChildren.size() ) );
  d += getSyntaxTreeChildrenDescription( indent + mIndentStep );
  return d;
}

bool SyntaxNodeFunctionParameterList::generateCode( crunch::core::BinaryBuffer *dest )
{
  return generateChildrenCode( dest );
}

} }
