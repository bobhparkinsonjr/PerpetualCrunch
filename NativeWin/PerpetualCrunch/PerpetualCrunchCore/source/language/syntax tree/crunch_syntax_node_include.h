#if !defined ( CRUNCH_SYNTAX_NODE_INCLUDE_H )
#define CRUNCH_SYNTAX_NODE_INCLUDE_H

#include "crunch_syntax_node.h"

namespace crunch { namespace language {

class SyntaxNodeRoot;

class SyntaxNodeInclude : public SyntaxNode
{
  public:
    SyntaxNodeInclude( ScriptCompiler *compiler = nullptr, SyntaxNodeRoot *root = nullptr, unsigned int lineNumber = 0, const crunch::core::String& includeItem = "" );
    virtual ~SyntaxNodeInclude();

  public:
    virtual NodeType getType() const override { return NodeType::NODE_INCLUDE; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const override;

    virtual void applyConstantFolding() override;

    virtual bool generateCode( crunch::core::BinaryBuffer *dest ) override;

    virtual void queryAliveStatus() override;

    void parseIncludeItem();

  private:
    crunch::core::String mIncludeItem;
    SyntaxNodeRoot *mIncludeRoot;

};

} }

#endif
