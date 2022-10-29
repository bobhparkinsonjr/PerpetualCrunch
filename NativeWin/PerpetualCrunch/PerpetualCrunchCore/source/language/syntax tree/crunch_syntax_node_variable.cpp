
#include "../../crunch_global.h"

#include "../crunch_script_compiler.h"
#include "../crunch_symbol_stack.h"
#include "../crunch_script_types.h"
#include "../crunch_script_instruction.h"
#include "../crunch_script_library.h"

#include "crunch_syntax_node_variable.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

SyntaxNodeVariable::SyntaxNodeVariable( ScriptCompiler *compiler, 
                                        SyntaxNodeRoot *root, 
                                        unsigned int lineNumber, 
                                        const crunch::core::String& identifier 
                                        ) :
  SyntaxNode( compiler, root, lineNumber ),
  mIdentifier( identifier )
{
  if ( mIdentifier == "" )
    reportInternalError( "variable node missing identifier" );
}

SyntaxNodeVariable::SyntaxNodeVariable( ScriptCompiler *compiler, 
                                        SyntaxNodeRoot *root, 
                                        unsigned int lineNumber, 
                                        SyntaxNode *parentExpression, 
                                        SyntaxNode *subExpression,
                                        SubType st
                                        ) :
  SyntaxNode( compiler, root, lineNumber )
{
  if ( parentExpression == nullptr )
    reportInternalError( "variable node missing parent expression" );

  if ( subExpression == nullptr )
    reportInternalError( "variable node missing sub expression" );

  append( parentExpression );
  append( subExpression );

  mSubType = st;

  switch ( mSubType )
  {
    case SubType::SUB_TYPE_MEMBER:
      subExpression->setParentContextNode( parentExpression );
    break;

    default:
      // empty
    break;
  }
}

SyntaxNodeVariable::~SyntaxNodeVariable()
{
  // empty
}

crunch::core::String SyntaxNodeVariable::getSyntaxTreeDescription( const crunch::core::String& indent ) const
{
  crunch::core::String d = crunch::core::String::fmt( "%s%u: VARIABLE %s (sub:%s) (children: %u)\n", indent.c_str(), mLineNumber, mIdentifier.c_str(), getSubTypeName( mSubType ).c_str(), (unsigned int)( mChildren.size() ) );
  d += getSyntaxTreeChildrenDescription( indent + mIndentStep );
  return d;
}

crunch::core::String SyntaxNodeVariable::getSourceDescription() const
{
  if ( mIdentifier == "" )
  {
    if ( mChildren.size() == 2 && mChildren[0] != nullptr && mChildren[1] != nullptr )
    {
      switch ( mSubType )
      {
        case SubType::SUB_TYPE_MEMBER:
          return mChildren[0]->getSourceDescription() + "." + mChildren[1]->getSourceDescription();

        case SubType::SUB_TYPE_SUBSCRIPT:
          return mChildren[0]->getSourceDescription() + "[" + mChildren[1]->getSourceDescription() + "]";

        default:
          // empty
        break;
      }

      return mChildren[0]->getSourceDescription() + "<internal error>" + mChildren[1]->getSourceDescription();
    }
  }

  return mIdentifier;
}

void SyntaxNodeVariable::setParentContextNode( SyntaxNode *node )
{
  mParentContextNode = node;
}

