
#include "../crunch_global.h"

#include "crunch_script_compiler.h"

#include "crunch_symbol_stack.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

///////////////////////////////////////////////////////////////////////////////////////////////////

SymbolStack::SymbolStack( ScriptCompiler *compiler ) :
  mCompiler( compiler )
{
  mOffset = 0;

  mScopeStack.clear();
  mScopeStack.push_back( Scope( mOffset ) ); // push the global scope
}

SymbolStack::~SymbolStack()
{
  destroy();
}
  
void SymbolStack::destroy()
{
  mScopeStack.clear();
}

void SymbolStack::pushScope()
{
  mScopeStack.push_back( Scope( mOffset ) );
}

void SymbolStack::popScope()
{
  if ( mScopeStack.empty() )
  {
    mCompiler->reportInternalError( L"", 0, "scope stack is empty" );
    return;
  }

  // restore offset to fully remove scope from stack
  {
    Scope& myScope = mScopeStack.back();
    mOffset = myScope.mOffset;
  }

  mScopeStack.pop_back();
}

unsigned int SymbolStack::getCurrentScopeLevel()
{
  if ( mScopeStack.empty() )
  {
    mCompiler->reportInternalError( L"", 0, "scope stack is empty" );
    return UINT_MAX;
  }

  return (unsigned int)( mScopeStack.size() - 1 );
}

unsigned int SymbolStack::getStackSize( unsigned int scopeLevel ) const
{
  unsigned int totalScopes = (unsigned int)( mScopeStack.size() );

  if ( totalScopes == 0 || scopeLevel >= totalScopes )
  {
    mCompiler->reportInternalError( L"", 0, "scope stack is empty or scope level is invalid | total scopes: %u | scope level: %u", totalScopes, scopeLevel );
    return 0;
  }

  unsigned int total = 0;

  for ( unsigned int i = scopeLevel; i < totalScopes; ++i )
    total += mScopeStack[i].mSize;

  return total;
}

void SymbolStack::pushSymbol( const crunch::core::String& name, SyntaxNodeTypeIdentifier *typeIdentifier )
{
  if ( name == "" )
  {
    mCompiler->reportInternalError( L"", 0, "symbol name is empty" );
    return;
  }

  if ( typeIdentifier == nullptr )
  {
    mCompiler->reportInternalError( L"", 0, "symbol missing type identifier" );
    return;
  }

  unsigned int typeDataStackSize = typeIdentifier->getDataStackSize(); // in bytes
  if ( typeDataStackSize == 0 )
  {
    mCompiler->reportInternalError( L"", 0, "symbol '%s' with type '%s' has no data stack size", name.c_str(), typeIdentifier->getScriptTypeDescription().getTypeDescription().c_str() );
    return;
  }

  Scope& myScope = mScopeStack.back();

  unsigned int offset = mOffset;
  mOffset += typeDataStackSize;
  myScope.mSize += typeDataStackSize;

  SyntaxNode *shadowNode = nullptr;
  bool foundShadowSymbol = findSymbol( name, nullptr, &shadowNode, true, nullptr );

  std::pair< SymbolSetType::iterator, bool > insertResult = myScope.mSymbols.insert( Symbol( name, offset, typeIdentifier, SYMBOL_LOCATION_DATA_STACK ) );

  if ( !( insertResult.second ) )
    typeIdentifier->reportError( "symbol '%s' already defined in current scope", name.c_str() );

  if ( foundShadowSymbol )
    typeIdentifier->reportWarning( "symbol '%s' shadows existing symbol from previous scope, declared around line %u", name.c_str(), shadowNode->getSourceLineNumber() );
}

void SymbolStack::pushGlobalSymbol( const crunch::core::String& name, SyntaxNodeTypeIdentifier *typeIdentifier )
{
  if ( mScopeStack.size() != 1 )
  {
    mCompiler->reportInternalError( L"", 0, "cannot push global symbols onto stack of non-one size" );
    return;
  }

  if ( name == "" )
  {
    mCompiler->reportInternalError( L"", 0, "symbol name is empty" );
    return;
  }

  if ( typeIdentifier == nullptr )
  {
    mCompiler->reportInternalError( L"", 0, "symbol missing type identifier" );
    return;
  }

  if ( typeIdentifier->getScriptType() != ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS )
  {
    mCompiler->reportInternalError( L"", 0, "symbol stack globals are required to be library class instances" );
    return;
  }

  Scope& myScope = mScopeStack.back();

  std::pair< SymbolSetType::iterator, bool > insertResult = myScope.mSymbols.insert( Symbol( name, OFFSET_UNUSED, typeIdentifier, SYMBOL_LOCATION_GLOBAL_LIBRARY_CLASS_INSTANCE ) );

  if ( !( insertResult.second ) )
    typeIdentifier->reportError( "symbol '%s' already defined in current scope", name.c_str() );
}

void SymbolStack::pushConstantSymbol( const crunch::core::String& name, SyntaxNodeLiteralType *literalNode )
{
  if ( name == "" )
  {
    mCompiler->reportInternalError( L"", 0, "symbol name is empty" );
    return;
  }

  if ( literalNode == nullptr )
  {
    mCompiler->reportInternalError( L"", 0, "symbol missing literal node" );
    return;
  }

  Scope& myScope = mScopeStack.back();

  std::pair< SymbolSetType::iterator, bool > insertResult = myScope.mSymbols.insert( Symbol( name, OFFSET_UNUSED, literalNode, SYMBOL_LOCATION_CONSTANT ) );

  if ( !( insertResult.second ) )
    literalNode->reportError( "symbol '%s' already defined in current scope", name.c_str() );
}

bool SymbolStack::findSymbol( const crunch::core::String& name, 
                              unsigned int *offset, 
                              SyntaxNode **node, 
                              bool skipCurrentScope,
                              SymbolLocation *location
                              )
{
  if ( mScopeStack.empty() )
    return false;

  Symbol findMe( name, 0, nullptr, SYMBOL_LOCATION_NONE );

  ScopeStackType::const_reverse_iterator it = mScopeStack.crbegin();
  ScopeStackType::const_reverse_iterator itEnd = mScopeStack.crend();

  if ( skipCurrentScope )
    ++it;

  for ( ; it != itEnd; ++it )
  {
    const Scope& myScope = *it;

    SymbolSetType::const_iterator findIt = myScope.mSymbols.find( findMe );
    if ( findIt != myScope.mSymbols.end() )
    {
      const Symbol *found = &( *findIt );

      if ( offset != nullptr )
        *offset = found->mOffset;

      if ( node != nullptr )
        *node = found->mNode;

      if ( location != nullptr )
        *location = found->mLocation;

      return true;
    }
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

} }
