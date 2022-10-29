
#include "../../crunch_global.h"

#include "../crunch_script_compiler.h"
#include "../crunch_script_instruction.h"
#include "../crunch_script_library_class.h"
#include "../crunch_script_library.h"

#include "crunch_syntax_node_expression.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

SyntaxNodeExpression::SyntaxNodeExpression( ScriptCompiler *compiler, 
                                            SyntaxNodeRoot *root, 
                                            unsigned int lineNumber, 
                                            SyntaxNode *node, 
                                            Operator op 
                                            ) :
  SyntaxNode( compiler, root, lineNumber ),
  mOperator( op )
{
  append( node, false );
}

SyntaxNodeExpression::~SyntaxNodeExpression()
{
  // empty
}

crunch::core::String SyntaxNodeExpression::getSyntaxTreeDescription( const crunch::core::String& indent ) const
{
  crunch::core::String d = crunch::core::String::fmt( "%s%u: EXPRESSION %s (statement context: %s) (children: %u)\n", indent.c_str(), mLineNumber, getOperatorName( mOperator ).c_str(), mStatementContext ? "true" : "false", (unsigned int)( mChildren.size() ) );
  d += getSyntaxTreeChildrenDescription( indent + mIndentStep );
  return d;
}

crunch::core::String SyntaxNodeExpression::getSourceDescription() const
{
  if ( mChildren.size() > 0 && mChildren[0] != nullptr )
  {
    crunch::core::String prefix;

    switch ( mOperator )
    {
      case Operator::OPERATOR_MINUS:
        prefix = "-";
      break;

      case Operator::OPERATOR_BOOL_NOT:
        prefix = "!";
      break;

      case Operator::OPERATOR_BIT_COMP:
        prefix = "~";
      break;

      default:
        // empty
      break;
    }

    return prefix + mChildren[0]->getSourceDescription();
  }

  return "";
}

ScriptTypes::ScriptTypeDescription SyntaxNodeExpression::getScriptTypeDescription() const
{
  if ( !( mChildren.empty() ) && mChildren[0] != nullptr )
    return mChildren[0]->getScriptTypeDescription();
  return ScriptTypes::ScriptTypeDescription();
}

ScriptValue SyntaxNodeExpression::getScriptValue() const
{
  if ( !( mChildren.empty() ) && mChildren[0] != nullptr )
  {
    switch ( mOperator )
    {
      case Operator::OPERATOR_MINUS:
        return mChildren[0]->getScriptValue().negate();

      case Operator::OPERATOR_BOOL_NOT:
        return mChildren[0]->getScriptValue().boolNot();

      case Operator::OPERATOR_BIT_COMP:
        return mChildren[0]->getScriptValue().bitwiseComplement();

      default:
        return mChildren[0]->getScriptValue();
    }
  }

  return ScriptValue();
}

void SyntaxNodeExpression::setParentContextNode( SyntaxNode *node )
{
  if ( node != nullptr && !( mChildren.empty() ) && mChildren[0] != nullptr )
    mChildren[0]->setParentContextNode( node );
}

void SyntaxNodeExpression::applyConstantFolding()
{
  if ( mChildren.size() != 1 || mChildren[0] == nullptr || mChildren[0]->getType() == NodeType::NODE_LITERAL_TYPE )
    return;

  ScriptValue v = mChildren[0]->getScriptValue();

  if ( v.isConstantFoldType() )
  {
    unsigned int lineNumber = mChildren[0]->getSourceLineNumber();

    delete mChildren[0];

    mChildren[0] = new SyntaxNodeLiteralType( mCompiler, mRoot, lineNumber, v );
  }
  else
    mChildren[0]->applyConstantFolding();
}

