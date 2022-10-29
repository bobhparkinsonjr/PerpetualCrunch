#if !defined ( CRUNCH_SYNTAX_NODE_TYPE_IDENTIFIER_H )
#define CRUNCH_SYNTAX_NODE_TYPE_IDENTIFIER_H

#include "../crunch_script_types.h"

#include "crunch_syntax_node.h"

namespace crunch { namespace language {

class SyntaxNodeTypeIdentifier : public SyntaxNode
{
  public:
    enum { ID_MAX_LENGTH = 63 };

  public:
    SyntaxNodeTypeIdentifier( ScriptCompiler *compiler = nullptr,
                              SyntaxNodeRoot *root = nullptr,
                              unsigned int lineNumber = 0, 
                              ScriptTypes::ScriptType theType = ScriptTypes::ScriptType::TYPE_NONE, 
                              const crunch::core::String& identifier = "" 
                              );

    virtual ~SyntaxNodeTypeIdentifier();

  public:
    virtual NodeType getType() const override { return NodeType::NODE_TYPE_IDENTIFIER; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const override;

    virtual ScriptTypes::ScriptTypeDescription getScriptTypeDescription() const override { resolveScriptType(); return mScriptTypeDescription; }

    // in bytes
    unsigned int getDataStackSize() const;

    // if ScriptType is none, and the identifier is non-empty, then this will attempt to resolve the ScriptType
    void resolveScriptType() const;

    core_forceinline crunch::core::String getIdentifier() const { return mScriptTypeDescription.mIdentifier; }

  private:
    mutable ScriptTypes::ScriptTypeDescription mScriptTypeDescription;

};

} }

#endif
