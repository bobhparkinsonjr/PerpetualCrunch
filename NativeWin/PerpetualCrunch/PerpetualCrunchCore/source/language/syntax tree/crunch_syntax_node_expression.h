#if !defined ( CRUNCH_SYNTAX_NODE_EXPRESSION_H )
#define CRUNCH_SYNTAX_NODE_EXPRESSION_H

#include "crunch_syntax_node.h"

namespace crunch { namespace language {

class SyntaxNodeExpression : public SyntaxNode
{
  public:
    enum class Operator
    {
      OPERATOR_NONE = -1,

      OPERATOR_MINUS,
      OPERATOR_BOOL_NOT,
      OPERATOR_BIT_COMP,

      OPERATOR_TOTAL
    };

  public:
    SyntaxNodeExpression( ScriptCompiler *compiler = nullptr, 
                          SyntaxNodeRoot *root = nullptr, 
                          unsigned int lineNumber = 0, 
                          SyntaxNode *node = nullptr, 
                          Operator op = Operator::OPERATOR_NONE 
                          );

    virtual ~SyntaxNodeExpression();

  public:
    virtual NodeType getType() const override { return NodeType::NODE_EXPRESSION; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const override;

    virtual crunch::core::String getSourceDescription() const override;

    virtual ScriptTypes::ScriptTypeDescription getScriptTypeDescription() const override;

    virtual ScriptValue getScriptValue() const override;

    virtual void setParentContextNode( SyntaxNode *node ) override;

    virtual void applyConstantFolding() override;

    virtual bool generateCode( crunch::core::BinaryBuffer *dest ) override;

    virtual bool isLValue() const override;

    virtual void setStatementContext( bool v ) override { mStatementContext = v; }

  public:
    static crunch::core::String getOperatorName( Operator op );
    static Operator getOperator( const crunch::core::String& name );

  private:
    Operator mOperator;
    bool mStatementContext = false;

};

} }

#endif
