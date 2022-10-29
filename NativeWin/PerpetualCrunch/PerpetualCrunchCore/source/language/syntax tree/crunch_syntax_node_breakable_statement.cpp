
#include "../../crunch_global.h"

#include "../crunch_script_compiler.h"
#include "../crunch_script_instruction.h"
#include "crunch_syntax_node_break_statement.h"

#include "crunch_syntax_node_breakable_statement.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

SyntaxNodeBreakableStatement::SyntaxNodeBreakableStatement( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber ) :
  SyntaxNode( compiler, root, lineNumber )
{
  // empty
}

SyntaxNodeBreakableStatement::~SyntaxNodeBreakableStatement()
{
  // empty
}

bool SyntaxNodeBreakableStatement::registerBreakStatement( SyntaxNodeBreakStatement *breakStatement )
{
  if ( breakStatement == nullptr || this != SyntaxNodeBreakableStatement::getCurrentBreakableStatement() )
  {
    reportInternalError( "invalid break statement for current breakable statement" );
    return false;
  }

  for ( SyntaxNodeBreakStatement *b : mBreakStatements )
  {
    if ( b == breakStatement )
    {
      reportInternalError( "break statement already registered" );
      return false;
    }
  }

  mBreakStatements.push_back( breakStatement );

  return true;
}

SyntaxNodeBreakableStatement* SyntaxNodeBreakableStatement::getCurrentBreakableStatement()
{
  if ( !( mBreakableStatementStack.empty() ) )
    return mBreakableStatementStack.back();
  return nullptr;
}

void SyntaxNodeBreakableStatement::updateJumpDestinations( uint32_t endBreakableStatementOffset )
{
  for ( SyntaxNodeBreakStatement *breakStatement : mBreakStatements )
    breakStatement->updateJumpDestination( endBreakableStatementOffset );

  mBreakStatements.clear();
}

void SyntaxNodeBreakableStatement::pushCurrentBreakableStatement( SyntaxNodeBreakableStatement *breakableStatement )
{
  mBreakableStatementStack.push_back( breakableStatement );
}

void SyntaxNodeBreakableStatement::popCurrentBreakableStatement( SyntaxNodeBreakableStatement *breakableStatement )
{
  if ( breakableStatement == nullptr )
  {
    corePrint( "ERROR: internal compiler error: missing breakable statement in popCurrentBreakableStatement" );
  }
  else
  if ( mBreakableStatementStack.empty() )
    breakableStatement->reportInternalError( "corrupt breakable statement stack" );
  else
  if ( getCurrentBreakableStatement() != breakableStatement )
    breakableStatement->reportInternalError( "unexpected breakable statement on top of breakable statement stack" );
  else
    mBreakableStatementStack.pop_back();
}

} }
