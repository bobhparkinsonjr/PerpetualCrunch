
#include "../../crunch_global.h"

#include "../crunch_script_compiler.h"
#include "crunch_syntax_node_root.h"

#include "crunch_syntax_node.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

crunch::core::String SyntaxNode::mIndentStep = "  ";

SyntaxNode::SyntaxNode( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber ) :
  mCompiler( compiler ),
  mRoot( root ),
  mLineNumber( lineNumber )
{
  // empty
}

SyntaxNode::~SyntaxNode()
{
  deleteChildren();
}

void SyntaxNode::append( SyntaxNode *node, bool allowNullNode )
{
  if ( node != nullptr || allowNullNode )
    mChildren.push_back( node );
}

void SyntaxNode::applyConstantFolding()
{
  ChildrenListType::iterator it = mChildren.begin();
  ChildrenListType::iterator itEnd = mChildren.end();

  for ( ; it != itEnd; ++it )
  {
    if ( *it == nullptr ) continue;
    ( *it )->applyConstantFolding();
  }
}

void SyntaxNode::queryAliveStatus()
{
  ChildrenListType::iterator it = mChildren.begin();
  ChildrenListType::iterator itEnd = mChildren.end();

  for ( ; it != itEnd; ++it )
  {
    if ( *it == nullptr ) continue;
    ( *it )->queryAliveStatus();
  }
}

unsigned int SyntaxNode::getSourceLineNumber() const
{
  return mLineNumber;
}

crunch::core::WideString SyntaxNode::getSourceFilePath() const
{
  if ( mRoot != nullptr )
    return mRoot->getSourceFilePath();

  return L"";
}

const SyntaxNode* SyntaxNode::findNode( NodeType v ) const
{
  if ( !( mChildren.empty() ) )
  {
    ChildrenListType::const_iterator it = mChildren.begin();
    ChildrenListType::const_iterator itEnd = mChildren.end();

    for ( ; it != itEnd; ++it )
    {
      if ( *it == nullptr )
        continue;

      if ( ( *it )->getType() == v )
        return *it;

      const SyntaxNode *node = ( *it )->findNode( v );
      if ( node != nullptr )
        return node;
    }
  }

  return nullptr;
}

const SyntaxNode* SyntaxNode::matchTree2( NodeType thisNode, NodeType childNode1, bool allowChildNode1Children ) const
{
  if ( thisNode != getType() )
    return nullptr;

  if ( mChildren.size() != 1 || mChildren[0] == nullptr || mChildren[0]->getType() != childNode1 )
    return nullptr;

  if ( !allowChildNode1Children && !( mChildren[0]->mChildren.empty() ) )
    return nullptr;

  return mChildren[0];
}

bool SyntaxNode::getChildrenScriptTypeDescriptions( std::vector< ScriptTypes::ScriptTypeDescription > *dest ) const
{
  if ( dest == nullptr )
    return false;

  unsigned int totalChildren = (unsigned int)( mChildren.size() );

  dest->resize( totalChildren );

  ChildrenListType::const_iterator it = mChildren.begin();
  ChildrenListType::const_iterator itEnd = mChildren.end();

  for ( unsigned int index = 0; it != itEnd; ++it, ++index )
  {
    if ( *it != nullptr )
      ( *dest )[index] = ( *it )->getScriptTypeDescription();
    else
      ( *dest )[index] = ScriptTypes::ScriptTypeDescription();
  }

  return true;
}

crunch::core::String SyntaxNode::getChildrenSourceDescription() const
{
  ChildrenListType::const_iterator it = mChildren.begin();
  ChildrenListType::const_iterator itEnd = mChildren.end();

  crunch::core::String nullChild = "nullptr";

  crunch::core::String d;

  if ( !( mChildren.empty() ) )
  {
    if ( *it == nullptr )
      d = nullChild;
    else
      d = ( *it )->getSourceDescription();

    ++it;

    for ( ; it != itEnd; ++it )
    {
      if ( *it == nullptr )
        d += ( "," + nullChild );
      else
        d += ( "," + ( *it )->getSourceDescription() );
    }
  }

  return d;
}

void SyntaxNode::reportInternalError( const char *format, ... ) const
{
	va_list args;
	va_start( args, format );
  crunch::core::String message = crunch::core::String::fmtv( format, args );
  va_end( args );

  mCompiler->reportInternalError( getSourceFilePath(), getSourceLineNumber(), message.c_str() );
}

void SyntaxNode::reportError( const char *format, ... ) const
{
	va_list args;
	va_start( args, format );
  crunch::core::String message = crunch::core::String::fmtv( format, args );
  va_end( args );

  mCompiler->reportError( getSourceFilePath(), getSourceLineNumber(), message.c_str() );
}

void SyntaxNode::reportWarning( const char *format, ... ) const
{
	va_list args;
	va_start( args, format );
  crunch::core::String message = crunch::core::String::fmtv( format, args );
  va_end( args );

  mCompiler->reportWarning( getSourceFilePath(), getSourceLineNumber(), message.c_str() );
}

crunch::core::String SyntaxNode::getSyntaxTreeChildrenDescription( const crunch::core::String& indent ) const
{
  crunch::core::String d;

  ChildrenListType::const_iterator it = mChildren.begin();
  ChildrenListType::const_iterator itEnd = mChildren.end();

  crunch::core::String nullChild = crunch::core::String::fmt( "%snullptr\n", indent.c_str() );

  for ( ; it != itEnd; ++it )
  {
    if ( *it == nullptr )
      d += nullChild;
    else
      d += ( *it )->getSyntaxTreeDescription( indent );
  }

  return d;
}

bool SyntaxNode::generateChildrenCode( crunch::core::BinaryBuffer *dest )
{
  if ( dest == nullptr )
    return false;

  bool retVal = true;

  ChildrenListType::const_iterator it = mChildren.begin();
  ChildrenListType::const_iterator itEnd = mChildren.end();

  for ( ; it != itEnd; ++it )
  {
    if ( *it != nullptr )
    {
      if ( !( ( *it )->generateCode( dest ) ) )
        retVal = false;
    }
  }

  return retVal;
}

void SyntaxNode::deleteChildren()
{
  if ( !( mChildren.empty() ) )
  {
    // corePrint( "deleting %u children ...", (unsigned int)( mChildren.size() ) );

    ChildrenListType::iterator it = mChildren.begin();
    ChildrenListType::iterator itEnd = mChildren.end();

    for ( ; it != itEnd; ++it )
    {
      if ( *it != nullptr )
      {
        --( ( *it )->mReferenceCount );

        if ( ( *it )->mReferenceCount <= 0 )
          delete *it;
      }
    }

    mChildren.clear();
  }
}

} }
