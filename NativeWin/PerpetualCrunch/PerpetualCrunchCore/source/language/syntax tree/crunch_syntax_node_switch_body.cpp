
#include "../../crunch_global.h"

#include "../../core/core_binary_buffer.h"
#include "../../core/core_qsort.h"

#include "../crunch_script_compiler.h"

#include "crunch_syntax_node_switch_case.h"

#include "crunch_syntax_node_switch_body.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#define CRUNCH_SWITCH_MAX_JUMP_TABLE_SIZE 32000 // max number of entries in jump table
#define CRUNCH_SWITCH_JUMP_TABLE_COVERAGE_THRESHOLD 0.7f // ratio of used slots in jump table must be greater than this

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

SyntaxNodeSwitchBody::SyntaxNodeSwitchBody( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber ) :
  SyntaxNodeBreakableStatement( compiler, root, lineNumber )
{
  // empty
}

SyntaxNodeSwitchBody::~SyntaxNodeSwitchBody()
{
  // empty
}

crunch::core::String SyntaxNodeSwitchBody::getSyntaxTreeDescription( const crunch::core::String& indent ) const
{
  unsigned int totalChildren = (unsigned int)( mChildren.size() );

  crunch::core::String d = crunch::core::String::fmt( "%s%u: SWITCH BODY (children: %u)\n", indent.c_str(), mLineNumber, totalChildren );

  if ( totalChildren > 0 )
  {
    crunch::core::String i0 = indent + mIndentStep;
    
    if ( mChildren[totalChildren - 1] != nullptr )
    {
      d += crunch::core::String::fmt( "%sSWITCH EXPRESSION\n", i0.c_str() );
      if ( mChildren[totalChildren - 1]->getType() != NodeType::NODE_EXPRESSION )
        d += crunch::core::String::fmt( "%sinternal error: expected switch expression at end of children list\n", i0.c_str() );
      d += mChildren[totalChildren - 1]->getSyntaxTreeDescription( i0 );
    }
    else
      d += crunch::core::String::fmt( "%sinternal error: missing switch expression\n", i0.c_str() );

    crunch::core::String nullChild = crunch::core::String::fmt( "%snullptr\n", i0.c_str() );

    unsigned int lastChild = totalChildren - 1;

    for ( unsigned int i = 0; i < lastChild; ++i )
    {
      if ( mChildren[i] == nullptr )
        d += nullChild;
      else
        d += mChildren[i]->getSyntaxTreeDescription( i0 );
    }
  }

  return d;
}

