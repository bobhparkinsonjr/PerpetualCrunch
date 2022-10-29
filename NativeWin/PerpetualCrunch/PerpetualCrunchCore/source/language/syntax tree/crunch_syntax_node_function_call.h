#if !defined ( CRUNCH_SYNTAX_NODE_FUNCTION_CALL_H )
#define CRUNCH_SYNTAX_NODE_FUNCTION_CALL_H

#include "crunch_syntax_node.h"
#include "../crunch_script_types.h"

namespace crunch { namespace language {

class SyntaxNodeExpressionList;
class SyntaxNodeFunctionDeclaration;

class SyntaxNodeFunctionCall : public SyntaxNode
{
  private:
    typedef enum
    {
      TYPE_IDENTIFIER,
      PARAMETERS,

      TOTAL
    } ChildType;

  public:
    SyntaxNodeFunctionCall( ScriptCompiler *compiler = nullptr, SyntaxNodeRoot *root = nullptr, unsigned int lineNumber = 0, SyntaxNode *typeIdentifier = nullptr, SyntaxNode *parameters = nullptr );
    virtual ~SyntaxNodeFunctionCall();

  public:
    virtual NodeType getType() const override { return NodeType::NODE_FUNCTION_CALL; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const override;

    virtual crunch::core::String getSourceDescription() const override;

    virtual void setParentContextNode( SyntaxNode *node ) override;

    virtual bool generateCode( crunch::core::BinaryBuffer *dest ) override;

    virtual void queryAliveStatus() override;

    virtual ScriptTypes::ScriptTypeDescription getScriptTypeDescription() const override;

    virtual ScriptValue getScriptValue() const override;

  private:
    void updateCallInfo() const;

    bool generateCodeCast( crunch::core::BinaryBuffer *dest, SyntaxNodeExpressionList *parameters );
    bool generateCodeCall( crunch::core::BinaryBuffer *dest, SyntaxNodeExpressionList *parameters );

  private:
    SyntaxNode *mParentContextNode = nullptr;

    mutable ScriptTypes::ScriptTypeDescription mCallTypeDescription;
    mutable std::vector< ScriptTypes::ScriptTypeDescription > mParameterTypes;
    mutable crunch::core::String mFunctionSignature;
    mutable SyntaxNodeFunctionDeclaration *mDeclaration = nullptr;

};

} }

#endif
