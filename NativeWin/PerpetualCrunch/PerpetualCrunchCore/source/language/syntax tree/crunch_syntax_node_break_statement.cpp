
#include "../../crunch_global.h"

#include "../crunch_script_instruction.h"

#include "crunch_syntax_node_breakable_statement.h"

#include "crunch_syntax_node_break_statement.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

SyntaxNodeBreakStatement::SyntaxNodeBreakStatement( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber ) :
  SyntaxNode( compiler, root, lineNumber )
{
  // empty
}

SyntaxNodeBreakStatement::~SyntaxNodeBreakStatement()
{
  // empty
}

crunch::core::String SyntaxNodeBreakStatement::getSyntaxTreeDescription( const crunch::core::String& indent ) const
{
  crunch::core::String d = crunch::core::String::fmt( "%s%u: BREAK STATEMENT (children: %u)\n", indent.c_str(), mLineNumber, (unsigned int)( mChildren.size() ) );
  d += getSyntaxTreeChildrenDescription( indent + mIndentStep );
  return d;
}

bool SyntaxNodeBreakStatement::generateCode( crunch::core::BinaryBuffer *dest )
{
  SyntaxNodeBreakableStatement *breakableStatement = SyntaxNodeBreakableStatement::getCurrentBreakableStatement();

  if ( breakableStatement == nullptr )
  {
    reportError( "break statement invalid, no outer breakable statement found" );
    return false;
  }

  breakableStatement->registerBreakStatement( this );

  mPCJump = dest->c_str_offset();
  ScriptInstruction::write_JUMP( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), "break out of breakable statement" );

  return true;
}

bool SyntaxNodeBreakStatement::updateJumpDestination( uint32_t dest )
{
  if ( mPCJump == nullptr )
  {
    reportInternalError( "break statement invalid for update jump" );
    return false;
  }

  ScriptInstruction::update_JUMP( mCompiler, mPCJump, dest );
  mPCJump = nullptr;

  return true;
}

} }
