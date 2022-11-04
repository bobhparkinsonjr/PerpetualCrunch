
#include "../crunch_global.h"

#include "crunch_script_vm.h"
#include "crunch_script_executable.h"
#include "crunch_script_instruction.h"
#include "crunch_script_library.h"
#include "crunch_script_library_class.h"
#include "crunch_script_library_function.h"

#include "crunch_script_thread.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

ScriptThread::ScriptThread( ScriptVM *vm, const ScriptExecutable *executable ) :
  mVM( vm ),
  mExecutable( executable )
{
  // empty
}

ScriptThread::~ScriptThread()
{
  destroy();
}

void ScriptThread::destroy()
{
  if ( isRunning() )
  {
    cancel();
    mProcessingThread.wait();
  }
}

bool ScriptThread::setRootFunction( const crunch::core::String& signature )
{
  if ( mExecutable == nullptr )
    return false;

  if ( mExecutable->getGlobalObject()->getFunctionInfo( signature, &mRootFunctionOffset, &mRootFunctionReturnType ) )
  {
    mRootFunctionSignature = signature;
    return true;
  }

  return false;
}

bool ScriptThread::setRootFunction( const crunch::core::String& name, const std::vector< ScriptTypes::ScriptTypeDescription >& parameterTypes )
{
  crunch::core::String signature;

  if ( ScriptExecutable::getFunctionSignature( name, parameterTypes, &signature ) )
    return setRootFunction( signature );

  return false;
}

bool ScriptThread::read_f64( crunch_f64 *f )
{
  if ( isRunning() || mExitType != ExitType::EXIT_NORMAL || f == nullptr )
    return false;
  *f = mDataStack.read_f64();
  return true;
}

bool ScriptThread::isRunning()
{
  if ( !( mProcessingThread.isCreated() ) )
    return false;

  return !( mProcessingThread.wait( 0 ) );
}

bool ScriptThread::run( bool waitComplete, bool runInline )
{
  if ( mRootFunctionSignature == "" )
    return false;

  if ( runInline )
  {
    processing();
    return true;
  }
  else
  if ( mProcessingThread.start( processingEntry, (void*)( this ) ) )
  {
    if ( waitComplete )
      mProcessingThread.wait();

    return true;
  }

  return false;
}

void ScriptThread::cancel()
{
  mCancelRequested = true;
}

crunch::core::String ScriptThread::getExitTypeName( ExitType v )
{
  switch ( v )
  {
    case ExitType::EXIT_NORMAL: return "NORMAL";
    case ExitType::EXIT_BAD_OPCODE: return "BAD_OPCODE";
    case ExitType::EXIT_CANCELED: return "CANCELED";
    case ExitType::EXIT_DIVIDE_BY_ZERO: return "DIVIDE_BY_ZERO";

    default:
      // empty
    break;
  }

  return "";
}

CORE_THREAD_ENTRY_POINT_BODY( ScriptThread, processingEntry )
{
  ScriptThread *inst = (ScriptThread*)( lpParam );
  inst->processing();
  CORE_THREAD_EXIT( 0 );
}

