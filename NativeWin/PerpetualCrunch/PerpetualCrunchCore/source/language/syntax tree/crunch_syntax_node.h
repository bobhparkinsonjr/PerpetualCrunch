#if !defined ( CRUNCH_SYNTAX_NODE_H )
#define CRUNCH_SYNTAX_NODE_H

#include "../crunch_script_types.h"
#include "../crunch_script_value.h"

namespace crunch { namespace language {

class ScriptCompiler;
class SyntaxNodeRoot;

class SyntaxNode
{
  public:
    enum class NodeType
    {
      NODE_NONE = -1,

      #define ROW( _enum_name, _class_name ) NODE_##_enum_name ,
      #include "crunch_syntax_nodes.table"
      #undef ROW

      NODE_TOTAL
    };

    typedef std::vector< SyntaxNode* > ChildrenListType;

  public:
    SyntaxNode( ScriptCompiler *compiler = nullptr, SyntaxNodeRoot *root = nullptr, unsigned int lineNumber = 0 );
    virtual ~SyntaxNode();

  public:
    virtual NodeType getType() const { return NodeType::NODE_BASE; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const { return ( indent + "BASE\n" ); }

    virtual crunch::core::String getSourceDescription() const { return ""; }

    virtual void append( SyntaxNode *node, bool allowNullNode = true );

    virtual void setParentContextNode( SyntaxNode *node ) {}

    virtual void applyConstantFolding();

    virtual bool generateCode( crunch::core::BinaryBuffer *dest ) { return false; }

    virtual void queryAliveStatus();

    virtual void setStatementContext( bool v ) { }

    virtual ScriptTypes::ScriptTypeDescription getScriptTypeDescription() const { return ScriptTypes::ScriptTypeDescription(); }

    core_forceinline ScriptTypes::ScriptType getScriptType() const { return getScriptTypeDescription().mScriptType; }

    virtual ScriptValue getScriptValue() const { return ScriptValue(); }

    virtual bool isLValue() const { return false; }

    unsigned int getSourceLineNumber() const;
    crunch::core::WideString getSourceFilePath() const;

    // performs depth-first search for node by type
    const SyntaxNode* findNode( NodeType v ) const;

    // upon success, returns the node at the end of the tree, otherwise returns NULL
    // tree has the simple form: this-node -> child-node
    // to match, this node must have type thisNode, and have exactly one child with type childNode1,
    //   and childNode1 must have no children
    const SyntaxNode* matchTree2( NodeType thisNode, NodeType childNode1, bool allowChildNode1Children = false ) const;

    core_forceinline const SyntaxNode* getChild( int i ) const { if ( i >= 0 && i < int( mChildren.size() ) ) return mChildren[i]; return nullptr; }
    core_forceinline const SyntaxNode* getLastChild() const { if ( !( mChildren.empty() ) ) return mChildren[mChildren.size() - 1]; return nullptr; }

    bool getChildrenScriptTypeDescriptions( std::vector< ScriptTypes::ScriptTypeDescription > *dest ) const;

    crunch::core::String getChildrenSourceDescription() const;

    core_forceinline SyntaxNode* addReference() { ++mReferenceCount; return this; }

  public:
    void reportInternalError( const char *format, ... ) const;
    void reportError( const char *format, ... ) const;
    void reportWarning( const char *format, ... ) const;

  protected:
    crunch::core::String getSyntaxTreeChildrenDescription( const crunch::core::String& indent = "" ) const;

    bool generateChildrenCode( crunch::core::BinaryBuffer *dest );

    void deleteChildren();

  protected:
    ChildrenListType mChildren;

    ScriptCompiler *mCompiler = nullptr;
    SyntaxNodeRoot *mRoot = nullptr;
    unsigned int mLineNumber = 0;

    int mReferenceCount = 1;

  protected:
    static crunch::core::String mIndentStep;

};

} }

#endif
