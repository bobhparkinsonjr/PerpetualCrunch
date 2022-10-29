
#include "../../crunch_global.h"

#include "../crunch_script_compiler.h"
#include "../crunch_script_instruction.h"

#include "crunch_syntax_node_if_statement.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

SyntaxNodeIfStatement::SyntaxNodeIfStatement( ScriptCompiler *compiler,
                                              SyntaxNodeRoot *root,
                                              unsigned int lineNumber,
                                              SyntaxNode *expression, 
                                              SyntaxNode *trueStatementBlock, 
                                              SyntaxNode *falseStatementBlock 
                                              ) :
  SyntaxNode( compiler, root, lineNumber )
{
  if ( expression == nullptr )
    reportInternalError( 0, "if statement missing expression node" );

  append( expression );
  append( trueStatementBlock );
  append( falseStatementBlock );

  if ( expression != nullptr )
    mLineNumber = expression->getSourceLineNumber();
}

SyntaxNodeIfStatement::~SyntaxNodeIfStatement()
{
  // empty
}

crunch::core::String SyntaxNodeIfStatement::getSyntaxTreeDescription( const crunch::core::String& indent ) const
{
  unsigned int totalChildren = (unsigned int)( mChildren.size() );

  crunch::core::String d = crunch::core::String::fmt( "%s%u: IF STATEMENT (children: %u)\n", indent.c_str(), mLineNumber, totalChildren );

  crunch::core::String i0 = indent + mIndentStep;
  crunch::core::String i1 = indent + mIndentStep + mIndentStep;
  
  if ( totalChildren == ChildType::TOTAL )
  {
    if ( mChildren[ChildType::EXPRESSION] != nullptr )
    {
      d += crunch::core::String::fmt( "%sEXPRESSION\n", i0.c_str() );
      d += mChildren[ChildType::EXPRESSION]->getSyntaxTreeDescription( i1 );
    }
    else
      d += crunch::core::String::fmt( "%sEXPRESSION: internal error: missing if expression\n", i0.c_str() );

    if ( mChildren[ChildType::TRUE_STATEMENT_BLOCK] != nullptr )
    {
      d += crunch::core::String::fmt( "%sTRUE STATEMENT BLOCK\n", i0.c_str() );
      d += mChildren[ChildType::TRUE_STATEMENT_BLOCK]->getSyntaxTreeDescription( i1 );
    }

    if ( mChildren[ChildType::FALSE_STATEMENT_BLOCK] != nullptr )
    {
      d += crunch::core::String::fmt( "%sFALSE STATEMENT BLOCK\n", i0.c_str() );
      d += mChildren[ChildType::FALSE_STATEMENT_BLOCK]->getSyntaxTreeDescription( i1 );
    }
  }
  else
    d += crunch::core::String::fmt( "%sinternal error: unexpected number of children\n", i0.c_str() );

  return d;
}

bool SyntaxNodeIfStatement::generateCode( crunch::core::BinaryBuffer *dest )
{
  if ( mChildren[ChildType::EXPRESSION] == nullptr )
    return false;

  {
    SCOPED_SCRIPT_CONTEXT( FETCH );

    if ( !( mChildren[ChildType::EXPRESSION]->generateCode( dest ) ) )
      return false;
  }

  if ( mChildren[ChildType::EXPRESSION]->getScriptType() != ScriptTypes::ScriptType::TYPE_I64 )
  {
    reportError( "expected i64 type for if expression" );
    return false;
  }

  if ( mChildren[ChildType::TRUE_STATEMENT_BLOCK] == nullptr && mChildren[ChildType::FALSE_STATEMENT_BLOCK] == nullptr )
  {
    // just clean stack of expression

    ScriptInstruction::write_POP( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), sizeof( crunch_i64 ), "clean if expression from stack, no true or false statement block" );
  }
  else
  if ( mChildren[ChildType::TRUE_STATEMENT_BLOCK] != nullptr && mChildren[ChildType::FALSE_STATEMENT_BLOCK] != nullptr )
  {
    char *pcBranch = dest->c_str_offset();
    ScriptInstruction::write_BRANCH( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), "if statement with true and false blocks" );

    if ( !( mChildren[ChildType::TRUE_STATEMENT_BLOCK]->generateCode( dest ) ) )
      return false;

    char *pcJump = dest->c_str_offset();
    ScriptInstruction::write_JUMP( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), "jump past false block" );

    uint32_t falseStartOffset = (uint32_t)( dest->getBufferLength() );
    ScriptInstruction::update_BRANCH( mCompiler, pcBranch, falseStartOffset );

    if ( !( mChildren[ChildType::FALSE_STATEMENT_BLOCK]->generateCode( dest ) ) )
      return false;

    uint32_t endIfOffset = (uint32_t)( dest->getBufferLength() );
    ScriptInstruction::update_JUMP( mCompiler, pcJump, endIfOffset );
  }
  else
  if ( mChildren[ChildType::TRUE_STATEMENT_BLOCK] != nullptr )
  {
    char *pcBranch = dest->c_str_offset();
    ScriptInstruction::write_BRANCH( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), "if statement with true block" );

    if ( !( mChildren[ChildType::TRUE_STATEMENT_BLOCK]->generateCode( dest ) ) )
      return false;

    uint32_t endIfOffset = (uint32_t)( dest->getBufferLength() );
    ScriptInstruction::update_BRANCH( mCompiler, pcBranch, endIfOffset );
  }
  else
  {
    char *pcBranch = dest->c_str_offset();
    ScriptInstruction::write_BRANCHF( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), "if statement with false block" );

    if ( !( mChildren[ChildType::FALSE_STATEMENT_BLOCK]->generateCode( dest ) ) )
      return false;

    uint32_t endIfOffset = (uint32_t)( dest->getBufferLength() );
    ScriptInstruction::update_BRANCHF( mCompiler, pcBranch, endIfOffset );
  }

  return true;
}

} }