bool SyntaxNodeVariable::generateCode( crunch::core::BinaryBuffer *dest )
{
  switch ( mCompiler->getContextStack()->getContext() )
  {
    case ScriptContextStack::Context::FETCH:
    {
      if ( mIdentifier == "" )
      {
        if ( mChildren.size() < 2 )
          return false;

        // this can be a library class member function call (eg. "a.foo( b )")
        // could be a local access (eg. "a.z")

        SyntaxNode *parent = (SyntaxNode*)( mChildren[0] );

        if ( parent == nullptr )
        {
          reportError( "invalid parent expression for variable" );
          return false;
        }

        switch ( mSubType )
        {
          case SubType::SUB_TYPE_MEMBER:
          {
            SyntaxNodeFunctionCall *memberFunctionCall = (SyntaxNodeFunctionCall*)( mChildren[1]->matchTree2( NodeType::NODE_EXPRESSION, NodeType::NODE_FUNCTION_CALL, true ) );
            if ( memberFunctionCall != nullptr )
              return generateMemberFetchCode( dest, parent, memberFunctionCall );
            return generateMemberFetchCode( dest, parent, (SyntaxNodeVariable*)( mChildren[1]->matchTree2( NodeType::NODE_EXPRESSION, NodeType::NODE_VARIABLE ) ) );
          }

          case SubType::SUB_TYPE_SUBSCRIPT:
            return generateSubscriptFetchCode( dest, parent, mChildren[1] );

          default:
            // empty
          break;
        }

        reportInternalError( "unknown sub type for variable" );
        return false;
      }
      else
      if ( mParentContextNode != nullptr )
      {
        // TODO
      }
      else
        return generateFetchCode( dest );
    }
    break;

    case ScriptContextStack::Context::STORE:
    {
      // only allowed to access local variables (function parameters and variables in current function)
      // not allowed to modify anything in the library (class, function, property, enum), we are allowed to assign a library class to another library class tho

      if ( mIdentifier == "" )
      {
        // find the variable from the left and right, req they both contain an identifier

        // parent/member should be: SyntaxNodeExpression -> SyntaxNodeVariable -> identifier

        SyntaxNodeVariable *parent = (SyntaxNodeVariable*)( mChildren[0]->matchTree2( NodeType::NODE_EXPRESSION, NodeType::NODE_VARIABLE ) );
        if ( parent == nullptr || parent->mIdentifier == "" )
        {
          reportError( "invalid parent expression for lhs variable" );
          return false;
        }

        switch ( mSubType )
        {
          case SubType::SUB_TYPE_MEMBER:
          {
            SyntaxNodeVariable *member = (SyntaxNodeVariable*)( mChildren[1]->matchTree2( NodeType::NODE_EXPRESSION, NodeType::NODE_VARIABLE ) );

            if ( member == nullptr || member->mIdentifier == "" )
            {
              reportError( "invalid member expression for lhs variable" );
              return false;
            }

            if ( !( generateStoreCode( dest, parent, member ) ) )
              return false;
          }
          break;

          case SubType::SUB_TYPE_SUBSCRIPT:
          {
            if ( !( generateSubscriptStoreCode( dest, parent, mChildren[1] ) ) )
              return false;
          }
          break;

          default:
            reportInternalError( "unknown variable sub type" );
          break;
        }
      }
      else
      if ( mParentContextNode != nullptr )
      {
        // TODO
      }
      else
      {
        if ( !( generateStoreCode( dest ) ) )
          return false;
      }
    }
    break;
    
    case ScriptContextStack::Context::NONE:
    default:
    {
      crunch::core::String c0;
      if ( mChildren.size() > 0 && mChildren[0] != nullptr )
        c0 = mChildren[0]->getSyntaxTreeDescription( "  " );

      crunch::core::String c1;
      if ( mChildren.size() > 1 && mChildren[1] != nullptr )
        c1 = mChildren[1]->getSyntaxTreeDescription( "  " );

      reportInternalError( "missing context for variable node, identifier: '%s'\nchild 0:\n%s\nchild 1:\n%s", mIdentifier.c_str(), c0.c_str(), c1.c_str() );
      return false;
    }
  }

  return true;
}

