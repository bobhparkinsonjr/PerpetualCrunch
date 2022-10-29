#if !defined ( CRUNCH_SYNTAX_NODE_EXPRESSION_LIST_H )
#define CRUNCH_SYNTAX_NODE_EXPRESSION_LIST_H

#include "crunch_syntax_node.h"

namespace crunch { namespace language {

class SyntaxNodeExpressionList : public SyntaxNode
{
  public:
    SyntaxNodeExpressionList( ScriptCompiler *compiler = nullptr, SyntaxNodeRoot *root = nullptr, unsigned int lineNumber = 0 );
    virtual ~SyntaxNodeExpressionList();

  public:
    virtual NodeType getType() const override { return NodeType::NODE_EXPRESSION_LIST; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const override;

    virtual void applyConstantFolding() override;

    virtual bool generateCode( crunch::core::BinaryBuffer *dest ) override;

};

} }

#endif
