
#include "../../crunch_global.h"

#include "../crunch_script_compiler.h"
#include "../crunch_script_instruction.h"

#include "crunch_syntax_node_break_statement.h"

#include "crunch_syntax_node_while_statement.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

SyntaxNodeWhileStatement::SyntaxNodeWhileStatement( ScriptCompiler *compiler,
                                                    SyntaxNodeRoot *root,
                                                    unsigned int lineNumber,
                                                    SyntaxNode *expression, 
                                                    SyntaxNode *statementBlock
                                                    ) :
  SyntaxNodeBreakableStatement( compiler, root, lineNumber )
{
  if ( expression == nullptr )
    reportInternalError( 0, "while statement missing expression node" );

  append( expression );
  append( statementBlock );

  if ( expression != nullptr )
    mLineNumber = expression->getSourceLineNumber();
}

SyntaxNodeWhileStatement::~SyntaxNodeWhileStatement()
{
  // empty
}

crunch::core::String SyntaxNodeWhileStatement::getSyntaxTreeDescription( const crunch::core::String& indent ) const
{
  unsigned int totalChildren = (unsigned int)( mChildren.size() );

  crunch::core::String d = crunch::core::String::fmt( "%s%u: WHILE STATEMENT (children: %u)\n", indent.c_str(), mLineNumber, totalChildren );

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
      d += crunch::core::String::fmt( "%sEXPRESSION: internal error: missing while expression\n", i0.c_str() );

    if ( mChildren[ChildType::STATEMENT_BLOCK] != nullptr )
    {
      d += crunch::core::String::fmt( "%sSTATEMENT BLOCK\n", i0.c_str() );
      d += mChildren[ChildType::STATEMENT_BLOCK]->getSyntaxTreeDescription( i1 );
    }
  }
  else
    d += crunch::core::String::fmt( "%sinternal error: unexpected number of children\n", i0.c_str() );

  return d;
}

bool SyntaxNodeWhileStatement::generateCode( crunch::core::BinaryBuffer *dest )
{
  if ( mChildren[ChildType::EXPRESSION] == nullptr )
    return false;

  SCOPED_BREAKABLE_STATEMENT();

  uint32_t whileStartOffset = (uint32_t)( dest->getBufferLength() );

  {
    SCOPED_SCRIPT_CONTEXT( FETCH );

    if ( !( mChildren[ChildType::EXPRESSION]->generateCode( dest ) ) )
      return false;
  }

  if ( mChildren[ChildType::EXPRESSION]->getScriptType() != ScriptTypes::ScriptType::TYPE_I64 )
  {
    reportError( "expected i64 type for while expression" );
    return false;
  }

  char *pcBranch = dest->c_str_offset();
  ScriptInstruction::write_BRANCH( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), "while statement expression test" );

  if ( mChildren[ChildType::STATEMENT_BLOCK] != nullptr )
  {
    if ( !( mChildren[ChildType::STATEMENT_BLOCK]->generateCode( dest ) ) )
      return false;
  }

  char *pcJump = dest->c_str_offset();
  ScriptInstruction::write_JUMP( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), "jump to start of while loop" );
  ScriptInstruction::update_JUMP( mCompiler, pcJump, whileStartOffset );

  uint32_t endLoopOffset = (uint32_t)( dest->getBufferLength() );
  ScriptInstruction::update_BRANCH( mCompiler, pcBranch, endLoopOffset );

  updateJumpDestinations( endLoopOffset );

  return true;
}

} }
