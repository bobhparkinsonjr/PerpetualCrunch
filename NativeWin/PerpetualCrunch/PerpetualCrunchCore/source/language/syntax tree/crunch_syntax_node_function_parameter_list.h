#if !defined ( CRUNCH_SYNTAX_NODE_FUNCTION_PARAMETER_LIST_H )
#define CRUNCH_SYNTAX_NODE_FUNCTION_PARAMETER_LIST_H

#include "crunch_syntax_node.h"

namespace crunch { namespace language {

class SyntaxNodeFunctionParameterList : public SyntaxNode
{
  public:
    SyntaxNodeFunctionParameterList( ScriptCompiler *compiler = nullptr, SyntaxNodeRoot *root = nullptr, unsigned int lineNumber = 0 );
    virtual ~SyntaxNodeFunctionParameterList();

  public:
    virtual NodeType getType() const override { return NodeType::NODE_FUNCTION_PARAMETER_LIST; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const override;

    virtual bool generateCode( crunch::core::BinaryBuffer *dest ) override;

};

} }

#endif