ScriptTypes::ScriptTypeDescription SyntaxNodeVariable::getScriptTypeDescription() const
{
  if ( mIdentifier == "" )
  {
    if ( mChildren.size() < 2 || mChildren[1] == nullptr )
      return ScriptTypes::ScriptTypeDescription();

    SyntaxNodeFunctionCall *memberFunctionCall = (SyntaxNodeFunctionCall*)( mChildren[1]->matchTree2( NodeType::NODE_EXPRESSION, NodeType::NODE_FUNCTION_CALL, true ) );

    if ( memberFunctionCall != nullptr )
      return mChildren[1]->getScriptTypeDescription();

    SyntaxNode *parent = (SyntaxNode*)( mChildren[0]->getChild( 0 ) );
    if ( parent == nullptr )
      return ScriptTypes::ScriptTypeDescription();

    switch ( mSubType )
    {
      case SubType::SUB_TYPE_MEMBER:
      {
        SyntaxNodeVariable *member = (SyntaxNodeVariable*)( mChildren[1]->matchTree2( NodeType::NODE_EXPRESSION, NodeType::NODE_VARIABLE ) );
        if ( member == nullptr )
          return ScriptTypes::ScriptTypeDescription();

        ScriptTypes::ScriptTypeDescription parentScriptTypeDescription = parent->getScriptTypeDescription();

        if ( parentScriptTypeDescription.mScriptType == ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS )
        {
          const ScriptLibraryClass *slc = ScriptLibrary::findLibraryClass( parentScriptTypeDescription.mIdentifier.c_str() );

          ScriptTypes::ScriptType memberScriptType;
          crunch::core::String memberTypeIdentifier;
          unsigned int memberOffset;
          unsigned int memberSize;

          if ( !( slc->getMemberInfo( member->mIdentifier, &memberScriptType, &memberTypeIdentifier, &memberOffset, &memberSize ) ) )
            return ScriptTypes::ScriptTypeDescription();
      
          return ScriptTypes::ScriptTypeDescription( memberScriptType, memberTypeIdentifier );
        }
      }
      break;

      case SubType::SUB_TYPE_SUBSCRIPT:
      {
        ScriptTypes::ScriptTypeDescription parentType = parent->getScriptTypeDescription();

        if ( parentType.mScriptType == ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS )
        {
          const ScriptLibraryClass *slc = ScriptLibrary::findLibraryClass( parentType.mIdentifier.c_str() );

          if ( slc != nullptr )
          {
            ScriptTypes::ScriptTypeDescription baseType = slc->getBaseType();

            if ( baseType.mScriptType != ScriptTypes::ScriptType::TYPE_NONE )
              return baseType;
          }
        }
      }
      break;

      default:
        // empty
      break;
    }

    return ScriptTypes::ScriptTypeDescription();
  }
  else
  if ( mParentContextNode != nullptr )
  {
    // TODO
  }
  else
  {
    unsigned int dataOffset = 0xFF;
    SyntaxNode *identifierNode;
    SymbolStack::SymbolLocation symbolLocation;

    if ( !( mCompiler->getSymbolStack()->findSymbol( mIdentifier, &dataOffset, &identifierNode, false, &symbolLocation ) ) )
    {
      reportError( "symbol '%s' not found", mIdentifier.c_str() );
      return ScriptTypes::ScriptTypeDescription();
    }

    if ( identifierNode == nullptr )
    {
      reportInternalError( "symbol '%s' missing type node", mIdentifier.c_str() );
      return ScriptTypes::ScriptTypeDescription();
    }

    return identifierNode->getScriptTypeDescription();
  }

  return ScriptTypes::ScriptTypeDescription();
}

ScriptValue SyntaxNodeVariable::getScriptValue() const
{
  if ( mIdentifier != "" )
  {
    unsigned int offset;
    SyntaxNode *node;
    SymbolStack::SymbolLocation symbolLocation;

    if ( !( mCompiler->getSymbolStack()->findSymbol( mIdentifier, &offset, &node, false, &symbolLocation ) ) )
      return ScriptValue();
    
    if ( node != nullptr && symbolLocation == SymbolStack::SymbolLocation::SYMBOL_LOCATION_CONSTANT )
      return node->getScriptValue();
  }

  return ScriptValue();
}

crunch::core::String SyntaxNodeVariable::getSubTypeName( SubType st )
{
  switch ( st )
  {
    case SubType::SUB_TYPE_MEMBER:
      return "SUB_TYPE_MEMBER";

    case SubType::SUB_TYPE_SUBSCRIPT:
      return "SUB_TYPE_SUBSCRIPT";

    default:
      // empty
    break;
  }

  return "";
}

