#if !defined ( CRUNCH_SYNTAX_NODE_DECLARATION_STATEMENT_H )
#define CRUNCH_SYNTAX_NODE_DECLARATION_STATEMENT_H

#include "crunch_syntax_node.h"
#include "crunch_syntax_node_type_identifier.h"
#include "crunch_syntax_node_expression.h"

namespace crunch { namespace language {

class SyntaxNodeDeclarationStatement : public SyntaxNode
{
  public:
    SyntaxNodeDeclarationStatement( ScriptCompiler *compiler = nullptr,
                                    SyntaxNodeRoot *root = nullptr,
                                    unsigned int lineNumber = 0,
                                    SyntaxNode *typeIdentifier = nullptr, 
                                    const crunch::core::String& identifier = "", 
                                    SyntaxNode *expression = nullptr
                                    );

    virtual ~SyntaxNodeDeclarationStatement();

  public:
    virtual NodeType getType() const override { return NodeType::NODE_DECLARATION_STATEMENT; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const override;

    virtual void applyConstantFolding() override;

    virtual bool generateCode( crunch::core::BinaryBuffer *dest ) override;

  private:
    crunch::core::String mIdentifier;

};

} }

#endif
