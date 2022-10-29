#if !defined ( CRUNCH_SYNTAX_NODE_BINARY_EXPRESSION_H )
#define CRUNCH_SYNTAX_NODE_BINARY_EXPRESSION_H

#include "crunch_syntax_node.h"

namespace crunch { namespace language {

class SyntaxNodeBinaryExpression : public SyntaxNode
{
  public:
    enum class Operator
    {
      OPERATOR_NONE = -1,

      OPERATOR_EE,
      OPERATOR_NE,
      OPERATOR_GT,
      OPERATOR_GE,
      OPERATOR_LT,
      OPERATOR_LE,
      OPERATOR_PLUS,
      OPERATOR_MINUS,
      OPERATOR_MULTIPLY,
      OPERATOR_DIVIDE,
      OPERATOR_BOOL_AND,
      OPERATOR_BOOL_OR,
      OPERATOR_BIT_AND,
      OPERATOR_BIT_OR,

      OPERATOR_TOTAL
    };

  public:
    SyntaxNodeBinaryExpression( ScriptCompiler *compiler = nullptr, 
                                SyntaxNodeRoot *root = nullptr, 
                                unsigned int lineNumber = 0, 
                                SyntaxNode *lhs = nullptr, 
                                Operator op = Operator::OPERATOR_NONE, 
                                SyntaxNode *rhs = nullptr
                                );

    virtual ~SyntaxNodeBinaryExpression();

  public:
    virtual NodeType getType() const override { return NodeType::NODE_BINARY_EXPRESSION; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const override;

    virtual ScriptTypes::ScriptTypeDescription getScriptTypeDescription() const override;

    virtual ScriptValue getScriptValue() const override;

    virtual void applyConstantFolding() override;

    virtual bool generateCode( crunch::core::BinaryBuffer *dest ) override;

  public:
    static crunch::core::String getOperatorName( Operator op );
    static Operator getOperator( const crunch::core::String& name );

  private:
    bool generateCodeLibraryClassOperatorCall( crunch::core::BinaryBuffer *dest, 
                                               const crunch::core::String& operatorName, 
                                               const crunch::core::String& operatorSymbol,
                                               const ScriptTypes::ScriptTypeDescription& lhsTypeDescription,
                                               const ScriptTypes::ScriptTypeDescription& rhsTypeDescription
                                               );

  private:
    Operator mOperator;

};

} }

#endif