SyntaxNodeVariable::SubType SyntaxNodeVariable::getSubType( const crunch::core::String& n )
{
  crunch::core::String findMe = n;
  findMe.toUpper();

  if ( findMe == "SUB_TYPE_MEMBER" )
    return SubType::SUB_TYPE_MEMBER;

  if ( findMe == "SUB_TYPE_SUBSCRIPT" )
    return SubType::SUB_TYPE_SUBSCRIPT;

  return SubType::SUB_TYPE_NONE;
}

bool SyntaxNodeVariable::generateMemberFetchCode( crunch::core::BinaryBuffer *dest, SyntaxNode *parent, SyntaxNodeFunctionCall *memberFunctionCall )
{
  // eg. a.get()

  if ( parent == nullptr || memberFunctionCall == nullptr )
  {
    reportInternalError( "expected parent and member function call" );
    return false;
  }

  ScriptTypes::ScriptTypeDescription parentScriptTypeDescription = parent->getScriptTypeDescription();

  switch ( parentScriptTypeDescription.mScriptType )
  {
    case ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS:
    {
      if ( !( parent->generateCode( dest ) ) )
        return false;

      if ( !( memberFunctionCall->generateCode( dest ) ) )
        return false;
    }
    break;

    default:
      reportError( "invalid type '%s' for parent", ScriptTypes::getScriptTypeName( parentScriptTypeDescription.mScriptType ).c_str() );
      return false;
  }

  return true;
}

bool SyntaxNodeVariable::generateMemberFetchCode( crunch::core::BinaryBuffer *dest, SyntaxNode *parent, SyntaxNodeVariable *member )
{
  if ( parent == nullptr )
  {
    reportInternalError( "expected parent variable" );
    return false;
  }

  if ( member == nullptr )
  {
    reportInternalError( "expected member variable" );
    return false;
  }

  if ( member->mIdentifier == "" )
  {
    reportError( "invalid member expression for variable" );
    return true;
  }

  ScriptTypes::ScriptTypeDescription parentScriptTypeDescription = parent->getScriptTypeDescription();

  if ( parentScriptTypeDescription.mScriptType == ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS )
    return generateLibraryClassMemberFetchCode( dest, parentScriptTypeDescription, parent, member );

  reportError( "no built-in types support member variables" );
  return true;
}

bool SyntaxNodeVariable::generateLibraryClassMemberFetchCode( crunch::core::BinaryBuffer *dest, 
                                                              const ScriptTypes::ScriptTypeDescription& parentScriptTypeDescription,
                                                              SyntaxNode *parent, 
                                                              SyntaxNodeVariable *member 
                                                              )
{
  const ScriptLibraryClass *slc = ScriptLibrary::findLibraryClass( parentScriptTypeDescription.mIdentifier.c_str() );

  if ( slc == nullptr )
  {
    reportError( "library class '%s' not found", parentScriptTypeDescription.mIdentifier.c_str() );
    return true;
  }

  ScriptTypes::ScriptType memberScriptType;
  crunch::core::String memberTypeIdentifier;
  unsigned int memberOffset;
  unsigned int memberSize;

  if ( !( slc->getMemberInfo( member->mIdentifier, &memberScriptType, &memberTypeIdentifier, &memberOffset, &memberSize ) ) )
  {
    reportError( "library class '%s' doesn't have member '%s'", parentScriptTypeDescription.mIdentifier.c_str(), member->mIdentifier.c_str() );
    return true;
  }

  crunch::core::String comment = crunch::core::String::fmt( "%s.%s, fetch library class member", parent->getSourceDescription().c_str(), member->mIdentifier.c_str() );

  switch ( parentScriptTypeDescription.mScriptType )
  {
    case ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS:
    {
      if ( !( parent->generateCode( dest ) ) )
        return false;

      if ( !( ScriptInstruction::update_PUSH_FETCH_LOCAL_member( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), (uint32_t)( memberOffset ), (uint64_t)( memberSize ) ) ) )
      {
        if ( !( ScriptInstruction::write_FETCH_TEMP_MEMBER( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), (uint32_t)( slc->getDataStackSize() ), (uint32_t)( memberSize ), (uint32_t)( memberOffset ), comment ) ) )
          return false;
      }
    }
    break;

    default:
      reportError( "unknown parent variable type for member access" );
      return false;
  }

  return true;
}

