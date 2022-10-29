#if !defined ( CRUNCH_SYNTAX_NODE_FUNCTION_DECLARATION_H )
#define CRUNCH_SYNTAX_NODE_FUNCTION_DECLARATION_H

#include "crunch_syntax_node.h"

namespace crunch { namespace language {

class ScriptObject;

class SyntaxNodeFunctionDeclaration : public SyntaxNode
{
  private:
    typedef enum
    {
      RETURN_TYPE,
      PARAMETERS,
      BODY,

      TOTAL
    } ChildType;

  public:
    SyntaxNodeFunctionDeclaration( ScriptCompiler *compiler = nullptr,
                                   SyntaxNodeRoot *root = nullptr,
                                   unsigned int lineNumber = 0,
                                   SyntaxNode *typeIdentifier = nullptr, 
                                   const crunch::core::String& name = "", 
                                   SyntaxNode *parameterList = nullptr, 
                                   SyntaxNode *body = nullptr
                                   );

    virtual ~SyntaxNodeFunctionDeclaration();

  public:
    virtual NodeType getType() const override { return NodeType::NODE_FUNCTION_DECLARATION; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const override;

    virtual bool generateCode( crunch::core::BinaryBuffer *dest ) override;

    virtual void queryAliveStatus() override;

    // this will generate code to pop the parameters of the function, as well as any local data up to the current scope
    // this should be called before leaving the function body (eg. early return, final return, end-of-function no return stm)
    bool generateCodePopLocals( crunch::core::BinaryBuffer *dest );

    unsigned int getScopeStackSize() const;

    ScriptTypes::ScriptTypeDescription getReturnType() const;

    inline const crunch::core::String& getName() const { return mName; }

    void reportDependencies( ScriptObject *so, const crunch::core::String& functionSignature );

    inline void setAlive() { mAlive = true; }

  private:
    bool setupFunctionSignature();

  public:
    static inline SyntaxNodeFunctionDeclaration* getCurrentFunctionDeclaration() { return mCurrentFunctionDeclaration; }

  private:
    crunch::core::String mName;
    crunch::core::String mFunctionSignature;
    ScriptObject *mScriptObject = nullptr;
    unsigned int mStartScopeLevel;
    bool mAlive = false;
    bool mQueryAliveStatusComplete = false;
    std::vector< ScriptTypes::ScriptTypeDescription > mParameterTypes;
    ScriptTypes::ScriptTypeDescription mReturnTypeDescription;

  private:
    static SyntaxNodeFunctionDeclaration *mCurrentFunctionDeclaration;

};

} }

#endif
