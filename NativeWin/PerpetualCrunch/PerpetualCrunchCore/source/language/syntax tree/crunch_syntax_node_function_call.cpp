
#include "../../crunch_global.h"

#include "../crunch_script_library.h"
#include "../crunch_script_compiler.h"
#include "../crunch_script_executable.h"
#include "../crunch_script_instruction.h"
#include "../crunch_script_object.h"

#include "../library/libclass_functions_global.h"

#include "crunch_syntax_node_expression_list.h"

#include "crunch_syntax_node_function_call.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

SyntaxNodeFunctionCall::SyntaxNodeFunctionCall( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber, SyntaxNode *typeIdentifier, SyntaxNode *parameters ) :
  SyntaxNode( compiler, root, lineNumber )
{
  append( typeIdentifier );
  append( parameters );
}

SyntaxNodeFunctionCall::~SyntaxNodeFunctionCall()
{
  // empty
}

crunch::core::String SyntaxNodeFunctionCall::getSyntaxTreeDescription( const crunch::core::String& indent ) const
{
  unsigned int totalChildren = (unsigned int)( mChildren.size() );

  crunch::core::String d = crunch::core::String::fmt( "%s%u: FUNCTION CALL (children: %u)\n", indent.c_str(), mLineNumber, totalChildren );

  crunch::core::String i0 = indent + mIndentStep;
  crunch::core::String i1 = indent + mIndentStep + mIndentStep;
  
  if ( totalChildren == ChildType::TOTAL )
  {
    if ( mChildren[ChildType::TYPE_IDENTIFIER] != nullptr )
    {
      d += crunch::core::String::fmt( "%sFUNCTION/TYPE\n", i0.c_str() );
      d += mChildren[ChildType::TYPE_IDENTIFIER]->getSyntaxTreeDescription( i1 );
    }
    else
      d += crunch::core::String::fmt( "%sFUNCTION/TYPE: internal error: missing type identifier\n", i0.c_str() );

    if ( mChildren[ChildType::PARAMETERS] != nullptr )
    {
      d += crunch::core::String::fmt( "%sPARAMETERS\n", i0.c_str() );
      d += mChildren[ChildType::PARAMETERS]->getSyntaxTreeDescription( i1 );
    }
  }
  else
    d += crunch::core::String::fmt( "%sinternal error: unexpected number of children\n", i0.c_str() );

  return d;
}

crunch::core::String SyntaxNodeFunctionCall::getSourceDescription() const
{
  updateCallInfo();

  if ( mCallTypeDescription.mIdentifier == "" )
  {
    // cast operation

    if ( mParameterTypes.size() != 1 )
      return "";

    return ScriptTypes::getScriptTypeKeyword( mCallTypeDescription.mScriptType ) + "(" + mChildren[ChildType::PARAMETERS]->getLastChild()->getSourceDescription() + ")";
  }

  return mFunctionSignature;
}

void SyntaxNodeFunctionCall::setParentContextNode( SyntaxNode *node )
{
  mParentContextNode = node;
}

bool SyntaxNodeFunctionCall::generateCode( crunch::core::BinaryBuffer *dest )
{
  unsigned int totalChildren = (unsigned int)( mChildren.size() );

  if ( totalChildren != ChildType::TOTAL )
  {
    reportInternalError( "unexpected number of children nodes on function call node" );
    return false;
  }

  if ( mChildren[ChildType::TYPE_IDENTIFIER] == nullptr )
  {
    reportInternalError( "missing type identifier node on function call node" );
    return false;
  }

  SyntaxNodeExpressionList *parameters = nullptr;

  if ( mChildren[ChildType::PARAMETERS] != nullptr )
  {
    if ( mChildren[ChildType::PARAMETERS]->getType() != NodeType::NODE_EXPRESSION_LIST )
    {
      reportInternalError( "expected parameters for function call to be an expression list" );
      return false;
    }

    parameters = (SyntaxNodeExpressionList*)( mChildren[ChildType::PARAMETERS] );
  }

  updateCallInfo();

  if ( mCallTypeDescription.mIdentifier == "" )
    return generateCodeCast( dest, parameters );

  return generateCodeCall( dest, parameters );
}

void SyntaxNodeFunctionCall::queryAliveStatus()
{
  updateCallInfo();

  if ( mDeclaration != nullptr )
  {
    mDeclaration->setAlive();
    mDeclaration->queryAliveStatus();
  }

  SyntaxNode::queryAliveStatus();
}

ScriptTypes::ScriptTypeDescription SyntaxNodeFunctionCall::getScriptTypeDescription() const
{
  if ( mChildren.size() != ChildType::TOTAL || mChildren[ChildType::TYPE_IDENTIFIER] == nullptr )
    return ScriptTypes::ScriptTypeDescription();

  updateCallInfo();

  if ( mCallTypeDescription.mIdentifier == "" )
  {
    // cast operation

    if ( mParameterTypes.size() != 1 )
      return ScriptTypes::ScriptTypeDescription();

    return mCallTypeDescription;
  }
  else
  {
    ScriptTypes::ScriptType objectType;
    unsigned int offset;
    ScriptTypes::ScriptTypeDescription returnType;
    crunch::core::String parentContextName;

    if ( mCompiler->findFunctionInfo( mParentContextNode, mFunctionSignature, &objectType, &offset, &returnType, &parentContextName, nullptr, nullptr ) )
      return returnType;
  }

  return ScriptTypes::ScriptTypeDescription();
}