bool SyntaxNodeVariable::generateFetchCode( crunch::core::BinaryBuffer *dest )
{
  unsigned int offset;
  SyntaxNode *node;
  SymbolStack::SymbolLocation symbolLocation;

  if ( !( mCompiler->getSymbolStack()->findSymbol( mIdentifier, &offset, &node, false, &symbolLocation ) ) )
  {
    reportError( "symbol '%s' not found", mIdentifier.c_str() );
    return false;
  }
  
  switch ( symbolLocation )
  {
    case SymbolStack::SymbolLocation::SYMBOL_LOCATION_DATA_STACK:
    {
      SyntaxNodeTypeIdentifier *typeIdentifier = (SyntaxNodeTypeIdentifier*)( node );

      unsigned int dataSize = typeIdentifier->getDataStackSize();

      if ( !( ScriptInstruction::write_PUSH_FETCH_LOCAL( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), (uint32_t)( offset ), (uint64_t)( dataSize ), mIdentifier ) ) )
        return false;
    }
    break;

    case SymbolStack::SymbolLocation::SYMBOL_LOCATION_GLOBAL_LIBRARY_CLASS_INSTANCE:
    {
      SyntaxNodeTypeIdentifier *typeIdentifier = (SyntaxNodeTypeIdentifier*)( node );

      const crunch::core::String *className = mCompiler->stringTableMergeString( typeIdentifier->getIdentifier().c_str(), false );
      const crunch::core::String *instanceName = mCompiler->stringTableMergeString( mIdentifier.c_str(), false );

      if ( !( ScriptInstruction::write_PUSH_FETCH_GLIB_CLS( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), className, instanceName, mIdentifier ) ) )
        return false;
    }
    break;

    case SymbolStack::SymbolLocation::SYMBOL_LOCATION_CONSTANT:
    {
      SyntaxNodeLiteralType *literalNode = (SyntaxNodeLiteralType*)( node );

      switch ( literalNode->getScriptType() )
      {
        case ScriptTypes::ScriptType::TYPE_STRING:
          if ( !( ScriptInstruction::write_PUSH_STRING( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), literalNode->getValueString(), mIdentifier ) ) )
            return false;
        break;

        case ScriptTypes::ScriptType::TYPE_I64:
          if ( !( ScriptInstruction::write_PUSH_I64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), literalNode->getValueI64(), mIdentifier ) ) )
            return false;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          if ( !( ScriptInstruction::write_PUSH_U64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), literalNode->getValueU64(), mIdentifier ) ) )
            return false;
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          if ( !( ScriptInstruction::write_PUSH_F64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), literalNode->getValueF64(), mIdentifier ) ) )
            return false;
        break;

        default:
          reportInternalError( "unsupported type for constant symbol" );
          return false;
      }
    }
    break;

    default:
      reportInternalError( "unknown symbol location type for '%s'", mIdentifier.c_str() );
      return false;
  }

  return true;
}

bool SyntaxNodeVariable::generateSubscriptFetchCode( crunch::core::BinaryBuffer *dest, SyntaxNode *parent, SyntaxNode *subscript )
{
  if ( parent == nullptr || subscript == nullptr )
  {
    reportInternalError( "variable missing parent and/or subscript node for fetch" );
    return false;
  }

  ScriptTypes::ScriptTypeDescription parentType = parent->getScriptTypeDescription();
  ScriptTypes::ScriptTypeDescription subscriptType = subscript->getScriptTypeDescription();

  if ( parentType.mScriptType != ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS )
  {
    reportError( "only library classes support the [] operator" );
    return true;
  }

  if ( subscriptType.mScriptType != ScriptTypes::ScriptType::TYPE_I64 )
  {
    reportError( "subscript type must be i64" );
    return true;
  }

  if ( !( parent->generateCode( dest ) ) )
    return false;

  if ( !( subscript->generateCode( dest ) ) )
    return false;

  crunch::core::String classIdentifier = parentType.mIdentifier;

  const ScriptLibraryClass *slc = ScriptLibrary::getGlobalLibraryClass();

  if ( slc == nullptr )
  {
    reportInternalError( "missing the global library class" );
    return false;
  }

  crunch::core::String operatorSignature = crunch::core::String::fmt( "%s_operator_get_index(_library_class %s,_i64)", classIdentifier.c_str(), classIdentifier.c_str() );

  if ( slc->hasFunctionSignature( operatorSignature ) )
  {
    const crunch::core::String *funcSig = mCompiler->stringTableMergeString( operatorSignature.c_str(), false );
    return ScriptInstruction::write_LIB_GFUNC_CALL( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), funcSig, "library operator [] for fetch" );
  }

  reportError( "library class '%s' does not define operator [] for fetch", classIdentifier.c_str() );
  return true;
}

