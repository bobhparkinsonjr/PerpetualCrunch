#if !defined ( CRUNCH_SYNTAX_NODE_BREAKABLE_STATEMENT_H )
#define CRUNCH_SYNTAX_NODE_BREAKABLE_STATEMENT_H

#include "crunch_syntax_node.h"

namespace crunch { namespace language {

class SyntaxNodeBreakStatement;

class SyntaxNodeBreakableStatement : public SyntaxNode
{
  public:
    SyntaxNodeBreakableStatement( ScriptCompiler *compiler = nullptr, SyntaxNodeRoot *root = nullptr, unsigned int lineNumber = 0 );
    virtual ~SyntaxNodeBreakableStatement();

  public:
    bool registerBreakStatement( SyntaxNodeBreakStatement *breakStatement );

  public:
    // can return nullptr
    static SyntaxNodeBreakableStatement* getCurrentBreakableStatement();

  protected:
    void updateJumpDestinations( uint32_t endBreakableStatementOffset );

  private:
    static void pushCurrentBreakableStatement( SyntaxNodeBreakableStatement *breakableStatement );
    static void popCurrentBreakableStatement( SyntaxNodeBreakableStatement *breakableStatement );

  private:
    std::vector< SyntaxNodeBreakStatement* > mBreakStatements;

  private:
    inline static std::vector< SyntaxNodeBreakableStatement* > mBreakableStatementStack;

  friend class SyntaxNodeBreakableStatementScope;
};

class SyntaxNodeBreakableStatementScope
{
  public:
    SyntaxNodeBreakableStatementScope( SyntaxNodeBreakableStatement *breakableStatement ) :
      mBreakableStatement( breakableStatement )
    {
      SyntaxNodeBreakableStatement::pushCurrentBreakableStatement( breakableStatement );
    }

    ~SyntaxNodeBreakableStatementScope()
    {
      SyntaxNodeBreakableStatement::popCurrentBreakableStatement( mBreakableStatement );
    }

  private:
    SyntaxNodeBreakableStatement *mBreakableStatement;

};

#define SCOPED_BREAKABLE_STATEMENT() SyntaxNodeBreakableStatementScope _SyntaxNodeBreakableStatementScope( this )

} }

#endif
