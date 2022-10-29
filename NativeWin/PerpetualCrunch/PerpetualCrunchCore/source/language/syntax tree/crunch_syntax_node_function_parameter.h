#if !defined ( CRUNCH_SYNTAX_NODE_FUNCTION_PARAMETER_H )
#define CRUNCH_SYNTAX_NODE_FUNCTION_PARAMETER_H

#include "crunch_syntax_node.h"

namespace crunch { namespace language {

class SyntaxNodeFunctionParameter : public SyntaxNode
{
  public:
    SyntaxNodeFunctionParameter( ScriptCompiler *compiler = nullptr, SyntaxNodeRoot *root = nullptr, unsigned int lineNumber = 0, SyntaxNode *typeIdentifier = nullptr, const crunch::core::String& identifier = "" );
    virtual ~SyntaxNodeFunctionParameter();

  public:
    virtual NodeType getType() const override { return NodeType::NODE_FUNCTION_PARAMETER; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const override;

    virtual bool generateCode( crunch::core::BinaryBuffer *dest ) override;

    virtual ScriptTypes::ScriptTypeDescription getScriptTypeDescription() const override;

  private:
    crunch::core::String mIdentifier;

};

} }

#endif