bool SyntaxNodeVariable::generateStoreCode( crunch::core::BinaryBuffer *dest, SyntaxNodeVariable *parent, SyntaxNodeVariable *member )
{
  unsigned int parentOffset;
  SyntaxNode *parentNode;
  SymbolStack::SymbolLocation symbolLocation;

  if ( !( mCompiler->getSymbolStack()->findSymbol( parent->mIdentifier, &parentOffset, &parentNode, false, &symbolLocation ) ) )
  {
    reportError( "symbol '%s' not found", parent->mIdentifier.c_str() );
    return true;
  }

  if ( parentNode == nullptr )
  {
    reportInternalError( "missing type identifier node for symbol '%s'", parent->mIdentifier.c_str() );
    return false;
  }

  if ( parentNode->getScriptType() == ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS )
    return generateLibraryClassMemberStoreCode( dest, parentNode, parentOffset, parent, member );

  if ( symbolLocation != SymbolStack::SymbolLocation::SYMBOL_LOCATION_DATA_STACK )
  {
    reportError( "cannot assign to members that don't reside on the data stack" );
    return false;
  }

  if ( parentNode->getScriptType() == ScriptTypes::ScriptType::TYPE_NONE )
  {
    reportError( "lhs parent expression must be a script type" );
    return false;
  }

  unsigned int memberOffset;
  unsigned int memberSize;

  if ( !( ScriptTypes::getMemberInfo( parentNode->getScriptType(), member->mIdentifier, &memberOffset, &memberSize ) ) )
  {
    reportError( "script type '%s' has no member '%s'", ScriptTypes::getScriptTypeName( parentNode->getScriptType() ).c_str(), member->mIdentifier.c_str() );
    return false;
  }

  crunch::core::String comment = parent->getSourceDescription() + "." + member->mIdentifier;

  unsigned int dataOffset = parentOffset + memberOffset;

  if ( !( ScriptInstruction::write_POP_STORE_LOCAL( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), (uint32_t)( dataOffset ), (uint64_t)( memberSize ), comment ) ) )
    return false;

  return true;
}

bool SyntaxNodeVariable::generateLibraryClassMemberStoreCode( crunch::core::BinaryBuffer *dest, 
                                                              SyntaxNode *parentNode, 
                                                              unsigned int parentOffset, 
                                                              SyntaxNodeVariable *parent, 
                                                              SyntaxNodeVariable *member 
                                                              )
{
  if ( parentNode->getType() != NodeType::NODE_TYPE_IDENTIFIER )
  {
    reportInternalError( "unexpected node for library class symbol on symbol stack" );
    return false;
  }

  SyntaxNodeTypeIdentifier *parentTypeIdentifier = (SyntaxNodeTypeIdentifier*)( parentNode );

  const ScriptLibraryClass *slc = ScriptLibrary::findLibraryClass( parentTypeIdentifier->getIdentifier().c_str() );

  if ( slc == nullptr )
  {
    reportError( "library class '%s' not found", parentTypeIdentifier->getIdentifier().c_str() );
    return true;
  }

  ScriptTypes::ScriptType memberScriptType;
  crunch::core::String memberTypeIdentifier;
  unsigned int memberOffset;
  unsigned int memberSize;

  if ( !( slc->getMemberInfo( member->mIdentifier, &memberScriptType, &memberTypeIdentifier, &memberOffset, &memberSize ) ) )
  {
    reportError( "library class '%s' doesn't have member '%s'", parentTypeIdentifier->getIdentifier().c_str(), member->mIdentifier.c_str() );
    return true;
  }

  crunch::core::String comment = crunch::core::String::fmt( "%s.%s, store library class member", parent->getSourceDescription().c_str(), member->mIdentifier.c_str() );

  unsigned int dataOffset = parentOffset + memberOffset;

  if ( !( ScriptInstruction::write_POP_STORE_LOCAL( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), (uint32_t)( dataOffset ), (uint64_t)( memberSize ), comment ) ) )
    return false;

  return true;
}

