#if !defined ( CRUNCH_SYNTAX_NODE_RETURN_STATEMENT_H )
#define CRUNCH_SYNTAX_NODE_RETURN_STATEMENT_H

#include "crunch_syntax_node.h"

namespace crunch { namespace language {

class SyntaxNodeReturnStatement : public SyntaxNode
{
  public:
    SyntaxNodeReturnStatement( ScriptCompiler *compiler = nullptr, SyntaxNodeRoot *root = nullptr, unsigned int lineNumber = 0, SyntaxNode *expression = nullptr );
    virtual ~SyntaxNodeReturnStatement();

  public:
    virtual NodeType getType() const override { return NodeType::NODE_RETURN_STATEMENT; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const override;

    virtual bool generateCode( crunch::core::BinaryBuffer *dest ) override;

};

} }

#endif
