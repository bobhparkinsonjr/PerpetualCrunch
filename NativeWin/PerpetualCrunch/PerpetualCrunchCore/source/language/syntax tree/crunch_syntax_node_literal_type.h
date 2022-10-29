#if !defined ( CRUNCH_SYNTAX_NODE_LITERAL_TYPE_H )
#define CRUNCH_SYNTAX_NODE_LITERAL_TYPE_H

#include "crunch_syntax_node.h"

#include "../crunch_script_types.h"
#include "../crunch_script_value.h"

namespace crunch { namespace language {

class SyntaxNodeLiteralType : public SyntaxNode
{
  public:
    enum class LiteralType
    {
      LITERAL_NONE = -1,

      LITERAL_I64,
      LITERAL_U64,
      LITERAL_F64,
      LITERAL_STRING,
      LITERAL_CHAR,

      LITERAL_TOTAL
    };

  private:
    union LiteralValue
    {
      crunch_i64 i;
      crunch_u64 u;
      crunch_f64 f;
      const crunch_string *s;
      crunch_char c;
    };

  public:
    SyntaxNodeLiteralType( ScriptCompiler *compiler = nullptr, SyntaxNodeRoot *root = nullptr, unsigned int lineNumber = 0 );
    explicit SyntaxNodeLiteralType( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber, crunch_i64 i );
    explicit SyntaxNodeLiteralType( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber, crunch_u64 u );
    explicit SyntaxNodeLiteralType( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber, crunch_f64 f );
    explicit SyntaxNodeLiteralType( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber, const crunch_string *s );
    explicit SyntaxNodeLiteralType( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber, char c );
    explicit SyntaxNodeLiteralType( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber, const ScriptValue& v );
    virtual ~SyntaxNodeLiteralType();

  public:
    virtual NodeType getType() const override { return NodeType::NODE_LITERAL_TYPE; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const override;

    virtual crunch::core::String getSourceDescription() const override;

    virtual bool generateCode( crunch::core::BinaryBuffer *dest ) override;

    virtual ScriptTypes::ScriptTypeDescription getScriptTypeDescription() const override;

    virtual ScriptValue getScriptValue() const override;

    core_forceinline crunch_i64 getValueI64() const { return mValue.i; }
    core_forceinline crunch_u64 getValueU64() const { return mValue.u; }
    core_forceinline crunch_f64 getValueF64() const { return mValue.f; }
    core_forceinline const crunch_string* getValueString() const { return mValue.s; }
    core_forceinline crunch_char getValueChar() const { return mValue.c; }

  private:
    LiteralType mType;
    LiteralValue mValue;

};

} }

#endif
