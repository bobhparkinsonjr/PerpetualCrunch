#if !defined ( CRUNCH_SYMBOL_STACK_H )
#define CRUNCH_SYMBOL_STACK_H

#include "syntax tree/crunch_syntax_nodes.h"

namespace crunch { namespace language {

class SyntaxNodeTypeIdentifier;

class SymbolStack final
{
  public:
    enum { OFFSET_UNUSED = 0xFFFFFFFF };

    typedef enum
    {
      SYMBOL_LOCATION_NONE = -1,

      SYMBOL_LOCATION_DATA_STACK,
      SYMBOL_LOCATION_GLOBAL_LIBRARY_CLASS_INSTANCE,
      SYMBOL_LOCATION_CONSTANT,

      SYMBOL_LOCATION_TOTAL
    } SymbolLocation;

    class Symbol
    {
      public:
        crunch::core::String mName;
        unsigned int mOffset = OFFSET_UNUSED;
        SyntaxNode *mNode = nullptr;
        SymbolLocation mLocation = SYMBOL_LOCATION_NONE;

      public:
        Symbol( const crunch::core::String& name, unsigned int offset, SyntaxNode *node, SymbolLocation location ) :
          mName( name ),
          mOffset( offset ),
          mNode( node ),
          mLocation( location )
        {
          // empty
        }

      public:
        bool operator < ( const Symbol& other ) const
        {
          return ( mName < other.mName );
        }

    };

    typedef std::set< Symbol > SymbolSetType;

    class Scope
    {
      public:
        SymbolSetType mSymbols;
        unsigned int mOffset;
        unsigned int mSize;

      public:
        Scope( unsigned int offset = 0 ) :
          mOffset( offset ),
          mSize( 0 )
        {
          // empty
        }

    };

    typedef std::vector< Scope > ScopeStackType;

  public:
    SymbolStack( ScriptCompiler *compiler = nullptr );
    ~SymbolStack();

  public:
    void destroy();

    void pushScope();
    void popScope();

    // 0=>first scope, 1=>second scope, ...
    // will return UINT_MAX on error
    unsigned int getCurrentScopeLevel();

    // in bytes
    // this is the number of bytes to pop from the stack to fully remove all data from all the scopes starting at scopeLevel
    unsigned int getStackSize( unsigned int scopeLevel ) const;

    // for local declarations and function parameters only, things that live on the data stack at run-time
    void pushSymbol( const crunch::core::String& name, SyntaxNodeTypeIdentifier *typeIdentifier );

    // for globals that don't consume data stack at run-time
    void pushGlobalSymbol( const crunch::core::String& name, SyntaxNodeTypeIdentifier *typeIdentifier );

    // for constants at any scope that don't consume data stack at run-time
    void pushConstantSymbol( const crunch::core::String& name, SyntaxNodeLiteralType *literalNode );

    bool findSymbol( const crunch::core::String& name, 
                     unsigned int *offset, // allowed to be nullptr
                     SyntaxNode **node, // allowed to be nullptr
                     bool skipCurrentScope,
                     SymbolLocation *location // allowed to be nullptr
                     );

  private:
    ScriptCompiler *mCompiler = nullptr;
    ScopeStackType mScopeStack;
    unsigned int mOffset = 0;

};

} }

#endif
