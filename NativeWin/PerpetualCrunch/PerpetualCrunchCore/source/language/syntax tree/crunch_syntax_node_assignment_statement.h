#if !defined ( CRUNCH_SYNTAX_NODE_ASSIGNMENT_STATEMENT_H )
#define CRUNCH_SYNTAX_NODE_ASSIGNMENT_STATEMENT_H

#include "crunch_syntax_node.h"

namespace crunch { namespace language {

class SyntaxNodeAssignmentStatement : public SyntaxNode
{
  public:
    SyntaxNodeAssignmentStatement( ScriptCompiler *compiler = nullptr, SyntaxNodeRoot *root = nullptr, unsigned int lineNumber = 0, SyntaxNode *lhs = nullptr, SyntaxNode *rhs = nullptr );
    virtual ~SyntaxNodeAssignmentStatement();

  public:
    virtual NodeType getType() const override { return NodeType::NODE_ASSIGNMENT_STATEMENT; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const override;

    virtual void applyConstantFolding() override;

    virtual bool generateCode( crunch::core::BinaryBuffer *dest ) override;

};

} }

#endif