bool SyntaxNodeSwitchBody::generateCode( crunch::core::BinaryBuffer *dest )
{
  SCOPED_BREAKABLE_STATEMENT();

  unsigned int totalChildren = (unsigned int)( mChildren.size() );

  if ( totalChildren == 0 )
  {
    reportInternalError( "switch statement missing switch expression node" );
    return false;
  }

  if ( totalChildren < 2 )
  {
    // we may just have a single default with no cases if totalChildren==2, a check
    //   to ensure at least one non-default case is done below

    reportError( "switch statement must have at least one case" );
    return false;
  }

  unsigned int lastChild = totalChildren - 1;

  {
    SCOPED_SCRIPT_CONTEXT( FETCH );

    if ( !( mChildren[lastChild]->generateCode( dest ) ) )
      return false;
  }

  if ( mChildren[lastChild]->getScriptType() != ScriptTypes::ScriptType::TYPE_I64 )
  {
    reportError( "expected i64 type for switch expression" );
    return false;
  }

  crunch_i64 minValue = _I64_MAX;
  crunch_i64 maxValue = _I64_MIN;

  std::set< crunch_i64 > usedCases;
  std::vector< SwitchCaseInfo > caseValues;

  unsigned int defaultCaseIndex = -1;
  SyntaxNodeSwitchCase *defaultSwitchCase = nullptr;

  for ( unsigned int i = 0; i < lastChild; ++i )
  {
    SyntaxNodeSwitchCase *switchCase = (SyntaxNodeSwitchCase*)( mChildren[i] );

    crunch_i64 expressionValue;

    if ( !( switchCase->generateExpressionValue( &expressionValue ) ) )
    {
      if ( defaultSwitchCase == nullptr )
      {
        defaultSwitchCase = switchCase;
        defaultCaseIndex = i;
        expressionValue = 0;
      }
      else
      {
        reportError( "only 1 default case allowed per switch statement" );
        return false;
      }
    }

    if ( defaultCaseIndex != i )
    {
      std::pair< std::set< crunch_i64 >::iterator, bool > ret = usedCases.insert( expressionValue );

      if ( !( ret.second ) )
      {
        reportError( "duplicate case for value %d", expressionValue );
        return false;
      }

      caseValues.push_back( SwitchCaseInfo( expressionValue, switchCase ) );

      if ( expressionValue < minValue )
        minValue = expressionValue;

      if ( expressionValue > maxValue )
        maxValue = expressionValue;
    }
  }

  unsigned int totalCases = (unsigned int)( caseValues.size() ); // doesn't include optional default case

  int32_t jumpTableSize = (int32_t)( maxValue - minValue + 1 );

  if ( totalCases > 0 )
  {
    if ( totalCases > CRUNCH_SWITCH_MAX_JUMP_TABLE_SIZE )
    {
      reportError( "switch statement has too many cases" );
      return false;
    }
    else
    if ( jumpTableSize > CRUNCH_SWITCH_MAX_JUMP_TABLE_SIZE )
    {
      reportError( "switch statement has too many cases that require too much range (jump table size too large)" );
      return false;
    }
    else
    {
      float jumpTableUsageRatio = ( float( totalCases ) ) / float( jumpTableSize );

      if ( jumpTableUsageRatio < CRUNCH_SWITCH_JUMP_TABLE_COVERAGE_THRESHOLD )
        reportWarning( "poor switch statement, consider using an if statement tree instead (jump table coverage was %0.1f%%", jumpTableUsageRatio * 100.0f );
    }
  }
  else
  {
    reportError( "switch statement must have at least one case" );
    return false;
  }

  char *pcSwitchJumpTable = dest->c_str_offset();
  if ( !( ScriptInstruction::write_SWITCH_JUMP_TABLE( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), int64_t( minValue ), jumpTableSize, "jump table" ) ) )
    return false;

  for ( unsigned int i = 0; i < totalCases; ++i )
  {
    SyntaxNodeSwitchCase *switchCase = (SyntaxNodeSwitchCase*)( caseValues[i].mSwitchCase );

    caseValues[i].mOffset = (uint32_t)( dest->getBufferLength() );

    if ( !( switchCase->generateCode( dest ) ) )
      return false;
  }

  uint32_t defaultCaseOffset = (uint32_t)( dest->getBufferLength() );

  if ( defaultSwitchCase != nullptr )
  {
    if ( !( defaultSwitchCase->generateCode( dest ) ) )
      return false;
  }

  if ( totalCases > 0 )
    crunch::core::core_qsort( (void*)( &( caseValues[0] ) ), std::size_t( totalCases ), sizeof( SwitchCaseInfo ), switchCaseInfoCompare, nullptr );

  std::vector< uint64_t > jumpTable;
  jumpTable.resize( jumpTableSize );

  // fill jump table
  {
    unsigned int valueIndex = 0;
    crunch_i64 currentValue = minValue;

    for ( unsigned int i = 0; i < totalCases; ++i, ++currentValue )
    {
      if ( caseValues[valueIndex].mExpressionValue == currentValue )
      {
        jumpTable[i] = uint64_t( caseValues[valueIndex].mOffset );
        ++valueIndex;
      }
      else
        jumpTable[i] = uint64_t( defaultCaseOffset );
    }
  }

  if ( !( ScriptInstruction::update_SWITCH_JUMP_TABLE( mCompiler, pcSwitchJumpTable, uint64_t( defaultCaseOffset ), &( jumpTable[0] ) ) ) )
    return false;

  uint32_t endSwitchOffset = (uint32_t)( dest->getBufferLength() );

  updateJumpDestinations( endSwitchOffset );

  return true;
}

int SyntaxNodeSwitchBody::switchCaseInfoCompare( void *context, const void *a, const void *b )
{
  SwitchCaseInfo *aInfo = (SwitchCaseInfo*)( a );
  SwitchCaseInfo *bInfo = (SwitchCaseInfo*)( b );

  crunch_i64 d = aInfo->mExpressionValue - bInfo->mExpressionValue;

  if ( d < 0 )
    return -1;

  if ( d > 0 )
    return 1;

  return 0;
}

} }
