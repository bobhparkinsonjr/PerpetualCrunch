
#include "../../crunch_global.h"

#include "../crunch_script_compiler.h"
#include "../crunch_script_instruction.h"

#include "crunch_syntax_node_return_statement.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

SyntaxNodeReturnStatement::SyntaxNodeReturnStatement( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber, SyntaxNode *expression ) :
  SyntaxNode( compiler, root, lineNumber )
{
  append( expression );
}

SyntaxNodeReturnStatement::~SyntaxNodeReturnStatement()
{
  // empty
}

crunch::core::String SyntaxNodeReturnStatement::getSyntaxTreeDescription( const crunch::core::String& indent ) const
{
  crunch::core::String d = crunch::core::String::fmt( "%s%u: RETURN STATEMENT (children: %u)\n", indent.c_str(), mLineNumber, (unsigned int)( mChildren.size() ) );
  d += getSyntaxTreeChildrenDescription( indent + mIndentStep );
  return d;
}

bool SyntaxNodeReturnStatement::generateCode( crunch::core::BinaryBuffer *dest )
{
  SyntaxNodeFunctionDeclaration *currentFunction = SyntaxNodeFunctionDeclaration::getCurrentFunctionDeclaration();

  if ( currentFunction == nullptr )
  {
    reportInternalError( "return statement missing current function declaration" );
    return false;
  }

  if ( mChildren.size() != 1 )
  {
    reportInternalError( "return statement should have 1 child node" );
    return false;
  }

  ScriptTypes::ScriptTypeDescription functionReturnType = currentFunction->getReturnType();

  crunch::core::String comment = crunch::core::String::fmt( "exit function '%s' with return value", currentFunction->getName().c_str() );

  if ( mChildren[0] != nullptr )
  {
    ScriptTypes::ScriptTypeDescription returnExpressionType = mChildren[0]->getScriptTypeDescription();
    if ( returnExpressionType != functionReturnType )
      reportError( "function return type mismatch, function must return type '%s', found '%s'", functionReturnType.getDisplayTypeDescription().c_str(), returnExpressionType.getDisplayTypeDescription().c_str() );

    // leave return value on the stack

    {
      SCOPED_SCRIPT_CONTEXT( FETCH );

      if ( !( mChildren[0]->generateCode( dest ) ) )
        return false;
    }

    unsigned int scopeStackSize = currentFunction->getScopeStackSize();
    unsigned int returnTypeStackSize = ScriptTypes::getDataStackSize( returnExpressionType );

    if ( !( ScriptInstruction::write_RETURN2( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), uint32_t( scopeStackSize ), uint64_t( returnTypeStackSize ), comment ) ) )
      return false;
  }
  else
  {
    if ( functionReturnType.mScriptType != ScriptTypes::ScriptType::TYPE_VOID )
      reportError( "function must return a value, function must return type '%s'", functionReturnType.getDisplayTypeDescription().c_str() );

    if ( !( currentFunction->generateCodePopLocals( dest ) ) )
      return false;

    if ( !( ScriptInstruction::write_RETURN( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), comment ) ) )
      return false;
  }

  return true;
}

} }