bool SyntaxNodeExpression::generateCode( crunch::core::BinaryBuffer *dest )
{
  if ( mChildren.size() != 1 )
  {
    reportInternalError( "unexpected number of children on expression node" );
    return false;
  }
  
  if ( mChildren[0] == nullptr )
  {
    reportInternalError( "expression node child nullptr" );
    return false;
  }

  if ( mStatementContext )
    mCompiler->getContextStack()->pushContext( ScriptContextStack::Context::FETCH );

  bool retVal = mChildren[0]->generateCode( dest );

  if ( mStatementContext )
    mCompiler->getContextStack()->popContext();

  ScriptTypes::ScriptTypeDescription typeDescription = getScriptTypeDescription();

  switch ( mOperator )
  {
    case Operator::OPERATOR_MINUS:
      switch ( typeDescription.mScriptType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          if ( !( ScriptInstruction::write_MINUS_I64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) )
            retVal = false;
        break;

        case ScriptTypes::ScriptType::TYPE_F64:
          if ( !( ScriptInstruction::write_MINUS_F64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) )
            retVal = false;
        break;

        case ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS:
        {
          const ScriptLibraryClass *slc = ScriptLibrary::getGlobalLibraryClass();

          if ( slc != nullptr )
          {
            crunch::core::String operatorSignature = crunch::core::String::fmt( "%s_operator_minus(%s)", typeDescription.mIdentifier.c_str(), typeDescription.getTypeDescription().c_str() );

            if ( slc->hasFunctionSignature( operatorSignature ) )
            {
              const crunch::core::String *funcSig = mCompiler->stringTableMergeString( operatorSignature.c_str(), false );

              if ( !( ScriptInstruction::write_LIB_GFUNC_CALL( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), funcSig, "library operator unary minus" ) ) )
                retVal = false;
            }
            else
              reportError( "library class '%s' does not define unary minus operator", typeDescription.mIdentifier.c_str() );
          }
          else
            reportInternalError( "missing global library class" );
        }
        break;

        default:
          reportError( "type '%s' not supported for unary minus", typeDescription.getTypeDescription().c_str() );
        break;
      }
    break;

    case Operator::OPERATOR_BOOL_NOT:
      switch ( typeDescription.mScriptType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          if ( !( ScriptInstruction::write_BOOL_NOT_I64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) )
            retVal = false;
        break;

        default:
          reportError( "type '%s' not supported for bool not", typeDescription.getTypeDescription().c_str() );
        break;
      }
    break;

    case Operator::OPERATOR_BIT_COMP:
      switch ( typeDescription.mScriptType )
      {
        case ScriptTypes::ScriptType::TYPE_I64:
          if ( !( ScriptInstruction::write_BIT_COMP_I64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) )
            retVal = false;
        break;

        case ScriptTypes::ScriptType::TYPE_U64:
          if ( !( ScriptInstruction::write_BIT_COMP_U64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) )
            retVal = false;
        break;

        default:
          reportError( "type '%s' not supported for bitwise complement", typeDescription.getTypeDescription().c_str() );
        break;
      }
    break;

    default:
      // empty
    break;
  }

  return retVal;
}

bool SyntaxNodeExpression::isLValue() const
{
  if ( !( mChildren.empty() ) && mChildren[0] != nullptr )
    return mChildren[0]->isLValue();
  return false;
}

crunch::core::String SyntaxNodeExpression::getOperatorName( Operator op )
{
  switch ( op )
  {
    case Operator::OPERATOR_MINUS:
      return "OPERATOR_MINUS";

    case Operator::OPERATOR_BOOL_NOT:
      return "OPERATOR_BOOL_NOT";

    case Operator::OPERATOR_BIT_COMP:
      return "OPERATOR_BIT_COMP";

    default:
      // empty
    break;
  }

  return "OPERATOR_NONE";
}

SyntaxNodeExpression::Operator SyntaxNodeExpression::getOperator( const crunch::core::String& name )
{
  crunch::core::String findMe = name;
  findMe.toUpper();

  if ( findMe == "OPERATOR_MINUS" )
    return Operator::OPERATOR_MINUS;

  if ( findMe == "OPERATOR_BOOL_NOT" )
    return Operator::OPERATOR_BOOL_NOT;

  if ( findMe == "OPERATOR_BIT_COMP" )
    return Operator::OPERATOR_BIT_COMP;

  return Operator::OPERATOR_NONE;
}

} }