bool SyntaxNodeVariable::generateStoreCode( crunch::core::BinaryBuffer *dest )
{
  unsigned int dataOffset = 0xFF;

  SyntaxNode *identifierNode;
  SymbolStack::SymbolLocation symbolLocation;

  if ( !( mCompiler->getSymbolStack()->findSymbol( mIdentifier, &dataOffset, &identifierNode, false, &symbolLocation ) ) )
  {
    reportError( "symbol '%s' not found", mIdentifier.c_str() );
    return false;
  }

  if ( identifierNode == nullptr )
  {
    reportInternalError( "missing type identifier node for symbol '%s'", mIdentifier.c_str() );
    return false;
  }

  if ( symbolLocation != SymbolStack::SymbolLocation::SYMBOL_LOCATION_DATA_STACK )
  {
    reportError( "cannot assign to variables that don't reside on the data stack" );
    return false;
  }

  if ( identifierNode->getType() != NodeType::NODE_TYPE_IDENTIFIER )
  {
    reportInternalError( "expected type identifier node for symbol" );
    return false;
  }

  SyntaxNodeTypeIdentifier *typeIdentifier = (SyntaxNodeTypeIdentifier*)( identifierNode );

  unsigned int dataSize = typeIdentifier->getDataStackSize();

  if ( !( ScriptInstruction::write_POP_STORE_LOCAL( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), (uint32_t)( dataOffset ), (uint64_t)( dataSize ), mIdentifier ) ) )
    return false;

  return true;
}

bool SyntaxNodeVariable::generateSubscriptStoreCode( crunch::core::BinaryBuffer *dest, SyntaxNode *parent, SyntaxNode *subscript )
{
  if ( parent == nullptr || subscript == nullptr )
  {
    reportInternalError( "variable missing parent and/or subscript node for store" );
    return false;
  }

  ScriptTypes::ScriptTypeDescription parentType = parent->getScriptTypeDescription();
  ScriptTypes::ScriptTypeDescription subscriptType = subscript->getScriptTypeDescription();

  if ( parentType.mScriptType != ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS )
  {
    reportError( "only library classes support the [] operator" );
    return true;
  }

  if ( subscriptType.mScriptType != ScriptTypes::ScriptType::TYPE_I64 )
  {
    reportError( "subscript type must be i64" );
    return true;
  }

  if ( !( parent->generateCode( dest ) ) )
    return false;

  if ( !( subscript->generateCode( dest ) ) )
    return false;

  crunch::core::String classIdentifier = parentType.mIdentifier;

  const ScriptLibraryClass *slc = ScriptLibrary::getGlobalLibraryClass();

  if ( slc == nullptr )
  {
    reportInternalError( "missing global library class" );
    return false;
  }

  crunch::core::String operatorSignature = crunch::core::String::fmt( "%s_operator_set_index(_library_class %s,_i64)", classIdentifier.c_str(), classIdentifier.c_str() );

  if ( slc->hasFunctionSignature( operatorSignature ) )
  {
    const crunch::core::String *funcSig = mCompiler->stringTableMergeString( operatorSignature.c_str(), false );
    return ScriptInstruction::write_LIB_GFUNC_CALL( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), funcSig, "library operator [] for store" );
  }

  reportError( "library class '%s' does not define operator [] for store", classIdentifier.c_str() );
  return true;
}

} }
