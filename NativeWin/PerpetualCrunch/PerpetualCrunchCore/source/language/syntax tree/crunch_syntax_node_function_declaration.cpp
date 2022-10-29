
#include "../../crunch_global.h"

#include "../crunch_script_compiler.h"
#include "../crunch_script_executable.h"
#include "../crunch_script_object.h"
#include "../crunch_symbol_stack.h"
#include "../crunch_script_instruction.h"
#include "../crunch_script_library.h"
#include "../crunch_script_library_class.h"

#include "crunch_syntax_node_function_declaration.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

///////////////////////////////////////////////////////////////////////////////////////////////////

SyntaxNodeFunctionDeclaration *SyntaxNodeFunctionDeclaration::mCurrentFunctionDeclaration = nullptr;

///////////////////////////////////////////////////////////////////////////////////////////////////

SyntaxNodeFunctionDeclaration::SyntaxNodeFunctionDeclaration( ScriptCompiler *compiler,
                                                              SyntaxNodeRoot *root,
                                                              unsigned int lineNumber,
                                                              SyntaxNode *typeIdentifier, 
                                                              const crunch::core::String& name, 
                                                              SyntaxNode *parameterList, 
                                                              SyntaxNode *body
                                                              ) :
  SyntaxNode( compiler, root, lineNumber ),
  mName( name ),
  mStartScopeLevel( 0 )
{
  append( typeIdentifier );
  append( parameterList ); // can be nullptr
  append( body ); // can be nullptr

  if ( typeIdentifier != nullptr )
    mLineNumber = typeIdentifier->getSourceLineNumber();
}

SyntaxNodeFunctionDeclaration::~SyntaxNodeFunctionDeclaration()
{
  // empty
}

crunch::core::String SyntaxNodeFunctionDeclaration::getSyntaxTreeDescription( const crunch::core::String& indent ) const
{
  if ( (unsigned int)( mChildren.size() ) != ChildType::TOTAL )
    return "";

  crunch::core::String d = crunch::core::String::fmt( "%s%u: FUNCTION DECLARATION %s\n", indent.c_str(), mLineNumber, mName.c_str() );

  crunch::core::String i0 = indent + mIndentStep;
  crunch::core::String i1 = indent + mIndentStep + mIndentStep;

  if ( mChildren[ChildType::RETURN_TYPE] != nullptr )
    d += ( crunch::core::String::fmt( "%sRETURN TYPE\n", i0.c_str() ) + mChildren[ChildType::RETURN_TYPE]->getSyntaxTreeDescription( i1 ) );
  else
    d += crunch::core::String::fmt( "%sRETURN TYPE : internal error: missing the return type\n", i0.c_str() );

  if ( mChildren[ChildType::PARAMETERS] != nullptr )
    d += ( crunch::core::String::fmt( "%sPARAMETERS\n", i0.c_str() ) + mChildren[ChildType::PARAMETERS]->getSyntaxTreeDescription( i1 ) );

  if ( mChildren[ChildType::BODY] != nullptr )
    d += ( crunch::core::String::fmt( "%sBODY\n", i0.c_str() ) + mChildren[ChildType::BODY]->getSyntaxTreeDescription( i1 ) );

  return d;
}

