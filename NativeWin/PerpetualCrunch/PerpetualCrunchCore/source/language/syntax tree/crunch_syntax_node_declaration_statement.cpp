
#include "../../crunch_global.h"

#include "../crunch_script_compiler.h"
#include "../crunch_symbol_stack.h"

#include "crunch_syntax_node_function_declaration.h"

#include "crunch_syntax_node_declaration_statement.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

SyntaxNodeDeclarationStatement::SyntaxNodeDeclarationStatement( ScriptCompiler *compiler,
                                                                SyntaxNodeRoot *root,
                                                                unsigned int lineNumber,
                                                                SyntaxNode *typeIdentifier, 
                                                                const crunch::core::String& identifier,
                                                                SyntaxNode *expression
                                                                ) :
  SyntaxNode( compiler, root, lineNumber ),
  mIdentifier( identifier )
{
  append( typeIdentifier );
  append( expression, false );
}

SyntaxNodeDeclarationStatement::~SyntaxNodeDeclarationStatement()
{
  // empty
}

crunch::core::String SyntaxNodeDeclarationStatement::getSyntaxTreeDescription( const crunch::core::String& indent ) const
{
  crunch::core::String d = crunch::core::String::fmt( "%s%u: DECLARATION STATEMENT %s (children: %u)\n", indent.c_str(), mLineNumber, mIdentifier.c_str(), (unsigned int)( mChildren.size() ) );
  d += getSyntaxTreeChildrenDescription( indent + mIndentStep );
  return d;
}

void SyntaxNodeDeclarationStatement::applyConstantFolding()
{
  if ( mChildren.size() != 2 || mChildren[1] == nullptr || mChildren[1]->getType() == NodeType::NODE_LITERAL_TYPE )
    return;

  ScriptValue v = mChildren[1]->getScriptValue();

  if ( v.isConstantFoldType() )
  {
    unsigned int lineNumber = mChildren[1]->getSourceLineNumber();

    delete mChildren[1];

    mChildren[1] = new SyntaxNodeLiteralType( mCompiler, mRoot, lineNumber, v );
  }
  else
    mChildren[1]->applyConstantFolding();
}

bool SyntaxNodeDeclarationStatement::generateCode( crunch::core::BinaryBuffer *dest )
{
  if ( mChildren.size() != 1 && mChildren.size() != 2 )
  {
    reportInternalError( "unexpected number of children nodes on declaration statement node | total children: %u", (unsigned int)( mChildren.size() ) );
    return false;
  }

  if ( mChildren[0] == nullptr )
  {
    reportInternalError( "missing type identifier node" );
    return false;
  }

  if ( mChildren[0]->getType() != NodeType::NODE_TYPE_IDENTIFIER )
  {
    reportInternalError( "expected type identifier node for first child" );
    return false;
  }

  ScriptValue initializerValue;
  
  if ( mChildren.size() == 2 && mChildren[1] != nullptr )
    initializerValue = mChildren[1]->getScriptValue();

  SyntaxNodeFunctionDeclaration *currentFunctionDeclaration = SyntaxNodeFunctionDeclaration::getCurrentFunctionDeclaration();

  if ( currentFunctionDeclaration == nullptr )
  {
    // object property

    if ( initializerValue.getType() == ScriptTypes::ScriptType::TYPE_NONE )
    {
      reportError( "all properties require a compile-time initializer expression" );
      return false;
    }

    if ( !( ScriptObject::isAllowedPropertyType( initializerValue.getType() ) ) )
    {
      reportError( "script type '%s' is not an allowed property type", ScriptTypes::getScriptTypeName( initializerValue.getType() ).c_str() );
      return false;
    }

    ScriptObject *myObject = nullptr;

    {
      myObject = (ScriptObject*)( mCompiler->getGlobalObject() );
      if ( myObject == nullptr )
      {
        reportInternalError( "missing global object" );
        return false;
      }
    }

    if ( myObject->hasProperty( mIdentifier.c_str() ) )
    {
      reportError( "object '%s' already has a property '%s'", myObject->getName().c_str(), mIdentifier.c_str() );
      return false;
    }

    if ( !( myObject->addProperty( mIdentifier.c_str(), initializerValue ) ) )
    {
      reportInternalError( "failed to add property to object" );
      return false;
    }
  }
  else
  {
    // function local variable

    mCompiler->getSymbolStack()->pushSymbol( mIdentifier, (SyntaxNodeTypeIdentifier*)( mChildren[0] ) );

    if ( mChildren.size() == 2 && mChildren[1] != nullptr )
    {
      if ( initializerValue.getType() == ScriptTypes::ScriptType::TYPE_NONE )
      {
        // generate code for initializer

        SCOPED_SCRIPT_CONTEXT( FETCH );

        if ( !( mChildren[1]->generateCode( dest ) ) )
          return false;
      }
      else
      {
        // push compile-time constant for initializer

        if ( !( initializerValue.generateCode( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) )
          return false;
      }
    }
    else
    {
      // initialize to default value for the given type

      // TODO: support library class default constructors

      ScriptValue defaultValue( mCompiler, mChildren[0]->getScriptTypeDescription().mScriptType );

      if ( !( defaultValue.generateCode( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) )
        return false;
    }
  }

  return true;
}

} }
