#if !defined ( CRUNCH_SYNTAX_NODE_STATEMENT_LIST_H )
#define CRUNCH_SYNTAX_NODE_STATEMENT_LIST_H

#include "crunch_syntax_node.h"

namespace crunch { namespace language {

class SyntaxNodeStatementList : public SyntaxNode
{
  public:
    SyntaxNodeStatementList( ScriptCompiler *compiler = nullptr, SyntaxNodeRoot *root = nullptr, unsigned int lineNumber = 0 );
    virtual ~SyntaxNodeStatementList();

  public:
    virtual NodeType getType() const override { return NodeType::NODE_STATEMENT_LIST; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const override;

    virtual bool generateCode( crunch::core::BinaryBuffer *dest ) override;

    core_forceinline void setScoped() { mScoped = true; }

  private:
    bool mScoped = false;

};

} }

#endif
