#if !defined ( CRUNCH_SYNTAX_NODE_VARIABLE_H )
#define CRUNCH_SYNTAX_NODE_VARIABLE_H

#include "crunch_syntax_node.h"

namespace crunch { namespace language {

class SyntaxNodeVariable : public SyntaxNode
{
  public:
    typedef enum
    {
      SUB_TYPE_NONE = -1,

      SUB_TYPE_MEMBER,
      SUB_TYPE_SUBSCRIPT,

      SUB_TYPE_TOTAL
    } SubType;

  public:
    SyntaxNodeVariable( ScriptCompiler *compiler = nullptr, 
                        SyntaxNodeRoot *root = nullptr, 
                        unsigned int lineNumber = 0, 
                        const crunch::core::String& identifier = "" 
                        );

    SyntaxNodeVariable( ScriptCompiler *compiler, 
                        SyntaxNodeRoot *root, 
                        unsigned int lineNumber, 
                        SyntaxNode *parentExpression, 
                        SyntaxNode *subExpression,
                        SubType st
                        );

    virtual ~SyntaxNodeVariable();

  public:
    virtual NodeType getType() const override { return NodeType::NODE_VARIABLE; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const override;

    virtual crunch::core::String getSourceDescription() const override;

    virtual void setParentContextNode( SyntaxNode *node ) override;

    virtual bool generateCode( crunch::core::BinaryBuffer *dest ) override;

    virtual ScriptTypes::ScriptTypeDescription getScriptTypeDescription() const override;

    virtual ScriptValue getScriptValue() const override;

    virtual bool isLValue() const override { return true; }

  public:
    static crunch::core::String getSubTypeName( SubType st );
    static SubType getSubType( const crunch::core::String& n );

  private:
    bool generateMemberFetchCode( crunch::core::BinaryBuffer *dest, SyntaxNode *parent, SyntaxNodeFunctionCall *memberFunctionCall );

    bool generateMemberFetchCode( crunch::core::BinaryBuffer *dest, SyntaxNode *parent, SyntaxNodeVariable *member );

    bool generateLibraryClassMemberFetchCode( crunch::core::BinaryBuffer *dest, 
                                              const ScriptTypes::ScriptTypeDescription& parentScriptTypeDescription,
                                              SyntaxNode *parent, 
                                              SyntaxNodeVariable *member 
                                              );

    bool generateFetchCode( crunch::core::BinaryBuffer *dest );

    bool generateSubscriptFetchCode( crunch::core::BinaryBuffer *dest, SyntaxNode *parent, SyntaxNode *subscript );

  private:
    bool generateStoreCode( crunch::core::BinaryBuffer *dest, SyntaxNodeVariable *parent, SyntaxNodeVariable *member );
    bool generateLibraryClassMemberStoreCode( crunch::core::BinaryBuffer *dest, SyntaxNode *parentNode, unsigned int parentOffset, SyntaxNodeVariable *parent, SyntaxNodeVariable *member );
    bool generateStoreCode( crunch::core::BinaryBuffer *dest );
    bool generateSubscriptStoreCode( crunch::core::BinaryBuffer *dest, SyntaxNode *parent, SyntaxNode *subscript );

  private:
    SyntaxNode *mParentContextNode = nullptr;
    crunch::core::String mIdentifier;
    SubType mSubType = SubType::SUB_TYPE_NONE;

};

} }

#endif
