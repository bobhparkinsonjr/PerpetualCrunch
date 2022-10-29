#if !defined ( CRUNCH_SYNTAX_NODE_BREAK_STATEMENT_H )
#define CRUNCH_SYNTAX_NODE_BREAK_STATEMENT_H

#include "crunch_syntax_node.h"

namespace crunch { namespace language {

class SyntaxNodeBreakStatement : public SyntaxNode
{
  public:
    SyntaxNodeBreakStatement( ScriptCompiler *compiler = nullptr, SyntaxNodeRoot *root = nullptr, unsigned int lineNumber = 0 );
    virtual ~SyntaxNodeBreakStatement();

  public:
    virtual NodeType getType() const override { return NodeType::NODE_BREAK_STATEMENT; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const override;

    virtual bool generateCode( crunch::core::BinaryBuffer *dest ) override;

    bool updateJumpDestination( uint32_t dest );

  private:
    char *mPCJump = nullptr;

};

} }

#endif
