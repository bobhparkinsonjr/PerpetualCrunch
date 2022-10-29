
#include "../../crunch_global.h"

#include "../crunch_script_compiler.h"
#include "../crunch_script_instruction.h"
#include "../crunch_script_library.h"
#include "../crunch_script_library_class.h"

#include "crunch_syntax_node_binary_expression.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

SyntaxNodeBinaryExpression::SyntaxNodeBinaryExpression( ScriptCompiler *compiler, SyntaxNodeRoot *root, unsigned int lineNumber, SyntaxNode *lhs, Operator op, SyntaxNode *rhs ) :
  SyntaxNode( compiler, root, lineNumber ),
  mOperator( op )
{
  append( lhs );
  append( rhs );

  if ( lhs == nullptr )
    reportError( "binary expression missing lhs" );

  if ( rhs == nullptr )
    reportError( "binary expression missing rhs" );
}

SyntaxNodeBinaryExpression::~SyntaxNodeBinaryExpression()
{
  // empty
}

crunch::core::String SyntaxNodeBinaryExpression::getSyntaxTreeDescription( const crunch::core::String& indent ) const
{
  crunch::core::String d = crunch::core::String::fmt( "%s%u: BINARY EXPRESSION %s (children: %u)\n", indent.c_str(), mLineNumber, getOperatorName( mOperator ).c_str(), (unsigned int)( mChildren.size() ) );
  d += getSyntaxTreeChildrenDescription( indent + mIndentStep );
  return d;
}

ScriptTypes::ScriptTypeDescription SyntaxNodeBinaryExpression::getScriptTypeDescription() const
{
  switch ( mOperator )
  {
    case Operator::OPERATOR_EE:
    case Operator::OPERATOR_NE:
    case Operator::OPERATOR_GT:
    case Operator::OPERATOR_GE:
    case Operator::OPERATOR_LT:
    case Operator::OPERATOR_LE:
    case Operator::OPERATOR_BOOL_AND:
    case Operator::OPERATOR_BOOL_OR:
      return ScriptTypes::ScriptTypeDescription( ScriptTypes::ScriptType::TYPE_I64 );

    default:
      // empty
    break;
  }

  if ( !( mChildren.empty() ) && mChildren[0] != nullptr && mChildren[1] != nullptr )
  {
    ScriptTypes::ScriptTypeDescription lhsTypeDescription = mChildren[0]->getScriptTypeDescription();
    ScriptTypes::ScriptTypeDescription rhsTypeDescription = mChildren[1]->getScriptTypeDescription();
    
    if ( lhsTypeDescription.mScriptType == ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS )
      return lhsTypeDescription;

    return rhsTypeDescription;
  }

  return ScriptTypes::ScriptTypeDescription();
}

ScriptValue SyntaxNodeBinaryExpression::getScriptValue() const
{
  if ( mChildren.size() == 2 && mChildren[0] != nullptr && mChildren[1] != nullptr )
  {
    ScriptValue lhs = mChildren[0]->getScriptValue();
    ScriptValue rhs = mChildren[1]->getScriptValue();

    if ( lhs.getType() == ScriptTypes::ScriptType::TYPE_NONE || rhs.getType() == ScriptTypes::ScriptType::TYPE_NONE )
      return ScriptValue();

    switch ( mOperator )
    {
      case Operator::OPERATOR_EE: return lhs.compareEE( rhs );
      case Operator::OPERATOR_NE: return lhs.compareNE( rhs );
      case Operator::OPERATOR_GT: return lhs.compareGT( rhs );
      case Operator::OPERATOR_GE: return lhs.compareGE( rhs );
      case Operator::OPERATOR_LT: return lhs.compareLT( rhs );
      case Operator::OPERATOR_LE: return lhs.compareLE( rhs );

      case Operator::OPERATOR_PLUS: return lhs.add( rhs );
      case Operator::OPERATOR_MINUS: return lhs.subtract( rhs );
      case Operator::OPERATOR_MULTIPLY: return lhs.multiply( rhs );
      case Operator::OPERATOR_DIVIDE: return lhs.divide( rhs );

      case Operator::OPERATOR_BOOL_AND: return lhs.boolAnd( rhs );
      case Operator::OPERATOR_BOOL_OR: return lhs.boolOr( rhs );

      case Operator::OPERATOR_BIT_AND: return lhs.bitwiseAnd( rhs );
      case Operator::OPERATOR_BIT_OR: return lhs.bitwiseOr( rhs );

      default:
        // empty
      break;
    }
  }

  return ScriptValue();
}

