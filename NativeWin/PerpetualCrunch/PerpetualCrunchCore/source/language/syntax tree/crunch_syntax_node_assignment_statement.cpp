
#include "../../crunch_global.h"

#include "../crunch_script_compiler.h"

#include "crunch_syntax_node_assignment_statement.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

SyntaxNodeAssignmentStatement::SyntaxNodeAssignmentStatement( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber, SyntaxNode *lhs, SyntaxNode *rhs ) :
  SyntaxNode( compiler, root, lineNumber )
{
  if ( lhs == nullptr )
    reportInternalError( "assignment lhs missing" );

  if ( rhs == nullptr )
    reportInternalError( "assignment rhs missing" );

  append( lhs );
  append( rhs );
}

SyntaxNodeAssignmentStatement::~SyntaxNodeAssignmentStatement()
{
  // empty
}

crunch::core::String SyntaxNodeAssignmentStatement::getSyntaxTreeDescription( const crunch::core::String& indent ) const
{
  crunch::core::String d = crunch::core::String::fmt( "%s%u: ASSIGNMENT STATEMENT (children: %u)\n", indent.c_str(), mLineNumber, (unsigned int)( mChildren.size() ) );
  d += getSyntaxTreeChildrenDescription( indent + mIndentStep );
  return d;
}

void SyntaxNodeAssignmentStatement::applyConstantFolding()
{
  if ( mChildren.size() != 2 || mChildren[0] == nullptr || mChildren[1] == nullptr || mChildren[1]->getType() == NodeType::NODE_LITERAL_TYPE )
    return;

  ScriptValue v = mChildren[1]->getScriptValue();

  if ( v.isConstantFoldType() )
  {
    unsigned int lineNumber = mChildren[1]->getSourceLineNumber();

    delete mChildren[1];

    mChildren[1] = new SyntaxNodeLiteralType( mCompiler, mRoot, lineNumber, v );
  }
  else
    mChildren[1]->applyConstantFolding();
}

bool SyntaxNodeAssignmentStatement::generateCode( crunch::core::BinaryBuffer *dest )
{
  if ( mChildren.size() != 2 || mChildren[0] == nullptr || mChildren[1] == nullptr )
  {
    reportInternalError( "unexpected children nodes for assignment node" );
    return false;
  }

  if ( mChildren[0]->getScriptTypeDescription() != mChildren[1]->getScriptTypeDescription() )
  {
    reportError( "type mismatch in assignment" );
    return false;
  }

  if ( !( mChildren[0]->isLValue() ) )
  {
    reportError( "lhs of assignment requires an l-value" );
    return false;
  }

  bool retVal = true;

  // push the rhs result to the data stack
  {
    SCOPED_SCRIPT_CONTEXT( FETCH );

    if ( !( mChildren[1]->generateCode( dest ) ) )
      retVal = false;
  }

  // store the rhs into the lhs
  {
    SCOPED_SCRIPT_CONTEXT( STORE );

    if ( !( mChildren[0]->generateCode( dest ) ) )
      retVal = false;
  }

  return retVal;
}

} }