ScriptValue SyntaxNodeFunctionCall::getScriptValue() const
{

  // TODO

  return ScriptValue();
}

void SyntaxNodeFunctionCall::updateCallInfo() const
{
  if ( !( mCallTypeDescription.empty() ) )
    return;

  unsigned int totalChildren = (unsigned int)( mChildren.size() );

  if ( totalChildren != ChildType::TOTAL )
  {
    reportInternalError( "unexpected number of children nodes on function call node" );
    return;
  }

  if ( mChildren[ChildType::TYPE_IDENTIFIER] == nullptr )
  {
    reportInternalError( "missing type identifier node on function call node" );
    return;
  }

  mCallTypeDescription = mChildren[ChildType::TYPE_IDENTIFIER]->getScriptTypeDescription();

  if ( mChildren[ChildType::PARAMETERS] != nullptr )
  {
    if ( mChildren[ChildType::PARAMETERS]->getType() != NodeType::NODE_EXPRESSION_LIST )
    {
      reportInternalError( "expected parameters for function call to be an expression list" );
      return;
    }

    SyntaxNodeExpressionList *parameters = (SyntaxNodeExpressionList*)( mChildren[ChildType::PARAMETERS] );

    if ( !( parameters->getChildrenScriptTypeDescriptions( &mParameterTypes ) ) )
    {
      reportInternalError( "failed to get function call parameter types" );
      return;
    }
  }

  if ( mCallTypeDescription.mIdentifier != "" )
  {
    if ( !( ScriptExecutable::getFunctionSignature( mCallTypeDescription.mIdentifier, mParameterTypes, &mFunctionSignature ) ) )
    {
      reportInternalError( "failed to generate function signature for function '%s'", mCallTypeDescription.mIdentifier.c_str() );
      return;
    }

    ScriptTypes::ScriptType objectType;
    unsigned int offset;
    ScriptTypes::ScriptTypeDescription returnType;
    crunch::core::String parentContextName;

    mCompiler->findFunctionInfo( mParentContextNode, mFunctionSignature, &objectType, &offset, &returnType, &parentContextName, nullptr, &mDeclaration );
  }
}

bool SyntaxNodeFunctionCall::generateCodeCast( crunch::core::BinaryBuffer *dest, SyntaxNodeExpressionList *parameters )
{
  if ( parameters != nullptr )
  {
    // push all parameters to stack, the expression list node will use a FETCH context

    if ( !( parameters->generateCode( dest ) ) )
    {
      reportError( "failed to generate code for function call '%s' parameters", mCallTypeDescription.mIdentifier.c_str() );
      return false;
    }
  }

  switch ( mCallTypeDescription.mScriptType )
  {
    case ScriptTypes::ScriptType::TYPE_I64:
    {
      if ( mParameterTypes.size() != 1 )
      {
        reportError( "expected 1 argument for i64 cast" );
        return false;
      }

      switch ( mParameterTypes[0].mScriptType )
      {
        case ScriptTypes::ScriptType::TYPE_STRING: return ScriptInstruction::write_STRING_TO_I64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() );
        case ScriptTypes::ScriptType::TYPE_I64: break;
        case ScriptTypes::ScriptType::TYPE_U64: return ScriptInstruction::write_U64_TO_I64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() );
        case ScriptTypes::ScriptType::TYPE_F64: return ScriptInstruction::write_F64_TO_I64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() );

        default:
          reportError( "unsupported type for i64 cast" );
          return false;
      }
    }
    break;

    case ScriptTypes::ScriptType::TYPE_U64:
    {
      if ( mParameterTypes.size() != 1 )
      {
        reportError( "expected 1 argument for u64 cast" );
        return false;
      }

      switch ( mParameterTypes[0].mScriptType )
      {
        case ScriptTypes::ScriptType::TYPE_STRING: return ScriptInstruction::write_STRING_TO_U64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() );
        case ScriptTypes::ScriptType::TYPE_I64: return ScriptInstruction::write_I64_TO_U64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() );
        case ScriptTypes::ScriptType::TYPE_U64: break;
        case ScriptTypes::ScriptType::TYPE_F64: return ScriptInstruction::write_F64_TO_U64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() );

        default:
          reportError( "unsupported type for u64 cast" );
          return false;
      }
    }
    break;

    case ScriptTypes::ScriptType::TYPE_F64:
    {
      if ( mParameterTypes.size() != 1 )
      {
        reportError( "expected 1 argument for f64 cast" );
        return false;
      }

      switch ( mParameterTypes[0].mScriptType )
      {
        case ScriptTypes::ScriptType::TYPE_STRING: return ScriptInstruction::write_STRING_TO_F64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() );
        case ScriptTypes::ScriptType::TYPE_I64: return ScriptInstruction::write_I64_TO_F64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() );
        case ScriptTypes::ScriptType::TYPE_U64: return ScriptInstruction::write_U64_TO_F64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() );
        case ScriptTypes::ScriptType::TYPE_F64: break;

        default:
          reportError( "unsupported type for f64 cast" );
          return false;
      }
    }
    break;

    default:
      reportError( "unsupported function call or cast operation" );
      return false;
  }

  return true;
}