void SyntaxNodeBinaryExpression::applyConstantFolding()
{
  if ( mChildren.size() != 2 || mChildren[0] == nullptr || mChildren[1] == nullptr )
    return;

  for ( int childIndex = 0; childIndex < 2; ++childIndex )
  {
    if ( mChildren[childIndex]->getType() == NodeType::NODE_LITERAL_TYPE )  
      continue;

    ScriptValue v = mChildren[childIndex]->getScriptValue();

    if ( v.isConstantFoldType() )
    {
      unsigned int lineNumber = mChildren[childIndex]->getSourceLineNumber();

      delete mChildren[childIndex];

      mChildren[childIndex] = new SyntaxNodeLiteralType( mCompiler, mRoot, lineNumber, v );
    }
    else
      mChildren[childIndex]->applyConstantFolding();
  }
}

bool SyntaxNodeBinaryExpression::generateCode( crunch::core::BinaryBuffer *dest )
{
  if ( mChildren.size() != 2 || mChildren[0] == nullptr || mChildren[1] == nullptr )
  {
    reportInternalError( "unexpected children nodes in binary expression node" );
    return false;
  }

  ScriptTypes::ScriptTypeDescription lhsTypeDescription = mChildren[0]->getScriptTypeDescription();
  ScriptTypes::ScriptTypeDescription rhsTypeDescription = mChildren[1]->getScriptTypeDescription();

  generateChildrenCode( dest );

  ScriptTypes::ScriptTypeDescription typeMismatchLibraryClass;

  if ( lhsTypeDescription != rhsTypeDescription )
  {
    if ( lhsTypeDescription.mScriptType != ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS && rhsTypeDescription.mScriptType != ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS )
      reportError( "type mismatch in binary expression, lhs: '%s', rhs: '%s'", lhsTypeDescription.getDisplayTypeDescription().c_str(), rhsTypeDescription.getDisplayTypeDescription().c_str() );
    else
    if ( lhsTypeDescription.mScriptType == ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS )
      typeMismatchLibraryClass = lhsTypeDescription;
    else
      typeMismatchLibraryClass = rhsTypeDescription;
  }

  bool retVal = true;

  switch ( mOperator )
  {
    case Operator::OPERATOR_EE:
    {
      switch ( lhsTypeDescription.mScriptType )
      {
        case ScriptTypes::ScriptType::TYPE_STRING: if ( !( ScriptInstruction::write_EE_STRING( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_I64: if ( !( ScriptInstruction::write_EE_I64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_U64: if ( !( ScriptInstruction::write_EE_U64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_F64: if ( !( ScriptInstruction::write_EE_F64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;

        default:
          reportError( "invalid type for == comparison" );
        break;
      }
    }
    break;

    case Operator::OPERATOR_NE:
    {
      switch ( lhsTypeDescription.mScriptType )
      {
        case ScriptTypes::ScriptType::TYPE_STRING: if ( !( ScriptInstruction::write_NE_STRING( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_I64: if ( !( ScriptInstruction::write_NE_I64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_U64: if ( !( ScriptInstruction::write_NE_U64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_F64: if ( !( ScriptInstruction::write_NE_F64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;

        default:
          reportError( "invalid type for != comparison" );
        break;
      }
    }
    break;

    case Operator::OPERATOR_GT:
    {
      switch ( lhsTypeDescription.mScriptType )
      {
        case ScriptTypes::ScriptType::TYPE_STRING: if ( !( ScriptInstruction::write_GT_STRING( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_I64: if ( !( ScriptInstruction::write_GT_I64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_U64: if ( !( ScriptInstruction::write_GT_U64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_F64: if ( !( ScriptInstruction::write_GT_F64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;

        default:
          reportError( "invalid type for > comparison" );
        break;
      }
    }
    break;

    case Operator::OPERATOR_GE:
    {
      switch ( lhsTypeDescription.mScriptType )
      {
        case ScriptTypes::ScriptType::TYPE_STRING: if ( !( ScriptInstruction::write_GE_STRING( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_I64: if ( !( ScriptInstruction::write_GE_I64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_U64: if ( !( ScriptInstruction::write_GE_U64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_F64: if ( !( ScriptInstruction::write_GE_F64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;

        default:
          reportError( "invalid type for >= comparison" );
        break;
      }
    }
    break;

    case Operator::OPERATOR_LT:
    {
      switch ( lhsTypeDescription.mScriptType )
      {
        case ScriptTypes::ScriptType::TYPE_STRING: if ( !( ScriptInstruction::write_LT_STRING( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_I64: if ( !( ScriptInstruction::write_LT_I64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_U64: if ( !( ScriptInstruction::write_LT_U64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_F64: if ( !( ScriptInstruction::write_LT_F64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;

        default:
          reportError( "invalid type for < comparison" );
        break;
      }
    }
    break;

    case Operator::OPERATOR_LE:
    {
      switch ( lhsTypeDescription.mScriptType )
      {
        case ScriptTypes::ScriptType::TYPE_STRING: if ( !( ScriptInstruction::write_LE_STRING( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_I64: if ( !( ScriptInstruction::write_LE_I64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_U64: if ( !( ScriptInstruction::write_LE_U64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_F64: if ( !( ScriptInstruction::write_LE_F64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;

        default:
          reportError( "invalid type for <= comparison" );
        break;
      }
    }
    break;

    case Operator::OPERATOR_PLUS:
    {
      switch ( lhsTypeDescription.mScriptType )
      {
        case ScriptTypes::ScriptType::TYPE_I64: if ( !( ScriptInstruction::write_PLUS_I64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_U64: if ( !( ScriptInstruction::write_PLUS_U64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_F64: if ( !( ScriptInstruction::write_PLUS_F64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;

        case ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS:
          if ( !( generateCodeLibraryClassOperatorCall( dest, "operator_add", "+", lhsTypeDescription, rhsTypeDescription ) ) )
            retVal = false;
        break;

        default:
          reportError( "invalid type for binary +" );
        break;
      }
    }
    break;

    case Operator::OPERATOR_MINUS:
    {
      switch ( lhsTypeDescription.mScriptType )
      {
        case ScriptTypes::ScriptType::TYPE_I64: if ( !( ScriptInstruction::write_SUBTRACT_I64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_U64: if ( !( ScriptInstruction::write_SUBTRACT_U64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_F64: if ( !( ScriptInstruction::write_SUBTRACT_F64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;

        case ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS:
          if ( !( generateCodeLibraryClassOperatorCall( dest, "operator_subtract", "-", lhsTypeDescription, rhsTypeDescription ) ) )
            retVal = false;
        break;

        default:
          reportError( "invalid type for binary -" );
        break;
      }
    }
    break;

    case Operator::OPERATOR_MULTIPLY:
    {
      if ( typeMismatchLibraryClass.mScriptType == ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS )
      {
        if ( !( generateCodeLibraryClassOperatorCall( dest, "operator_multiply", "*", lhsTypeDescription, rhsTypeDescription ) ) )
          retVal = false;
      }
      else
      {
        switch ( lhsTypeDescription.mScriptType )
        {
          case ScriptTypes::ScriptType::TYPE_I64: if ( !( ScriptInstruction::write_MULTIPLY_I64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
          case ScriptTypes::ScriptType::TYPE_U64: if ( !( ScriptInstruction::write_MULTIPLY_U64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
          case ScriptTypes::ScriptType::TYPE_F64: if ( !( ScriptInstruction::write_MULTIPLY_F64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;

          case ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS:
            if ( !( generateCodeLibraryClassOperatorCall( dest, "operator_multiply", "*", lhsTypeDescription, rhsTypeDescription ) ) )
              retVal = false;
          break;

          default:
            reportError( "invalid type for binary *" );
          break;
        }
      }
    }
    break;

    case Operator::OPERATOR_DIVIDE:
    {
      if ( typeMismatchLibraryClass.mScriptType == ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS )
      {
        if ( !( generateCodeLibraryClassOperatorCall( dest, "operator_divide", "/", lhsTypeDescription, rhsTypeDescription ) ) )
          retVal = false;
      }
      else
      {
        switch ( lhsTypeDescription.mScriptType )
        {
          case ScriptTypes::ScriptType::TYPE_I64: if ( !( ScriptInstruction::write_DIVIDE_I64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
          case ScriptTypes::ScriptType::TYPE_U64: if ( !( ScriptInstruction::write_DIVIDE_U64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
          case ScriptTypes::ScriptType::TYPE_F64: if ( !( ScriptInstruction::write_DIVIDE_F64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;

          case ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS:
            if ( !( generateCodeLibraryClassOperatorCall( dest, "operator_divide", "/", lhsTypeDescription, rhsTypeDescription ) ) )
              retVal = false;
          break;

          default:
            reportError( "invalid type for binary /" );
          break;
        }
      }
    }
    break;

    case Operator::OPERATOR_BOOL_AND:
    {
      switch ( lhsTypeDescription.mScriptType )
      {
        case ScriptTypes::ScriptType::TYPE_I64: if ( !( ScriptInstruction::write_BOOL_AND_I64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_U64: if ( !( ScriptInstruction::write_BOOL_AND_U64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_F64: if ( !( ScriptInstruction::write_BOOL_AND_F64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;

        default:
          reportError( "invalid type for bool &&" );
        break;
      }
    }
    break;

    case Operator::OPERATOR_BOOL_OR:
    {
      switch ( lhsTypeDescription.mScriptType )
      {
        case ScriptTypes::ScriptType::TYPE_I64: if ( !( ScriptInstruction::write_BOOL_OR_I64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_U64: if ( !( ScriptInstruction::write_BOOL_OR_U64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_F64: if ( !( ScriptInstruction::write_BOOL_OR_F64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;

        default:
          reportError( "invalid type for bool ||" );
        break;
      }
    }
    break;

    case Operator::OPERATOR_BIT_AND:
    {
      switch ( lhsTypeDescription.mScriptType )
      {
        case ScriptTypes::ScriptType::TYPE_I64: if ( !( ScriptInstruction::write_BIT_AND_I64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_U64: if ( !( ScriptInstruction::write_BIT_AND_U64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;

        default:
          reportError( "invalid type for bitwise &" );
        break;
      }
    }
    break;

    case Operator::OPERATOR_BIT_OR:
    {
      switch ( lhsTypeDescription.mScriptType )
      {
        case ScriptTypes::ScriptType::TYPE_I64: if ( !( ScriptInstruction::write_BIT_OR_I64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;
        case ScriptTypes::ScriptType::TYPE_U64: if ( !( ScriptInstruction::write_BIT_OR_U64( mCompiler, dest, getSourceFilePath(), getSourceLineNumber() ) ) ) retVal = false; break;

        default:
          reportError( "invalid type for bitwise |" );
        break;
      }
    }
    break;

    default:
      reportInternalError( "unknown binary expression operator" );
      retVal = false;
    break;
  }

  return retVal;
}

crunch::core::String SyntaxNodeBinaryExpression::getOperatorName( Operator op )
{
  switch ( op )
  {
    case Operator::OPERATOR_EE: return "OPERATOR_EE";
    case Operator::OPERATOR_NE: return "OPERATOR_NE";
    case Operator::OPERATOR_GT: return "OPERATOR_GT";
    case Operator::OPERATOR_GE: return "OPERATOR_GE";
    case Operator::OPERATOR_LT: return "OPERATOR_LT";
    case Operator::OPERATOR_LE: return "OPERATOR_LE";
    case Operator::OPERATOR_PLUS: return "OPERATOR_PLUS";
    case Operator::OPERATOR_MINUS: return "OPERATOR_MINUS";
    case Operator::OPERATOR_MULTIPLY: return "OPERATOR_MULTIPLY";
    case Operator::OPERATOR_DIVIDE: return "OPERATOR_DIVIDE";
    case Operator::OPERATOR_BOOL_AND: return "OPERATOR_BOOL_AND";
    case Operator::OPERATOR_BOOL_OR: return "OPERATOR_BOOL_OR";
    case Operator::OPERATOR_BIT_AND: return "OPERATOR_BIT_AND";
    case Operator::OPERATOR_BIT_OR: return "OPERATOR_BIT_OR";

    default:
      // empty
    break;
  }

  return "OPERATOR_NONE";
}

SyntaxNodeBinaryExpression::Operator SyntaxNodeBinaryExpression::getOperator( const crunch::core::String& name )
{
  crunch::core::String findMe = name;
  findMe.toUpper();

  if ( findMe == "OPERATOR_EE" ) return Operator::OPERATOR_EE;
  if ( findMe == "OPERATOR_NE" ) return Operator::OPERATOR_NE;
  if ( findMe == "OPERATOR_GT" ) return Operator::OPERATOR_GT;
  if ( findMe == "OPERATOR_GE" ) return Operator::OPERATOR_GE;
  if ( findMe == "OPERATOR_LT" ) return Operator::OPERATOR_LT;
  if ( findMe == "OPERATOR_LE" ) return Operator::OPERATOR_LE;
  if ( findMe == "OPERATOR_PLUS" ) return Operator::OPERATOR_PLUS;
  if ( findMe == "OPERATOR_MINUS" ) return Operator::OPERATOR_MINUS;
  if ( findMe == "OPERATOR_MULTIPLY" ) return Operator::OPERATOR_MULTIPLY;
  if ( findMe == "OPERATOR_DIVIDE" ) return Operator::OPERATOR_DIVIDE;
  if ( findMe == "OPERATOR_BOOL_AND" ) return Operator::OPERATOR_BOOL_AND;
  if ( findMe == "OPERATOR_BOOL_OR" ) return Operator::OPERATOR_BOOL_OR;
  if ( findMe == "OPERATOR_BIT_AND" ) return Operator::OPERATOR_BIT_AND;
  if ( findMe == "OPERATOR_BIT_OR" ) return Operator::OPERATOR_BIT_OR;

  return Operator::OPERATOR_NONE;
}

bool SyntaxNodeBinaryExpression::generateCodeLibraryClassOperatorCall( crunch::core::BinaryBuffer *dest, 
                                                                       const crunch::core::String& operatorName, 
                                                                       const crunch::core::String& operatorSymbol,
                                                                       const ScriptTypes::ScriptTypeDescription& lhsTypeDescription,
                                                                       const ScriptTypes::ScriptTypeDescription& rhsTypeDescription
                                                                       )
{
  crunch::core::String classIdentifier;

  if ( lhsTypeDescription.mScriptType == ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS )
    classIdentifier = lhsTypeDescription.mIdentifier;
  else
    classIdentifier = rhsTypeDescription.mIdentifier;

  const ScriptLibraryClass *slc = ScriptLibrary::getGlobalLibraryClass();
  
  if ( slc != nullptr )
  {
    crunch::core::String operatorSignature = crunch::core::String::fmt( "%s_%s(%s,%s)", classIdentifier.c_str(), operatorName.c_str(), lhsTypeDescription.getTypeDescription().c_str(), rhsTypeDescription.getTypeDescription().c_str() );

    if ( slc->hasFunctionSignature( operatorSignature ) )
    {
      const crunch::core::String *funcSig = mCompiler->stringTableMergeString( operatorSignature.c_str(), false );
      return ScriptInstruction::write_LIB_GFUNC_CALL( mCompiler, dest, getSourceFilePath(), getSourceLineNumber(), funcSig, crunch::core::String::fmt( "library operator %s", operatorSymbol.c_str() ) );
    }
    else
      reportError( "library class '%s' does not define operator '%s'", classIdentifier.c_str(), operatorSymbol.c_str() );
  }
  else
    reportInternalError( "global library class not found while looking for operator '%s'", operatorSymbol.c_str() );

  return false;
}

} }
