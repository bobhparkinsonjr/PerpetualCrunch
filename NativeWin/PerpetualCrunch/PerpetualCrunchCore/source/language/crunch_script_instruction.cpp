
#include "../crunch_global.h"

#include "crunch_script_compiler.h"

#include "crunch_script_instruction.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#define write_crunch_instruction( _op ) \
  if ( !( writeInstruction( dest, ScriptInstruction::Opcode:: OP_##_op ) ) ) \
    return false; \
  CRUNCH_COMPILER_APPEND_ASSEMBLER_OP( sourceFilePath, lineNumber, #_op , "" ); \
  return true;

#define write_crunch_instruction_comment( _op, _comment ) \
  if ( !( writeInstruction( dest, ScriptInstruction::Opcode:: OP_##_op ) ) ) \
    return false; \
  CRUNCH_COMPILER_APPEND_ASSEMBLER_OP_COM( sourceFilePath, lineNumber, #_op , "", _comment ); \
  return true;

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

bool ScriptInstruction::write_POP_STORE_LOCAL( _script_instruction_params, uint32_t dataOffset, uint64_t dataSize, const crunch::core::String& comment )
{
  if ( !( writeInstruction( dest, ScriptInstruction::Opcode::OP_POP_STORE_LOCAL, dataOffset ) ) )
    return false;

  dest->append_uint64( dataSize );

  CRUNCH_COMPILER_APPEND_ASSEMBLER_OP_COM( sourceFilePath, lineNumber, "POP_STORE_LOCAL", crunch::core::String::fmt( "offset:%u size:%" PRIu64, dataOffset, dataSize ), comment );

  return true;
}

bool ScriptInstruction::write_POP( _script_instruction_params, uint32_t scopeStackSize, const crunch::core::String& comment )
{
  if ( !( writeInstruction( dest, ScriptInstruction::Opcode::OP_POP, scopeStackSize ) ) )
    return false;

  CRUNCH_COMPILER_APPEND_ASSEMBLER_OP_COM( sourceFilePath, lineNumber, "POP", crunch::core::String::fmt( "stack size:%u", scopeStackSize ), comment );

  return true;
}

bool ScriptInstruction::write_PUSH_FETCH_LOCAL( _script_instruction_params, uint32_t dataOffset, uint64_t dataSize, const crunch::core::String& comment )
{
  if ( !( writeInstruction( dest, ScriptInstruction::Opcode::OP_PUSH_FETCH_LOCAL, dataOffset ) ) )
    return false;

  dest->append_uint64( dataSize );

  CRUNCH_COMPILER_APPEND_ASSEMBLER_OP_COM( sourceFilePath, lineNumber, "PUSH_FETCH_LOCAL", crunch::core::String::fmt( "offset:%u size:%" PRIu64, dataOffset, dataSize ), comment );

  return true;
}

bool ScriptInstruction::update_PUSH_FETCH_LOCAL_member( _script_instruction_params, uint32_t memberOffset, uint64_t memberSize )
{
  char *pc = dest->c_str_offset() - ( sizeof( uint64_t ) * 2 );

  if ( ( *(uint64_t*)( pc ) & 0xFFFFFFFF ) != (uint64_t)( ScriptInstruction::Opcode::OP_PUSH_FETCH_LOCAL ) )
    return false;

  CRUNCH_COMPILER_APPEND_ASSEMBLER( sourceFilePath, lineNumber, crunch::core::String::fmt( ";         PUSH_FETCH_LOCAL                        updated for member fetch | member-offset:%u member-size:%" PRIu64 "\n", memberOffset, memberSize ) );

  uint32_t adjustedDataOffset = (uint32_t)( ( ( *(uint64_t*)( pc ) >> 32 ) & 0xFFFFFFFF ) ) + memberOffset;

  *(uint64_t*)( pc ) = (uint64_t)( (uint64_t)( ScriptInstruction::Opcode::OP_PUSH_FETCH_LOCAL ) | ( (uint64_t)( adjustedDataOffset ) << 32 ) );
  *(uint64_t*)( pc + sizeof( uint64_t ) ) = memberSize;

  return true;
}

bool ScriptInstruction::write_PUSH_FETCH_GLIB_CLS( _script_instruction_params, const crunch::core::String *className, const crunch::core::String *instanceName, const crunch::core::String& comment )
{
  unsigned int classIndex = compiler->stringTableGetIndex( className );
  unsigned int instanceIndex = compiler->stringTableGetIndex( instanceName );

  if ( classIndex == UINT_MAX )
  {
    if ( className != nullptr )
      compiler->reportInternalError( sourceFilePath, lineNumber, "class string '%s' not found in compiler string table", className->c_str() );
    else
      compiler->reportInternalError( sourceFilePath, lineNumber, "class string is nullptr" );

    return false;
  }

  if ( instanceIndex == UINT_MAX )
  {
    if ( instanceName != nullptr )
      compiler->reportInternalError( sourceFilePath, lineNumber, "instance string '%s' not found in compiler string table", instanceName->c_str() );
    else
      compiler->reportInternalError( sourceFilePath, lineNumber, "instance string is nullptr" );

    return false;
  }

  uint32_t opcodeOffset = (uint32_t)( dest->getBufferLength() );

  if ( !( writeInstruction( dest, ScriptInstruction::Opcode::OP_PUSH_FETCH_GLIB_CLS, (uint32_t)( instanceIndex ) ) ) )
    return false;

  dest->append_uint64( (uint64_t)( classIndex ) );

  CRUNCH_COMPILER_APPEND_ASSEMBLER_OP_COM( sourceFilePath, lineNumber, "PUSH_FETCH_GLIB_CLS", crunch::core::String::fmt( "%s (str:%u) %s (str::%u)", className->c_str(), classIndex, instanceName->c_str(), instanceIndex ), comment );

  const ScriptExecutable *se = compiler->getExecutable();
  LinkTableI2 *linkTable = (LinkTableI2*)( se->getLinkTable_PUSH_FETCH_GLIB_CLS() );

  if ( !( linkTable->append( (uint32_t)( instanceIndex ), (uint64_t)( classIndex ), opcodeOffset ) ) )
  {
    compiler->reportInternalError( sourceFilePath, lineNumber, "unable to record link instruction for PUSH_FETCH_GLIB_CLS" );
    return false;
  }

  return true;
}

bool ScriptInstruction::write_FETCH_TEMP_MEMBER( _script_instruction_params, uint32_t classSize, uint32_t memberSize, uint32_t memberOffset, const crunch::core::String& comment )
{
  if ( !( writeInstruction( dest, ScriptInstruction::Opcode::OP_FETCH_TEMP_MEMBER, classSize ) ) )
    return false;

  dest->append_uint64( ( (uint64_t)( memberSize ) << 32 ) | (uint64_t)( memberOffset ) );

  CRUNCH_COMPILER_APPEND_ASSEMBLER_OP_COM( sourceFilePath, lineNumber, "FETCH_TEMP_MEMBER", crunch::core::String::fmt( "class-size:%u member-size:%u member-offset:%u", classSize, memberSize, memberOffset ), comment );

  return true;
}

bool ScriptInstruction::write_PUSH_STRING( _script_instruction_params, const crunch::core::String *s, const crunch::core::String& comment )
{
  unsigned int index = compiler->stringTableGetIndex( s );

  if ( index == UINT_MAX )
  {
    if ( s != nullptr )
      compiler->reportInternalError( sourceFilePath, lineNumber, "string '%s' not found in compiler string table", s->c_str() );
    else
      compiler->reportInternalError( sourceFilePath, lineNumber, "string is nullptr" );

    return false;
  }

  if ( !( writeInstruction( dest, ScriptInstruction::Opcode::OP_PUSH_STRING, (uint32_t)( index ) ) ) )
    return false;

  CRUNCH_COMPILER_APPEND_ASSEMBLER_OP_COM( sourceFilePath, lineNumber, "PUSH_STRING", crunch::core::String::fmt( "\"%s\" (str:%u)", s->c_str(), index ), comment );

  return true;
}

bool ScriptInstruction::write_PUSH_I64( _script_instruction_params, int64_t i, const crunch::core::String& comment )
{
  if ( !( writeInstruction( dest, ScriptInstruction::Opcode::OP_PUSH_I64 ) ) )
    return false;

  dest->append_int64( i );

  CRUNCH_COMPILER_APPEND_ASSEMBLER_OP_COM( sourceFilePath, lineNumber, "PUSH_I64", crunch::core::String::fmt( "%" PRId64, i ), comment );

  return true;
}

bool ScriptInstruction::write_PUSH_U64( _script_instruction_params, uint64_t u, const crunch::core::String& comment )
{
  if ( !( writeInstruction( dest, ScriptInstruction::Opcode::OP_PUSH_U64 ) ) )
    return false;

  dest->append_uint64( u );

  CRUNCH_COMPILER_APPEND_ASSEMBLER_OP_COM( sourceFilePath, lineNumber, "PUSH_U64", crunch::core::String::fmt( "%" PRIu64, u ), comment );

  return true;
}

bool ScriptInstruction::write_PUSH_F64( _script_instruction_params, double f, const crunch::core::String& comment )
{
  if ( !( writeInstruction( dest, ScriptInstruction::Opcode::OP_PUSH_F64 ) ) )
    return false;

  dest->append_double( f );

  CRUNCH_COMPILER_APPEND_ASSEMBLER_OP_COM( sourceFilePath, lineNumber, "PUSH_F64", crunch::core::String::fmt( "%0.8f", f ), comment );

  return true;
}

bool ScriptInstruction::write_PUSH_NULL_PTR( _script_instruction_params )
{
  write_crunch_instruction( PUSH_NULL_PTR );
}

bool ScriptInstruction::write_RETURN( _script_instruction_params, const crunch::core::String& comment )
{
  write_crunch_instruction_comment( RETURN, comment );
}

bool ScriptInstruction::write_RETURN2( _script_instruction_params, uint32_t scopeStackSize, uint64_t returnTypeStackSize, const crunch::core::String& comment )
{
  if ( !( writeInstruction( dest, ScriptInstruction::Opcode::OP_RETURN2, scopeStackSize ) ) )
    return false;

  dest->append_uint64( returnTypeStackSize );

  CRUNCH_COMPILER_APPEND_ASSEMBLER_OP_COM( sourceFilePath, lineNumber, "RETURN2", crunch::core::String::fmt( "scope:%u return:%u", scopeStackSize, returnTypeStackSize ), comment );

  return true;
}

bool ScriptInstruction::write_MINUS_I64( _script_instruction_params ) { write_crunch_instruction( MINUS_I64 ); }
bool ScriptInstruction::write_MINUS_F64( _script_instruction_params ) { write_crunch_instruction( MINUS_F64 ); }

bool ScriptInstruction::write_BOOL_NOT_I64( _script_instruction_params ) { write_crunch_instruction( BOOL_NOT_I64 ); }

bool ScriptInstruction::write_BIT_COMP_I64( _script_instruction_params ) { write_crunch_instruction( BIT_COMP_I64 ); }
bool ScriptInstruction::write_BIT_COMP_U64( _script_instruction_params ) { write_crunch_instruction( BIT_COMP_U64 ); }

bool ScriptInstruction::write_EE_STRING( _script_instruction_params ) { write_crunch_instruction( EE_STRING ); }
bool ScriptInstruction::write_EE_I64( _script_instruction_params ) { write_crunch_instruction( EE_I64 ); }
bool ScriptInstruction::write_EE_U64( _script_instruction_params ) { write_crunch_instruction( EE_U64 ); }
bool ScriptInstruction::write_EE_F64( _script_instruction_params ) { write_crunch_instruction( EE_F64 ); }

bool ScriptInstruction::write_NE_STRING( _script_instruction_params ) { write_crunch_instruction( NE_STRING ); }
bool ScriptInstruction::write_NE_I64( _script_instruction_params ) { write_crunch_instruction( NE_I64 ); }
bool ScriptInstruction::write_NE_U64( _script_instruction_params ) { write_crunch_instruction( NE_U64 ); }
bool ScriptInstruction::write_NE_F64( _script_instruction_params ) { write_crunch_instruction( NE_F64 ); }

bool ScriptInstruction::write_GT_STRING( _script_instruction_params ) { write_crunch_instruction( GT_STRING ); }
bool ScriptInstruction::write_GT_I64( _script_instruction_params ) { write_crunch_instruction( GT_I64 ); }
bool ScriptInstruction::write_GT_U64( _script_instruction_params ) { write_crunch_instruction( GT_U64 ); }
bool ScriptInstruction::write_GT_F64( _script_instruction_params ) { write_crunch_instruction( GT_F64 ); }

bool ScriptInstruction::write_GE_STRING( _script_instruction_params ) { write_crunch_instruction( GE_STRING ); }
bool ScriptInstruction::write_GE_I64( _script_instruction_params ) { write_crunch_instruction( GE_I64 ); }
bool ScriptInstruction::write_GE_U64( _script_instruction_params ) { write_crunch_instruction( GE_U64 ); }
bool ScriptInstruction::write_GE_F64( _script_instruction_params ) { write_crunch_instruction( GE_F64 ); }

bool ScriptInstruction::write_LT_STRING( _script_instruction_params ) { write_crunch_instruction( LT_STRING ); }
bool ScriptInstruction::write_LT_I64( _script_instruction_params ) { write_crunch_instruction( LT_I64 ); }
bool ScriptInstruction::write_LT_U64( _script_instruction_params ) { write_crunch_instruction( LT_U64 ); }
bool ScriptInstruction::write_LT_F64( _script_instruction_params ) { write_crunch_instruction( LT_F64 ); }

bool ScriptInstruction::write_LE_STRING( _script_instruction_params ) { write_crunch_instruction( LE_STRING ); }
bool ScriptInstruction::write_LE_I64( _script_instruction_params ) { write_crunch_instruction( LE_I64 ); }
bool ScriptInstruction::write_LE_U64( _script_instruction_params ) { write_crunch_instruction( LE_U64 ); }
bool ScriptInstruction::write_LE_F64( _script_instruction_params ) { write_crunch_instruction( LE_F64 ); }

bool ScriptInstruction::write_PLUS_I64( _script_instruction_params ) { write_crunch_instruction( PLUS_I64 ); }
bool ScriptInstruction::write_PLUS_U64( _script_instruction_params ) { write_crunch_instruction( PLUS_U64 ); }
bool ScriptInstruction::write_PLUS_F64( _script_instruction_params ) { write_crunch_instruction( PLUS_F64 ); }

bool ScriptInstruction::write_SUBTRACT_I64( _script_instruction_params ) { write_crunch_instruction( SUBTRACT_I64 ); }
bool ScriptInstruction::write_SUBTRACT_U64( _script_instruction_params ) { write_crunch_instruction( SUBTRACT_U64 ); }
bool ScriptInstruction::write_SUBTRACT_F64( _script_instruction_params ) { write_crunch_instruction( SUBTRACT_F64 ); }

bool ScriptInstruction::write_MULTIPLY_I64( _script_instruction_params ) { write_crunch_instruction( MULTIPLY_I64 ); }
bool ScriptInstruction::write_MULTIPLY_U64( _script_instruction_params ) { write_crunch_instruction( MULTIPLY_U64 ); }
bool ScriptInstruction::write_MULTIPLY_F64( _script_instruction_params ) { write_crunch_instruction( MULTIPLY_F64 ); }

bool ScriptInstruction::write_DIVIDE_I64( _script_instruction_params ) { write_crunch_instruction( DIVIDE_I64 ); }
bool ScriptInstruction::write_DIVIDE_U64( _script_instruction_params ) { write_crunch_instruction( DIVIDE_U64 ); }
bool ScriptInstruction::write_DIVIDE_F64( _script_instruction_params ) { write_crunch_instruction( DIVIDE_F64 ); }

bool ScriptInstruction::write_BOOL_AND_I64( _script_instruction_params ) { write_crunch_instruction( BOOL_AND_I64 ); }
bool ScriptInstruction::write_BOOL_AND_U64( _script_instruction_params ) { write_crunch_instruction( BOOL_AND_U64 ); }
bool ScriptInstruction::write_BOOL_AND_F64( _script_instruction_params ) { write_crunch_instruction( BOOL_AND_F64 ); }

bool ScriptInstruction::write_BOOL_OR_I64( _script_instruction_params ) { write_crunch_instruction( BOOL_OR_I64 ); }
bool ScriptInstruction::write_BOOL_OR_U64( _script_instruction_params ) { write_crunch_instruction( BOOL_OR_U64 ); }
bool ScriptInstruction::write_BOOL_OR_F64( _script_instruction_params ) { write_crunch_instruction( BOOL_OR_F64 ); }

bool ScriptInstruction::write_BIT_AND_I64( _script_instruction_params ) { write_crunch_instruction( BIT_AND_I64 ); }
bool ScriptInstruction::write_BIT_AND_U64( _script_instruction_params ) { write_crunch_instruction( BIT_AND_U64 ); }

bool ScriptInstruction::write_BIT_OR_I64( _script_instruction_params ) { write_crunch_instruction( BIT_OR_I64 ); }
bool ScriptInstruction::write_BIT_OR_U64( _script_instruction_params ) { write_crunch_instruction( BIT_OR_U64 ); }

bool ScriptInstruction::write_STRING_TO_I64( _script_instruction_params ) { write_crunch_instruction( STRING_TO_I64 ); }
bool ScriptInstruction::write_U64_TO_I64( _script_instruction_params ) { write_crunch_instruction( U64_TO_I64 ); }
bool ScriptInstruction::write_F64_TO_I64( _script_instruction_params ) { write_crunch_instruction( F64_TO_I64 ); }

bool ScriptInstruction::write_STRING_TO_U64( _script_instruction_params ) { write_crunch_instruction( STRING_TO_U64 ); }
bool ScriptInstruction::write_I64_TO_U64( _script_instruction_params ) { write_crunch_instruction( I64_TO_U64 ); }
bool ScriptInstruction::write_F64_TO_U64( _script_instruction_params ) { write_crunch_instruction( F64_TO_U64 ); }

bool ScriptInstruction::write_STRING_TO_F64( _script_instruction_params ) { write_crunch_instruction( STRING_TO_F64 ); }
bool ScriptInstruction::write_I64_TO_F64( _script_instruction_params ) { write_crunch_instruction( I64_TO_F64 ); }
bool ScriptInstruction::write_U64_TO_F64( _script_instruction_params ) { write_crunch_instruction( U64_TO_F64 ); }

bool ScriptInstruction::write_LIB_FUNC_CALL( _script_instruction_params, const crunch::core::String *className, const crunch::core::String *functionSignature, const crunch::core::String& comment )
{
  unsigned int classIndex = compiler->stringTableGetIndex( className );
  unsigned int funcIndex = compiler->stringTableGetIndex( functionSignature );

  if ( classIndex == UINT_MAX )
  {
    if ( className != nullptr )
      compiler->reportInternalError( sourceFilePath, lineNumber, "class string '%s' not found in compiler string table", className->c_str() );
    else
      compiler->reportInternalError( sourceFilePath, lineNumber, "class string is nullptr" );

    return false;
  }

  if ( funcIndex == UINT_MAX )
  {
    if ( functionSignature != nullptr )
      compiler->reportInternalError( sourceFilePath, lineNumber, "function string '%s' not found in compiler string table", functionSignature->c_str() );
    else
      compiler->reportInternalError( sourceFilePath, lineNumber, "function string is nullptr" );

    return false;
  }

  uint32_t opcodeOffset = (uint32_t)( dest->getBufferLength() );

  if ( !( writeInstruction( dest, ScriptInstruction::Opcode::OP_LIB_FUNC_CALL, (uint32_t)( funcIndex ) ) ) )
    return false;

  dest->append_uint64( (uint64_t)( classIndex ) );

  CRUNCH_COMPILER_APPEND_ASSEMBLER_OP_COM( sourceFilePath, lineNumber, "LIB_FUNC_CALL", crunch::core::String::fmt( "%s (str:%u) :: %s (str:%u)", className->c_str(), classIndex, functionSignature->c_str(), funcIndex ), comment );

  const ScriptExecutable *se = compiler->getExecutable();
  LinkTableI2 *linkTable = (LinkTableI2*)( se->getLinkTable_LIB_FUNC_CALL() );

  if ( !( linkTable->append( (uint32_t)( funcIndex ), (uint64_t)( classIndex ), opcodeOffset ) ) )
  {
    compiler->reportInternalError( sourceFilePath, lineNumber, "unable to record link instruction for LIB_FUNC_CALL" );
    return false;
  }

  return true;
}

bool ScriptInstruction::write_LIB_GFUNC_CALL( _script_instruction_params, const crunch::core::String *functionSignature, const crunch::core::String& comment )
{
  unsigned int index = compiler->stringTableGetIndex( functionSignature );

  if ( index == UINT_MAX )
  {
    if ( functionSignature != nullptr )
      compiler->reportInternalError( sourceFilePath, lineNumber, "string '%s' not found in compiler string table", functionSignature->c_str() );
    else
      compiler->reportInternalError( sourceFilePath, lineNumber, "string is nullptr" );

    return false;
  }

  uint32_t opcodeOffset = (uint32_t)( dest->getBufferLength() );

  if ( !( writeInstruction( dest, ScriptInstruction::Opcode::OP_LIB_GFUNC_CALL, (uint32_t)( index ) ) ) )
    return false;

  dest->append_uint64( 0 ); // placeholder for library function pointer at run-time

  CRUNCH_COMPILER_APPEND_ASSEMBLER_OP_COM( sourceFilePath, lineNumber, "LIB_GFUNC_CALL", crunch::core::String::fmt( "%s (str:%u)", functionSignature->c_str(), index ), comment );

  const ScriptExecutable *se = compiler->getExecutable();
  LinkTableI1 *linkTable = (LinkTableI1*)( se->getLinkTable_LIB_GFUNC_CALL() );

  if ( !( linkTable->append( (uint32_t)( index ), opcodeOffset ) ) )
  {
    compiler->reportInternalError( sourceFilePath, lineNumber, "unable to record link instruction for LIB_GFUNC_CALL" );
    return false;
  }

  return true;
}

bool ScriptInstruction::write_SO_FUNC_CALL( _script_instruction_params )
{
  // TODO
  return false;
}

bool ScriptInstruction::write_SO_GFUNC_CALL( _script_instruction_params, uint32_t offset, uint64_t paramsStackSize, const crunch::core::String& comment )
{
  if ( !( writeInstruction( dest, ScriptInstruction::Opcode::OP_SO_GFUNC_CALL, offset ) ) )
    return false;

  dest->append_uint64( paramsStackSize );

  CRUNCH_COMPILER_APPEND_ASSEMBLER_OP_COM( sourceFilePath, lineNumber, "SO_GFUNC_CALL", crunch::core::String::fmt( "offset:%u params:%u", offset, paramsStackSize ), comment );

  return true;
}

bool ScriptInstruction::write_BRANCH( _script_instruction_params, const crunch::core::String& comment )
{
  if ( !( writeInstruction( dest, ScriptInstruction::Opcode::OP_BRANCH, 0 ) ) )
    return false;
  
  CRUNCH_COMPILER_APPEND_ASSEMBLER_OP_COM( sourceFilePath, lineNumber, "BRANCH", "", comment );

  return true;
}

bool ScriptInstruction::update_BRANCH( ScriptCompiler *compiler, char *pc, uint32_t falseOffset )
{
  assert( *(uint64_t*)( pc ) == (uint64_t)( ScriptInstruction::Opcode::OP_BRANCH ) && "expected BRANCH opcode" );
  *(uint64_t*)( pc ) = (uint64_t)( (uint64_t)( ScriptInstruction::Opcode::OP_BRANCH ) | ( (uint64_t)( falseOffset ) << 32 ) );
  return true;
}

bool ScriptInstruction::write_BRANCHF( _script_instruction_params, const crunch::core::String& comment )
{
  if ( !( writeInstruction( dest, ScriptInstruction::Opcode::OP_BRANCHF, 0 ) ) )
    return false;
  
  CRUNCH_COMPILER_APPEND_ASSEMBLER_OP_COM( sourceFilePath, lineNumber, "BRANCHF", "", comment );

  return true;
}

bool ScriptInstruction::update_BRANCHF( ScriptCompiler *compiler, char *pc, uint32_t endIfOffset )
{
  assert( *(uint64_t*)( pc ) == (uint64_t)( ScriptInstruction::Opcode::OP_BRANCHF ) && "expected BRANCHF opcode" );
  *(uint64_t*)( pc ) = (uint64_t)( (uint64_t)( ScriptInstruction::Opcode::OP_BRANCHF ) | ( (uint64_t)( endIfOffset ) << 32 ) );
  return true;
}

bool ScriptInstruction::write_JUMP( _script_instruction_params, const crunch::core::String& comment )
{
  if ( !( writeInstruction( dest, ScriptInstruction::Opcode::OP_JUMP, 0 ) ) )
    return false;

  CRUNCH_COMPILER_APPEND_ASSEMBLER_OP_COM( sourceFilePath, lineNumber, "JUMP", "", comment );

  return true;
}

bool ScriptInstruction::update_JUMP( ScriptCompiler *compiler, char *pc, uint32_t offset )
{
  assert( *(uint64_t*)( pc ) == (uint64_t)( ScriptInstruction::Opcode::OP_JUMP ) && "expected JUMP opcode" );
  *(uint64_t*)( pc ) = (uint64_t)( (uint64_t)( ScriptInstruction::Opcode::OP_JUMP ) | ( (uint64_t)( offset ) << 32 ) );
  return true;
}

crunch::core::String ScriptInstruction::getOpcodeName( Opcode op )
{
  switch ( op )
  {
    #define ROW( _enum_name ) case Opcode::OP_##_enum_name : return #_enum_name ;
    #include "crunch_script_opcodes.table"
    #undef ROW

    default:
      // empty
    break;
  }

  return "";
}

bool ScriptInstruction::write_SWITCH_JUMP_TABLE( _script_instruction_params, int64_t arrayBaseValue, int32_t jumpTableSize, const crunch::core::String& comment )
{
  if ( jumpTableSize < 0 )
    return false;

  if ( !( writeInstruction( dest, ScriptInstruction::Opcode::OP_SWITCH_JUMP_TABLE, 0 ) ) )
    return false;

  dest->append_int64( arrayBaseValue );
  dest->append_int64( int64_t( jumpTableSize ) );

  // default case offset placeholder, zero also indicates there is no default case
  dest->append_uint64( 0 );

  // jump table
  {
    for ( int32_t i = 0; i < jumpTableSize; ++i )
      dest->append_uint64( 0 );
  }

  CRUNCH_COMPILER_APPEND_ASSEMBLER_OP_COM( sourceFilePath, lineNumber, "SWITCH_JUMP_TABLE", "", comment );

  return true;
}

bool ScriptInstruction::update_SWITCH_JUMP_TABLE( ScriptCompiler *compiler, char *pc, uint64_t defaultCaseOffset, uint64_t *jumpTable )
{
  assert( *(uint64_t*)( pc ) == (uint64_t)( ScriptInstruction::Opcode::OP_SWITCH_JUMP_TABLE ) && "expected SWITCH_JUMP_TABLE opcode" );
  assert( defaultCaseOffset > 0 && "default case offset must be provided, set to after switch statement if no default case" );

  if ( jumpTable == nullptr )
    return false;

  int64_t jumpTableSize = *(int64_t*)( pc + sizeof( uint64_t ) + sizeof( int64_t ) );

  if ( jumpTableSize < 0 )
    return false;

  *(uint64_t*)( pc + sizeof( uint64_t ) + sizeof( int64_t ) + sizeof( int64_t ) ) = defaultCaseOffset;

  if ( jumpTableSize > 0 )
  {
    uint64_t *byteCodeJumpTable = (uint64_t*)( pc + sizeof( uint64_t ) + sizeof( int64_t ) + sizeof( int64_t ) + sizeof( uint64_t ) );
    memcpy( byteCodeJumpTable, jumpTable, jumpTableSize * sizeof( uint64_t ) );
  }

  return true;
}

bool ScriptInstruction::writeInstruction( crunch::core::BinaryBuffer *dest, Opcode op, uint32_t arg )
{
  static_assert( sizeof( arg ) == 4, "arg must be exactly 4 bytes wide" );

  if ( dest == nullptr )
    return false;

  if ( ( dest->getBufferLength() % 8 ) != 0 )
    return false;

  return dest->append_uint64( (uint64_t)( op ) | ( (uint64_t)( arg ) << 32 ) );
}

} }