bool SyntaxNodeFunctionCall::generateCodeCall( crunch::core::BinaryBuffer *dest, SyntaxNodeExpressionList *parameters )
{
  // NOTE: script type should be NONE, we now must search for this identifier to figure out what it is

  crunch::core::String parametersSourceDescription;

  if ( parameters != nullptr )
  {
    // push all parameters to stack, the expression list node will use a FETCH context

    if ( !( parameters->generateCode( dest ) ) )
    {
      reportError( "failed to generate code for function call '%s' parameters", mCallTypeDescription.mIdentifier.c_str() );
      return false;
    }

    parametersSourceDescription = "(" + parameters->getChildrenSourceDescription() + ")";
  }
  else
    parametersSourceDescription = "()";

  ScriptTypes::ScriptType objectType;
  unsigned int offset;
  ScriptTypes::ScriptTypeDescription returnType;
  crunch::core::String parentContextName;
  unsigned int paramsStackSize;

  if ( mCompiler->findFunctionInfo( mParentContextNode, mFunctionSignature, &objectType, &offset, &returnType, &parentContextName, &paramsStackSize, nullptr ) )
  {
    switch ( objectType )
    {
      case ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS:
      {
        const crunch::core::String *funcSig = mCompiler->stringTableMergeString( mFunctionSignature.c_str(), false );

        if ( parentContextName == CRUNCH_LIB_GLOBAL_CLASS_NAME )
        {
          const ScriptLibraryClass *slc = ScriptLibrary::getGlobalLibraryClass();

          const ScriptLibraryFunction *f = slc->findFunction( mFunctionSignature );
          if ( f == nullptr )
          {
            reportInternalError( "failed to find global library function '%s'", mFunctionSignature.c_str() );
            return false;
          }

          if ( !( f->nocallcode() ) )
          {
            if ( !( ScriptInstruction::write_LIB_GFUNC_CALL( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), funcSig, mCallTypeDescription.mIdentifier + parametersSourceDescription ) ) )
              return false;
          }
        }
        else
        {
          const crunch::core::String *className = mCompiler->stringTableMergeString( parentContextName.c_str(), false );
          if ( className == nullptr )
          {
            reportInternalError( "failed to merge string '%s'", parentContextName.c_str() );
            return false;
          }

          const ScriptLibraryClass *slc = ScriptLibrary::findLibraryClass( className->c_str() );
          if ( slc == nullptr )
          {
            reportInternalError( "failed to find library class '%s'", className->c_str() );
            return false;
          }

          const ScriptLibraryFunction *f = slc->findFunction( mFunctionSignature );
          if ( f == nullptr )
          {
            reportInternalError( "failed to find library function '%s' in class '%s'", mFunctionSignature.c_str(), className->c_str() );
            return false;
          }

          if ( !( f->nocallcode() ) )
          {
            if ( !( ScriptInstruction::write_LIB_FUNC_CALL( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), className, funcSig, mParentContextNode->getSourceDescription() + "." + mCallTypeDescription.mIdentifier + parametersSourceDescription ) ) )
              return false;
          }
        }
      }
      break;

      case ScriptTypes::ScriptType::TYPE_SCRIPT_OBJECT:
      {
        if ( parentContextName == CRUNCH_GLOBAL_SCRIPT_OBJECT_NAME )
        {
          if ( !( ScriptInstruction::write_SO_GFUNC_CALL( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), offset, uint64_t( paramsStackSize ), crunch::core::String::fmt( "call script function '%s'", mFunctionSignature.c_str() ) ) ) )
            return false;

          ScriptObject *so = (ScriptObject*)( mCompiler->getGlobalObject() );
          if ( so == nullptr )
            return false;

          mCompiler->reportDependencies( so, mFunctionSignature );
        }
        else
        {
          reportInternalError( "script object member functions not currently supported, parent context: '%s'", parentContextName.c_str() );
          return false;
        }
      }
      break;

      default:
        reportError( "unknown function location '%s'", ScriptTypes::getScriptTypeName( objectType ).c_str() );
        return false;
    }

    return true;
  }

  if ( mParentContextNode != nullptr )
    reportError( "member function '%s' of '%s' not found", mFunctionSignature.c_str(), mParentContextNode->getScriptTypeDescription().getTypeDescription().c_str() );
  else
    reportError( "global function '%s' not found", mFunctionSignature.c_str() );

  return false;
}

} }
