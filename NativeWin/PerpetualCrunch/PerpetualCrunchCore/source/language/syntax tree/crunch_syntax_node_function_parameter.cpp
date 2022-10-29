
#include "../../crunch_global.h"

#include "../crunch_script_compiler.h"
#include "../crunch_symbol_stack.h"

#include "crunch_syntax_node_function_parameter.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

SyntaxNodeFunctionParameter::SyntaxNodeFunctionParameter( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber, SyntaxNode *typeIdentifier, const crunch::core::String& identifier ) :
  SyntaxNode( compiler, root, lineNumber ),
  mIdentifier( identifier )
{
  append( typeIdentifier );
}

SyntaxNodeFunctionParameter::~SyntaxNodeFunctionParameter()
{
  // empty
}

crunch::core::String SyntaxNodeFunctionParameter::getSyntaxTreeDescription( const crunch::core::String& indent ) const
{
  crunch::core::String d = crunch::core::String::fmt( "%s%u: FUNCTION PARAMETER %s (children: %u)\n", indent.c_str(), mLineNumber, mIdentifier.c_str(), (unsigned int)( mChildren.size() ) );
  d += getSyntaxTreeChildrenDescription( indent + mIndentStep );
  return d;
}

bool SyntaxNodeFunctionParameter::generateCode( crunch::core::BinaryBuffer *dest )
{
  if ( mChildren.size() != 1 )
  {
    reportInternalError( "unexpected number of children in function parameter node" );
    return false;
  }

  if ( mChildren[0] == nullptr || mChildren[0]->getType() != NodeType::NODE_TYPE_IDENTIFIER )
  {
    reportInternalError( "unexpected type node for function parameter" );
    return false;
  }

  switch ( mCompiler->getContextStack()->getContext() )
  {
    case ScriptContextStack::Context::BEGIN_SCOPE:
    {
      // push symbol to scope (parameter is already on the stack)

      mCompiler->getSymbolStack()->pushSymbol( mIdentifier, (SyntaxNodeTypeIdentifier*)( mChildren[0] ) );
    }
    break;

    default:
      reportInternalError( "unexpected script context for function parameter" );
      return false;
  }

  return true;
}

ScriptTypes::ScriptTypeDescription SyntaxNodeFunctionParameter::getScriptTypeDescription() const
{
  if ( mChildren.size() == 1 && mChildren[0] != nullptr )
    return mChildren[0]->getScriptTypeDescription();

  return ScriptTypes::ScriptTypeDescription();
}

} }
