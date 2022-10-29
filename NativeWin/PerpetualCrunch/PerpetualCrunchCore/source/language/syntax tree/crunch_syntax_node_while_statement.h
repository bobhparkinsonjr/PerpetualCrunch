#if !defined ( CRUNCH_SYNTAX_NODE_WHILE_STATEMENT_H )
#define CRUNCH_SYNTAX_NODE_WHILE_STATEMENT_H

#include "crunch_syntax_node.h"
#include "crunch_syntax_node_breakable_statement.h"

namespace crunch { namespace language {

class SyntaxNodeBreakStatement;

class SyntaxNodeWhileStatement : public SyntaxNodeBreakableStatement
{
  private:
    typedef enum
    {
      EXPRESSION,
      STATEMENT_BLOCK,

      TOTAL
    } ChildType;

  public:
    SyntaxNodeWhileStatement( ScriptCompiler *compiler = nullptr, 
                              SyntaxNodeRoot *root = nullptr,
                              unsigned int lineNumber = 0, 
                              SyntaxNode *expression = nullptr, 
                              SyntaxNode *statementBlock = nullptr
                              );

    virtual ~SyntaxNodeWhileStatement();

  public:
    virtual NodeType getType() const override { return NodeType::NODE_WHILE_STATEMENT; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const override;

    virtual bool generateCode( crunch::core::BinaryBuffer *dest ) override;

};

} }

#endif
