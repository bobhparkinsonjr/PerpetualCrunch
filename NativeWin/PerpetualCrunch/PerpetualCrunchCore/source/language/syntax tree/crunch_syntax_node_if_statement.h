#if !defined ( CRUNCH_SYNTAX_NODE_IF_STATEMENT_H )
#define CRUNCH_SYNTAX_NODE_IF_STATEMENT_H

#include "crunch_syntax_node.h"

namespace crunch { namespace language {

class SyntaxNodeIfStatement : public SyntaxNode
{
  private:
    typedef enum
    {
      EXPRESSION,
      TRUE_STATEMENT_BLOCK,
      FALSE_STATEMENT_BLOCK,

      TOTAL
    } ChildType;

  public:
    SyntaxNodeIfStatement( ScriptCompiler *compiler = nullptr, 
                           SyntaxNodeRoot *root = nullptr,
                           unsigned int lineNumber = 0, 
                           SyntaxNode *expression = nullptr, 
                           SyntaxNode *trueStatementBlock = nullptr, 
                           SyntaxNode *falseStatementBlock = nullptr 
                           );

    virtual ~SyntaxNodeIfStatement();

  public:
    virtual NodeType getType() const override { return NodeType::NODE_IF_STATEMENT; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const override;

    virtual bool generateCode( crunch::core::BinaryBuffer *dest ) override;

};

} }

#endif
