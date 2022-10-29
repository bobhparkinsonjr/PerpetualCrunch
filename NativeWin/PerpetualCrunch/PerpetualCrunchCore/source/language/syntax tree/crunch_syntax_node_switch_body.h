#if !defined ( CRUNCH_SYNTAX_NODE_SWITCH_BODY_H )
#define CRUNCH_SYNTAX_NODE_SWITCH_BODY_H

#include "crunch_syntax_node.h"
#include "crunch_syntax_node_breakable_statement.h"

namespace crunch { namespace language {

class SyntaxNodeSwitchCase;

class SyntaxNodeSwitchBody : public SyntaxNodeBreakableStatement
{
  public:
    SyntaxNodeSwitchBody( ScriptCompiler *compiler = nullptr, SyntaxNodeRoot *root = nullptr, unsigned int lineNumber = 0 );
    virtual ~SyntaxNodeSwitchBody();

  public:
    virtual NodeType getType() const override { return NodeType::NODE_SWITCH_BODY; }

    virtual crunch::core::String getSyntaxTreeDescription( const crunch::core::String& indent = "" ) const override;

    virtual bool generateCode( crunch::core::BinaryBuffer *dest ) override;

  private:
    class SwitchCaseInfo
    {
      public:
        SwitchCaseInfo( crunch_i64 v = 0, SyntaxNodeSwitchCase *switchCase = nullptr ) : 
          mExpressionValue( v ), 
          mSwitchCase( switchCase ),
          mOffset( 0 ) 
        {
          // empty
        }

      public:
        crunch_i64 mExpressionValue = 0;
        SyntaxNodeSwitchCase *mSwitchCase = nullptr;
        uint32_t mOffset = 0;

    };

    static int switchCaseInfoCompare( void *context, const void *a, const void *b );

};

} }

#endif
