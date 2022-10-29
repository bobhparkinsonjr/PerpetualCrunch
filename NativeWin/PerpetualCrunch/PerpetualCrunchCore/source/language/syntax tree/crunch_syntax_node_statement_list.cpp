
#include "../../crunch_global.h"

#include "../crunch_script_compiler.h"
#include "../crunch_symbol_stack.h"
#include "../crunch_script_instruction.h"

#include "crunch_syntax_node_statement_list.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

SyntaxNodeStatementList::SyntaxNodeStatementList( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber ) :
  SyntaxNode( compiler, root, lineNumber )
{
  // empty
}

SyntaxNodeStatementList::~SyntaxNodeStatementList()
{
  // empty
}

crunch::core::String SyntaxNodeStatementList::getSyntaxTreeDescription( const crunch::core::String& indent ) const
{
  crunch::core::String d = crunch::core::String::fmt( "%s%u: STATEMENT LIST (children: %u)\n", indent.c_str(), mLineNumber, (unsigned int)( mChildren.size() ) );
  d += getSyntaxTreeChildrenDescription( indent + mIndentStep );
  return d;
}

bool SyntaxNodeStatementList::generateCode( crunch::core::BinaryBuffer *dest )
{
  unsigned int startScopeLevel;

  if ( mScoped )
  {
    mCompiler->getSymbolStack()->pushScope();
    startScopeLevel = mCompiler->getSymbolStack()->getCurrentScopeLevel();
  }

  bool retVal = generateChildrenCode( dest );

  if ( mScoped )
  {
    unsigned int stackSize = mCompiler->getSymbolStack()->getStackSize( startScopeLevel );

    if ( stackSize > 0 )
      ScriptInstruction::write_POP( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), (uint32_t)( stackSize ), "pop scoped statement list locals" );

    mCompiler->getSymbolStack()->popScope();
  }

  return retVal;
}

} }
