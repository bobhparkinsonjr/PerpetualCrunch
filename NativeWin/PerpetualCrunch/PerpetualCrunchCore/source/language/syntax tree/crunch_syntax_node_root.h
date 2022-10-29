#if !defined ( CRUNCH_SYNTAX_NODE_ROOT_H )
#define CRUNCH_SYNTAX_NODE_ROOT_H

#include "crunch_syntax_node.h"

namespace crunch { namespace language {

class SyntaxNodeRoot : public SyntaxNode
{
  public:
    SyntaxNodeRoot( ScriptCompiler *compiler = nullptr, SyntaxNodeRoot *root = nullptr, unsigned int lineNumber = 0 );
    virtual ~SyntaxNodeRoot();

  public:
    virtual NodeType getType() const override { return NodeType::NODE_ROOT; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const override;

    virtual bool generateCode( crunch::core::BinaryBuffer *dest ) override;

    core_forceinline void setSourceFilePath( const crunch::core::WideString& v ) { mSourceFilePath = v; }
    core_forceinline crunch::core::WideString getSourceFilePath() const { return mSourceFilePath; }

    void parseIncludeNodes();

  private:
    crunch::core::WideString mSourceFilePath;

};

} }

#endif