bool SyntaxNodeFunctionDeclaration::generateCode( crunch::core::BinaryBuffer *dest )
{
  if ( mCompiler->isDeadStrippingEnabled() && !mAlive )
    return true;

  if ( mCurrentFunctionDeclaration != nullptr )
  {
    reportInternalError( "already inside of a function declaration" );
    return false;
  }

  if ( (unsigned int)( mChildren.size() ) != ChildType::TOTAL )
  {
    reportInternalError( "function declaration has unexpected number of children nodes" );
    return false;
  }

  mCurrentFunctionDeclaration = this;

  mCompiler->getSymbolStack()->pushScope();

  mStartScopeLevel = mCompiler->getSymbolStack()->getCurrentScopeLevel();

  // register function with object
  {
    {
      mScriptObject = (ScriptObject*)( mCompiler->getGlobalObject() );
      if ( mScriptObject == nullptr )
      {
        reportInternalError( "missing global object" );
        return false;
      }
    }

    unsigned int functionOffset = dest->getBufferLength();

    if ( !( setupFunctionSignature() ) )
      return false;

    if ( mScriptObject->hasFunctionSignature( mFunctionSignature ) )
    {
      reportError( "script object '%s' already has function '%s' with signature '%s'", mScriptObject->getName().c_str(), mName.c_str(), mFunctionSignature.c_str() );
      return false;
    }

    if ( ScriptLibrary::getGlobalLibraryClass()->hasFunctionSignature( mFunctionSignature ) )
    {
      reportError( "global library class already has function '%s' with signature '%s'", mName.c_str(), mFunctionSignature.c_str() );
      return false;
    }

    if ( ScriptLibrary::hasLibraryClass( mName.c_str() ) )
    {
      reportError( "function '%s' conflicts with library class of same name", mName.c_str() );
      return false;
    }

    if ( !( mScriptObject->addFunction( mName, functionOffset, mParameterTypes, mReturnTypeDescription, this ) ) )
    {
      reportInternalError( "failed to add function to object" );
      return false;
    }
  }

  // push parameter symbols to function scope
  {
    if ( mChildren[ChildType::PARAMETERS] != nullptr )
    {
      SCOPED_SCRIPT_CONTEXT( BEGIN_SCOPE );

      if ( !( mChildren[ChildType::PARAMETERS]->generateCode( dest ) ) )
        return false;
    }
  }

  bool retVal = true;

  if ( mChildren[ChildType::BODY] != nullptr )
  {
    if ( !( mChildren[ChildType::BODY]->generateCode( dest ) ) )
      retVal = false;
  }

  if ( mChildren[ChildType::RETURN_TYPE] != nullptr )
  {
    if ( mChildren[ChildType::RETURN_TYPE]->getType() == NodeType::NODE_TYPE_IDENTIFIER )
    {
      ScriptTypes::ScriptTypeDescription typeDescription = mChildren[ChildType::RETURN_TYPE]->getScriptTypeDescription();

      if ( mChildren[ChildType::BODY] != nullptr )
      {
        if ( mChildren[ChildType::BODY]->getType() == NodeType::NODE_STATEMENT_LIST )
        {
          const SyntaxNode *lastStatement = mChildren[ChildType::BODY]->getLastChild();

          if ( typeDescription.mScriptType != ScriptTypes::ScriptType::TYPE_VOID )
          {
            if ( lastStatement == nullptr || lastStatement->getType() != NodeType::NODE_RETURN_STATEMENT )
              reportError( "function must return a value" );
          }
          else
          {
            if ( lastStatement == nullptr || lastStatement->getType() != NodeType::NODE_RETURN_STATEMENT )
            {
              // popping parameters for void return type, because no return node

              CRUNCH_MCOMPILER_APPEND_ASSEMBLER( getSourceFilePath(), getSourceLineNumber(), crunch::core::String::fmt( "\n  ; exit function '%s'", mName.c_str() ) );

              if ( !( generateCodePopLocals( dest ) ) )
                retVal = false;

              if ( !( ScriptInstruction::write_RETURN( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) )
                retVal = false;
            }
          }
        }
        else
        {
          reportInternalError( "function declaration body node has unexpected type" );
          retVal = false;
        }
      }
      else
      {
        // empty body

        if ( typeDescription.mScriptType != ScriptTypes::ScriptType::TYPE_VOID )
          reportError( "function cannot be empty, function must return a value" );
        else
        {
          // popping parameters for void return type, because no return node

          CRUNCH_MCOMPILER_APPEND_ASSEMBLER( getSourceFilePath(), getSourceLineNumber(), crunch::core::String::fmt( "\n  ; exit function '%s'", mName.c_str() ) );

          if ( !( generateCodePopLocals( dest ) ) )
            retVal = false;

          if ( !( ScriptInstruction::write_RETURN( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) )
            retVal = false;
        }
      }
    }
    else
    {
      reportInternalError( "function declaration return type has unexpected node type" );
      retVal = false;
    }
  }
  else
  {
    reportInternalError( "function declaration is missing return type node" );
    retVal = false;
  }

  mCompiler->getSymbolStack()->popScope();

  mCurrentFunctionDeclaration = nullptr;

  return retVal;
}

void SyntaxNodeFunctionDeclaration::queryAliveStatus()
{
  if ( !( setupFunctionSignature() ) )
    return;

  if ( mFunctionSignature == "" )
    reportInternalError( "function declaration is missing function signature" );

  if ( !mQueryAliveStatusComplete && ( mCompiler->isRootFunction( mFunctionSignature ) || mAlive ) )
  {
    mQueryAliveStatusComplete = true;
    setAlive();
    SyntaxNode::queryAliveStatus();
  }
}

bool SyntaxNodeFunctionDeclaration::generateCodePopLocals( crunch::core::BinaryBuffer *dest )
{
  if ( mCurrentFunctionDeclaration != this )
    return false;

  return ScriptInstruction::write_POP( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), (uint32_t)( getScopeStackSize() ), "pop locals" );
}

unsigned int SyntaxNodeFunctionDeclaration::getScopeStackSize() const
{
  return mCompiler->getSymbolStack()->getStackSize( mStartScopeLevel );
}

ScriptTypes::ScriptTypeDescription SyntaxNodeFunctionDeclaration::getReturnType() const
{
  if ( (unsigned int)( mChildren.size() ) == ChildType::TOTAL )
  {
    if ( mChildren[ChildType::RETURN_TYPE] != nullptr && mChildren[ChildType::RETURN_TYPE]->getType() == NodeType::NODE_TYPE_IDENTIFIER )
      return mChildren[ChildType::RETURN_TYPE]->getScriptTypeDescription();
  }

  return ScriptTypes::ScriptTypeDescription();
}

void SyntaxNodeFunctionDeclaration::reportDependencies( ScriptObject *so, const crunch::core::String& functionSignature )
{
  if ( mScriptObject == nullptr )
  {
    reportInternalError( "missing script object to add dependencies" );
    return;
  }

  if ( so == nullptr )
  {
    reportInternalError( "missing source script object to add dependencies" );
    return;
  }

  if ( !( mScriptObject->addFunctionDependencies( mFunctionSignature, so, functionSignature ) ) )
  {
    reportInternalError( "failed to add dependencies for function '%s' from function '%s' contained in object '%s'", mFunctionSignature.c_str(), functionSignature.c_str(), so->getName().c_str() );
    return;
  }
}

bool SyntaxNodeFunctionDeclaration::setupFunctionSignature()
{
  if ( mFunctionSignature != "" )
    return true;

  if ( mChildren[ChildType::PARAMETERS] != nullptr )
    mChildren[ChildType::PARAMETERS]->getChildrenScriptTypeDescriptions( &mParameterTypes );

  if ( mChildren[ChildType::RETURN_TYPE] != nullptr )
    mReturnTypeDescription = mChildren[ChildType::RETURN_TYPE]->getScriptTypeDescription();

  if ( !( ScriptExecutable::getFunctionSignature( mName, mParameterTypes, &mFunctionSignature ) ) )
  {
    reportInternalError( "failed to generate function signature for function '%s'", mName.c_str() );
    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

} }
