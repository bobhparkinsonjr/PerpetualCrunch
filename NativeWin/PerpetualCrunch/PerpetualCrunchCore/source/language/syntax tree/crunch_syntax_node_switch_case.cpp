
#include "../../crunch_global.h"

#include "crunch_syntax_node_switch_case.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

SyntaxNodeSwitchCase::SyntaxNodeSwitchCase( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber, SyntaxNode *expression, SyntaxNode *statementList ) :
  SyntaxNode( compiler, root, lineNumber )
{
  append( expression );
  append( statementList );
}

SyntaxNodeSwitchCase::~SyntaxNodeSwitchCase()
{
  // empty
}

crunch::core::String SyntaxNodeSwitchCase::getSyntaxTreeDescription( const crunch::core::String& indent ) const
{
  crunch::core::String d = crunch::core::String::fmt( "%s%u: SWITCH CASE (children: %u)\n", indent.c_str(), mLineNumber, (unsigned int)( mChildren.size() ) );
  d += getSyntaxTreeChildrenDescription( indent + mIndentStep );
  return d;
}

bool SyntaxNodeSwitchCase::generateCode( crunch::core::BinaryBuffer *dest )
{
  unsigned int totalChildren = (unsigned int)( mChildren.size() );

  if ( totalChildren != ChildType::TOTAL )
  {
    reportInternalError( "switch case has unexpected number of children" );
    return false;
  }

  if ( mChildren[ChildType::STATEMENT_LIST] != nullptr )
  {
    if ( !( mChildren[ChildType::STATEMENT_LIST]->generateCode( dest ) ) )
      return false;
  }

  return true;
}

bool SyntaxNodeSwitchCase::generateExpressionValue( crunch_i64 *expressionValue )
{
  if ( expressionValue == nullptr )
  {
    reportInternalError( "missing expression value for case" );
    return false;
  }

  if ( mChildren[ChildType::EXPRESSION] == nullptr )
    return false; // this indicates that we are the default case

  ScriptValue v = mChildren[ChildType::EXPRESSION]->getScriptValue();

  if ( v.getType() != ScriptTypes::ScriptType::TYPE_I64 )
  {
    reportError( "expected compile-time constant i64 type for case expression" );
    return false;
  }

  *expressionValue = v.getI64();
  return true;
}

} }