void ScriptThread::processing()
{
  #define EXIT_PROCESSING( _exit_state ) mExitType = ExitType:: _exit_state ; return

  #define BYTE_CODE_INSTRUCTION_ARG32( _var ) uint32_t _var = (uint32_t)( ( currentInstruction >> 32 ) & 0xFFFFFFFF )
  #define BYTE_CODE_INSTRUCTION_ARG32_OFFSET( _var, _offset ) BYTE_CODE_INSTRUCTION_ARG32( _var ); _var += _offset
  #define BYTE_CODE_BUFFER_ARGU64( _var ) uint64_t _var = *(uint64_t*)( pc ); pc += sizeof( uint64_t )
  #define BYTE_CODE_BUFFER_ARGU64_ARRAY( _var, _size ) uint64_t * _var = (uint64_t*)( pc ); pc += ( sizeof( uint64_t ) * _size )
  #define BYTE_CODE_BUFFER_ARGI64( _var ) int64_t _var = *(int64_t*)( pc ); pc += sizeof( int64_t )
  #define BYTE_CODE_BUFFER_ARGF64( _var ) double _var = *(double*)( pc ); pc += sizeof( double )
  #define BYTE_CODE_BUFFER_ARGV3( _var ) double _var [3]; memcpy( &( _var [0] ), pc, sizeof( double ) * 3 ); pc += ( sizeof( double ) * 3 )
  #define BYTE_CODE_BUFFER_ARGPTR( _var ) void * _var = *(void**)( pc ); pc += sizeof( void* )
  #define BYTE_CODE_BUFFER_ARG_LIB_FUNC_PTR( _var ) ScriptLibraryFunction * _var = *(ScriptLibraryFunction**)( pc ); pc += sizeof( ScriptLibraryFunction* )

  const char *byteCode = mExecutable->getByteCodeBuffer()->c_str();

  const char *pc = &( byteCode[mRootFunctionOffset] );
  uint32_t dataStackOffset = 0;

  std::list< FunctionJumpInfo > jumpStack;

  while ( true )
  {
    if ( mCancelRequested )
    {
      EXIT_PROCESSING( EXIT_CANCELED );
    }

    uint64_t currentInstruction = *(uint64_t*)( pc );
    ScriptInstruction::Opcode op = (ScriptInstruction::Opcode)( (uint32_t)( currentInstruction & 0xFFFFFFFF ) );

    pc += sizeof( uint64_t );

    /*
    #if defined _CRUNCH_DEBUG
    {
      uint64_t offset = mDataStack.getOffset();
      corePrint( "op: %s | data stack offset: %u", ScriptInstruction::getOpcodeName( op ).c_str(), offset );
    }
    #endif
    //*/

    switch ( op )
    {
      case ScriptInstruction::Opcode::OP_POP_STORE_LOCAL:
      {
        BYTE_CODE_INSTRUCTION_ARG32_OFFSET( dataOffset, dataStackOffset );
        BYTE_CODE_BUFFER_ARGU64( dataSize );
        mDataStack.pop_store( dataOffset, dataSize );
      }
      break;

      case ScriptInstruction::Opcode::OP_POP:
      {
        BYTE_CODE_INSTRUCTION_ARG32( scopeStackSize );
        mDataStack.pop( scopeStackSize );
      }
      break;

      case ScriptInstruction::Opcode::OP_PUSH_FETCH_LOCAL:
      {
        BYTE_CODE_INSTRUCTION_ARG32_OFFSET( dataOffset, dataStackOffset );
        BYTE_CODE_BUFFER_ARGU64( dataSize );
        mDataStack.push_fetch( dataOffset, dataSize );
      }
      break;

      case ScriptInstruction::Opcode::OP_PUSH_FETCH_GLIB_CLSX:
      {
        BYTE_CODE_BUFFER_ARGPTR( glibInstance );
        mDataStack.push_ptr( glibInstance );
      }
      break;

      case ScriptInstruction::Opcode::OP_FETCH_TEMP_MEMBER:
      {
        BYTE_CODE_INSTRUCTION_ARG32( classSize );
        BYTE_CODE_BUFFER_ARGU64( a );
        uint32_t memberSize = (uint32_t)( ( a >> 32 ) & 0xFFFFFFFF );
        uint32_t memberOffset = (uint32_t)( a & 0xFFFFFFFF );

        char tempClass[ScriptTypes::TYPE_MAX_SIZE_BYTES];
        memcpy( &( tempClass[0] ), mDataStack.getSPOffset( classSize ), classSize );
        mDataStack.pop( classSize );

        mDataStack.push( &( tempClass[0] ) + memberOffset, memberSize );
      }
      break;

      case ScriptInstruction::Opcode::OP_PUSH_STRING:
      {
        BYTE_CODE_INSTRUCTION_ARG32( index );
        const crunch::core::String *s = mExecutable->stringTableGetString( (unsigned int)( index ) );
        mDataStack.push_string( s );
      }
      break;

      case ScriptInstruction::Opcode::OP_PUSH_I64:
      {
        BYTE_CODE_BUFFER_ARGI64( i );
        mDataStack.push_i64( i );
      }
      break;

      case ScriptInstruction::Opcode::OP_PUSH_U64:
      {
        BYTE_CODE_BUFFER_ARGU64( u );
        mDataStack.push_u64( u );
      }
      break;

      case ScriptInstruction::Opcode::OP_PUSH_F64:
      {
        BYTE_CODE_BUFFER_ARGF64( f );
        mDataStack.push_f64( f );
      }
      break;

      case ScriptInstruction::Opcode::OP_PUSH_NULL_PTR:
        // TODO: needed?
      break;

      case ScriptInstruction::Opcode::OP_RETURN:
      {
        if ( jumpStack.empty() )
        {
          // leaving the root function

          #if defined _CRUNCH_DEBUG
          {
            uint64_t offset = mDataStack.getOffset();
            assert( offset == 0 && "expected clean stack when returning from root function" );
          }
          #endif

          EXIT_PROCESSING( EXIT_NORMAL );
        }
        else
        {
          pc = jumpStack.back().mPC;
          dataStackOffset = jumpStack.back().mDataStackOffset;
          jumpStack.pop_back();
        }
      }
      break;

      case ScriptInstruction::Opcode::OP_RETURN2:
      {
        BYTE_CODE_INSTRUCTION_ARG32( scopeStackSize );
        BYTE_CODE_BUFFER_ARGU64( returnTypeStackSize );
        mDataStack.collapse( scopeStackSize, uint32_t( returnTypeStackSize ) );

        if ( jumpStack.empty() )
        {
          // leaving the root function

          EXIT_PROCESSING( EXIT_NORMAL );
        }
        else
        {
          pc = jumpStack.back().mPC;
          dataStackOffset = jumpStack.back().mDataStackOffset;
          jumpStack.pop_back();
        }
      }
      break;

      case ScriptInstruction::Opcode::OP_MINUS_I64:
        mDataStack.push_i64( -( mDataStack.pop_i64() ) );
      break;

      case ScriptInstruction::Opcode::OP_MINUS_F64:
        mDataStack.push_f64( -( mDataStack.pop_f64() ) );
      break;

      case ScriptInstruction::Opcode::OP_BOOL_NOT_I64:
        mDataStack.push_i64( ( mDataStack.pop_i64() ) ? 0 : 1 );
      break;

      case ScriptInstruction::Opcode::OP_BIT_COMP_I64:
        mDataStack.push_i64( ~( mDataStack.pop_i64() ) );
      break;

      case ScriptInstruction::Opcode::OP_BIT_COMP_U64:
        mDataStack.push_u64( ~( mDataStack.pop_u64() ) );
      break;

      case ScriptInstruction::Opcode::OP_EE_STRING:
        mDataStack.push_i64( ( *( mDataStack.pop_string() ) == *( mDataStack.pop_string() ) ) ? 1 : 0 );
      break;

      case ScriptInstruction::Opcode::OP_EE_I64:
        mDataStack.push_i64( ( mDataStack.pop_i64() == mDataStack.pop_i64() ) ? 1 : 0 );
      break;

      case ScriptInstruction::Opcode::OP_EE_U64:
        mDataStack.push_i64( ( mDataStack.pop_u64() == mDataStack.pop_u64() ) ? 1 : 0 );
      break;

      case ScriptInstruction::Opcode::OP_EE_F64:
        mDataStack.push_i64( ( mDataStack.pop_f64() == mDataStack.pop_f64() ) ? 1 : 0 );
      break;

      case ScriptInstruction::Opcode::OP_NE_STRING:
        mDataStack.push_i64( ( *( mDataStack.pop_string() ) != *( mDataStack.pop_string() ) ) ? 1 : 0 );
      break;

      case ScriptInstruction::Opcode::OP_NE_I64:
        mDataStack.push_i64( ( mDataStack.pop_i64() != mDataStack.pop_i64() ) ? 1 : 0 );
      break;

      case ScriptInstruction::Opcode::OP_NE_U64:
        mDataStack.push_i64( ( mDataStack.pop_u64() != mDataStack.pop_u64() ) ? 1 : 0 );
      break;

      case ScriptInstruction::Opcode::OP_NE_F64:
        mDataStack.push_i64( ( mDataStack.pop_f64() != mDataStack.pop_f64() ) ? 1 : 0 );
      break;

      case ScriptInstruction::Opcode::OP_GT_STRING:
      {
        const crunch_string *b = mDataStack.pop_string();
        const crunch_string *a = mDataStack.pop_string();
        mDataStack.push_i64( ( *a > *b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_GT_I64:
      {
        crunch_i64 b = mDataStack.pop_i64();
        crunch_i64 a = mDataStack.pop_i64();
        mDataStack.push_i64( ( a > b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_GT_U64:
      {
        crunch_u64 b = mDataStack.pop_u64();
        crunch_u64 a = mDataStack.pop_u64();
        mDataStack.push_i64( ( a > b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_GT_F64:
      {
        crunch_f64 b = mDataStack.pop_f64();
        crunch_f64 a = mDataStack.pop_f64();
        mDataStack.push_i64( ( a > b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_GE_STRING:
      {
        const crunch_string *b = mDataStack.pop_string();
        const crunch_string *a = mDataStack.pop_string();
        mDataStack.push_i64( ( *a >= *b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_GE_I64:
      {
        crunch_i64 b = mDataStack.pop_i64();
        crunch_i64 a = mDataStack.pop_i64();
        mDataStack.push_i64( ( a >= b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_GE_U64:
      {
        crunch_u64 b = mDataStack.pop_u64();
        crunch_u64 a = mDataStack.pop_u64();
        mDataStack.push_i64( ( a >= b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_GE_F64:
      {
        crunch_f64 b = mDataStack.pop_f64();
        crunch_f64 a = mDataStack.pop_f64();
        mDataStack.push_i64( ( a >= b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_LT_STRING:
      {
        const crunch_string *b = mDataStack.pop_string();
        const crunch_string *a = mDataStack.pop_string();
        mDataStack.push_i64( ( *a < *b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_LT_I64:
      {
        crunch_i64 b = mDataStack.pop_i64();
        crunch_i64 a = mDataStack.pop_i64();
        mDataStack.push_i64( ( a < b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_LT_U64:
      {
        crunch_u64 b = mDataStack.pop_u64();
        crunch_u64 a = mDataStack.pop_u64();
        mDataStack.push_i64( ( a < b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_LT_F64:
      {
        crunch_f64 b = mDataStack.pop_f64();
        crunch_f64 a = mDataStack.pop_f64();
        mDataStack.push_i64( ( a < b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_LE_STRING:
      {
        const crunch_string *b = mDataStack.pop_string();
        const crunch_string *a = mDataStack.pop_string();
        mDataStack.push_i64( ( *a <= *b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_LE_I64:
      {
        crunch_i64 b = mDataStack.pop_i64();
        crunch_i64 a = mDataStack.pop_i64();
        mDataStack.push_i64( ( a <= b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_LE_U64:
      {
        crunch_u64 b = mDataStack.pop_u64();
        crunch_u64 a = mDataStack.pop_u64();
        mDataStack.push_i64( ( a <= b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_LE_F64:
      {
        crunch_f64 b = mDataStack.pop_f64();
        crunch_f64 a = mDataStack.pop_f64();
        mDataStack.push_i64( ( a <= b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_PLUS_I64:
        mDataStack.push_i64( mDataStack.pop_i64() + mDataStack.pop_i64() );
      break;

      case ScriptInstruction::Opcode::OP_PLUS_U64:
        mDataStack.push_u64( mDataStack.pop_u64() + mDataStack.pop_u64() );
      break;

      case ScriptInstruction::Opcode::OP_PLUS_F64:
        mDataStack.push_f64( mDataStack.pop_f64() + mDataStack.pop_f64() );
      break;

      case ScriptInstruction::Opcode::OP_SUBTRACT_I64:
      {
        crunch_i64 b = mDataStack.pop_i64();
        crunch_i64 a = mDataStack.pop_i64();
        mDataStack.push_i64( a - b );
      }
      break;

      case ScriptInstruction::Opcode::OP_SUBTRACT_U64:
      {
        crunch_u64 b = mDataStack.pop_u64();
        crunch_u64 a = mDataStack.pop_u64();
        mDataStack.push_u64( a - b );
      }
      break;

      case ScriptInstruction::Opcode::OP_SUBTRACT_F64:
      {
        crunch_f64 b = mDataStack.pop_f64();
        crunch_f64 a = mDataStack.pop_f64();
        mDataStack.push_f64( a - b );
      }
      break;

      case ScriptInstruction::Opcode::OP_MULTIPLY_I64:
        mDataStack.push_i64( mDataStack.pop_i64() * mDataStack.pop_i64() );
      break;

      case ScriptInstruction::Opcode::OP_MULTIPLY_U64:
        mDataStack.push_u64( mDataStack.pop_u64() * mDataStack.pop_u64() );
      break;

      case ScriptInstruction::Opcode::OP_MULTIPLY_F64:
        mDataStack.push_f64( mDataStack.pop_f64() * mDataStack.pop_f64() );
      break;

      case ScriptInstruction::Opcode::OP_DIVIDE_I64:
      {
        crunch_i64 b = mDataStack.pop_i64();
        crunch_i64 a = mDataStack.pop_i64();
        if ( b == 0 ) { EXIT_PROCESSING( EXIT_DIVIDE_BY_ZERO ); }
        mDataStack.push_i64( a / b );
      }
      break;

      case ScriptInstruction::Opcode::OP_DIVIDE_U64:
      {
        crunch_u64 b = mDataStack.pop_u64();
        crunch_u64 a = mDataStack.pop_u64();
        if ( b == 0 ) { EXIT_PROCESSING( EXIT_DIVIDE_BY_ZERO ); }
        mDataStack.push_u64( a / b );
      }
      break;

      case ScriptInstruction::Opcode::OP_DIVIDE_F64:
      {
        crunch_f64 b = mDataStack.pop_f64();
        crunch_f64 a = mDataStack.pop_f64();
        if ( b > -0.0000001f && b < 0.0000001f ) { EXIT_PROCESSING( EXIT_DIVIDE_BY_ZERO ); }
        mDataStack.push_f64( a / b );
      }
      break;

      case ScriptInstruction::Opcode::OP_BOOL_AND_I64:
      {
        // have to pop both separate, to avoid short-circuit &&
        crunch_i64 b = mDataStack.pop_i64();
        crunch_i64 a = mDataStack.pop_i64();
        mDataStack.push_i64( ( a && b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_BOOL_AND_U64:
      {
        // have to pop both separate, to avoid short-circuit &&
        crunch_u64 b = mDataStack.pop_u64();
        crunch_u64 a = mDataStack.pop_u64();
        mDataStack.push_i64( ( a && b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_BOOL_AND_F64:
      {
        // have to pop both separate, to avoid short-circuit &&
        crunch_i64 b = crunch_i64( mDataStack.pop_f64() );
        crunch_i64 a = crunch_i64( mDataStack.pop_f64() );
        mDataStack.push_i64( ( a && b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_BOOL_OR_I64:
      {
        // have to pop both separate, to avoid short-circuit ||
        crunch_i64 b = mDataStack.pop_i64();
        crunch_i64 a = mDataStack.pop_i64();
        mDataStack.push_i64( ( a || b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_BOOL_OR_U64:
      {
        // have to pop both separate, to avoid short-circuit ||
        crunch_u64 b = mDataStack.pop_u64();
        crunch_u64 a = mDataStack.pop_u64();
        mDataStack.push_i64( ( a || b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_BOOL_OR_F64:
      {
        // have to pop both separate, to avoid short-circuit ||
        crunch_i64 b = crunch_i64( mDataStack.pop_f64() );
        crunch_i64 a = crunch_i64( mDataStack.pop_f64() );
        mDataStack.push_i64( ( a || b ) ? 1 : 0 );
      }
      break;

      case ScriptInstruction::Opcode::OP_BIT_AND_I64:
        mDataStack.push_i64( mDataStack.pop_i64() & mDataStack.pop_i64() );
      break;

      case ScriptInstruction::Opcode::OP_BIT_AND_U64:
        mDataStack.push_u64( mDataStack.pop_u64() & mDataStack.pop_u64() );
      break;

      case ScriptInstruction::Opcode::OP_BIT_OR_I64:
        mDataStack.push_i64( mDataStack.pop_i64() | mDataStack.pop_i64() );
      break;

      case ScriptInstruction::Opcode::OP_BIT_OR_U64:
        mDataStack.push_u64( mDataStack.pop_u64() | mDataStack.pop_u64() );
      break;

      case ScriptInstruction::Opcode::OP_STRING_TO_I64:
        mDataStack.push_i64( crunch::core::String::to_i64( *( mDataStack.pop_string() ) ) );
      break;

      case ScriptInstruction::Opcode::OP_U64_TO_I64:
        mDataStack.push_i64( crunch_i64( mDataStack.pop_u64() ) );
      break;

      case ScriptInstruction::Opcode::OP_F64_TO_I64:
        mDataStack.push_i64( crunch_i64( mDataStack.pop_f64() ) );
      break;

      case ScriptInstruction::Opcode::OP_STRING_TO_U64:
        mDataStack.push_u64( crunch::core::String::to_u64( *( mDataStack.pop_string() ) ) );
      break;

      case ScriptInstruction::Opcode::OP_I64_TO_U64:
        mDataStack.push_u64( crunch_u64( mDataStack.pop_i64() ) );
      break;

      case ScriptInstruction::Opcode::OP_F64_TO_U64:
        mDataStack.push_u64( crunch_u64( mDataStack.pop_f64() ) );
      break;

      case ScriptInstruction::Opcode::OP_STRING_TO_F64:
        mDataStack.push_f64( crunch::core::String::to_f64( *( mDataStack.pop_string() ) ) );
      break;

      case ScriptInstruction::Opcode::OP_I64_TO_F64:
        mDataStack.push_f64( crunch_f64( mDataStack.pop_i64() ) );
      break;

      case ScriptInstruction::Opcode::OP_U64_TO_F64:
        mDataStack.push_f64( crunch_f64( mDataStack.pop_u64() ) );
      break;

      case ScriptInstruction::Opcode::OP_LIB_FUNC_CALLX:
      {
        BYTE_CODE_BUFFER_ARG_LIB_FUNC_PTR( libFunction );

        uint32_t parametersDataStackSize = libFunction->getParametersDataStackSize() + uint32_t( libFunction->getOwner()->getDataStackSize() );
        char result[ScriptTypes::TYPE_MAX_SIZE_BYTES];

        #if defined _CRUNCH_DEBUG
        uint64_t debugOffset = mDataStack.getOffset();
        #endif

        libFunction->entry( mExecutable, mDataStack.getSPOffset( parametersDataStackSize ), &( result[0] ) );

        mDataStack.pop( parametersDataStackSize );

        if ( libFunction->getReturnTypeDataStackSize() > 0 )
          mDataStack.push( &( result[0] ), libFunction->getReturnTypeDataStackSize() );
      }
      break;

      case ScriptInstruction::Opcode::OP_LIB_GFUNC_CALLX:
      {
        BYTE_CODE_BUFFER_ARG_LIB_FUNC_PTR( libFunction );

        uint32_t parametersDataStackSize = libFunction->getParametersDataStackSize();
        char result[ScriptTypes::TYPE_MAX_SIZE_BYTES];

        #if defined _CRUNCH_DEBUG
        uint64_t debugOffset = mDataStack.getOffset();
        #endif

        libFunction->entry( mExecutable, mDataStack.getSPOffset( parametersDataStackSize ), &( result[0] ) );

        mDataStack.pop( parametersDataStackSize );

        if ( libFunction->getReturnTypeDataStackSize() > 0 )
          mDataStack.push( &( result[0] ), libFunction->getReturnTypeDataStackSize() );
      }
      break;

      case ScriptInstruction::Opcode::OP_SO_FUNC_CALL:
        // TODO
      break;

      case ScriptInstruction::Opcode::OP_SO_GFUNC_CALL:
      {
        BYTE_CODE_INSTRUCTION_ARG32( offset );
        BYTE_CODE_BUFFER_ARGU64( paramsStackSize );

        jumpStack.push_back( FunctionJumpInfo( pc, dataStackOffset ) );

        pc = &( byteCode[offset] );

        // all the parameters are already pushed to the stack
        dataStackOffset = uint32_t( mDataStack.getOffset() - paramsStackSize );
      }
      break;

      case ScriptInstruction::Opcode::OP_BRANCH: // fall-thru on true, jump for false
      {
        BYTE_CODE_INSTRUCTION_ARG32( falseOffset );

        if ( mDataStack.pop_i64() == 0 )
          pc = &( byteCode[falseOffset] );
      }
      break;

      case ScriptInstruction::Opcode::OP_BRANCHF: // fall-thru on false, jump for true
      {
        BYTE_CODE_INSTRUCTION_ARG32( endIfOffset );

        if ( mDataStack.pop_i64() != 0 )
          pc = &( byteCode[endIfOffset] );
      }
      break;

      case ScriptInstruction::Opcode::OP_JUMP:
      {
        BYTE_CODE_INSTRUCTION_ARG32( offset );
        pc = &( byteCode[offset] );
      }
      break;

      case ScriptInstruction::Opcode::OP_SWITCH_JUMP_TABLE:
      {
        crunch_i64 switchExpressionValue = mDataStack.pop_i64();

        BYTE_CODE_BUFFER_ARGI64( arrayBaseValue );
        BYTE_CODE_BUFFER_ARGI64( jumpTableSize );
        BYTE_CODE_BUFFER_ARGU64( defaultCaseOffset );
        BYTE_CODE_BUFFER_ARGU64_ARRAY( jumpTable, jumpTableSize );

        crunch_i64 jumpTableIndex = switchExpressionValue - arrayBaseValue;

        if ( jumpTableIndex >= 0 && jumpTableIndex < jumpTableSize )
          pc = &( byteCode[jumpTable[jumpTableIndex]] );
        else
          pc = &( byteCode[defaultCaseOffset] );
      }
      break;

      default:
        EXIT_PROCESSING( EXIT_BAD_OPCODE );
      break;
    }
  }

  #undef EXIT_PROCESSING

  #undef BYTE_CODE_INSTRUCTION_ARG32
  #undef BYTE_CODE_INSTRUCTION_ARG32_OFFSET
  #undef BYTE_CODE_BUFFER_ARGU64
  #undef BYTE_CODE_BUFFER_ARGI64
  #undef BYTE_CODE_BUFFER_ARGF64
  #undef BYTE_CODE_BUFFER_ARGV3
  #undef BYTE_CODE_BUFFER_ARGPTR
  #undef BYTE_CODE_BUFFER_ARG_LIB_FUNC_PTR
}

} }
