#if !defined ( CRUNCH_SYNTAX_NODE_SWITCH_CASE_H )
#define CRUNCH_SYNTAX_NODE_SWITCH_CASE_H

#include "crunch_syntax_node.h"

namespace crunch { namespace language {

class SyntaxNodeSwitchCase : public SyntaxNode
{
  public:
    typedef enum
    {
      EXPRESSION,
      STATEMENT_LIST,

      TOTAL
    } ChildType;

  public:
    SyntaxNodeSwitchCase( ScriptCompiler *compiler = nullptr, SyntaxNodeRoot *root = nullptr, unsigned int lineNumber = 0, SyntaxNode *expression = nullptr, SyntaxNode *statementList = nullptr );
    virtual ~SyntaxNodeSwitchCase();

  public:
    virtual NodeType getType() const override { return NodeType::NODE_SWITCH_CASE; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const override;

    virtual bool generateCode( crunch::core::BinaryBuffer *dest ) override;

    bool generateExpressionValue( crunch_i64 *expressionValue );

};

} }

#endif
