#if !defined ( CRUNCH_SCRIPT_THREAD_H )
#define CRUNCH_SCRIPT_THREAD_H

#include "../core/core_thread.h"

#include "crunch_script_data_stack.h"

namespace crunch { namespace language {

class ScriptVM;
class ScriptExecutable;

class ScriptThread final
{
  public:
    typedef enum
    {
      EXIT_NONE = -1,

      EXIT_NORMAL,
      EXIT_BAD_OPCODE,
      EXIT_CANCELED,
      EXIT_DIVIDE_BY_ZERO,

      EXIT_TOTAL
    } ExitType;

  public:
    ScriptThread( ScriptVM *vm = nullptr, const ScriptExecutable *executable = nullptr );
    ~ScriptThread();

  public:
    void destroy();

    bool setRootFunction( const crunch::core::String& signature );
    bool setRootFunction( const crunch::core::String& name, const std::vector< ScriptTypes::ScriptTypeDescription >& parameterTypes );

  public:
    core_forceinline void push_string( const crunch_string *s ) { if ( !( isRunning() ) && mExitType == ExitType::EXIT_NONE ) mDataStack.push_string( s ); }
    core_forceinline void push_i64( crunch_i64 i ) { if ( !( isRunning() ) && mExitType == ExitType::EXIT_NONE ) mDataStack.push_i64( i ); }
    core_forceinline void push_u64( crunch_u64 u ) { if ( !( isRunning() ) && mExitType == ExitType::EXIT_NONE ) mDataStack.push_u64( u ); }
    core_forceinline void push_f64( crunch_f64 f ) { if ( !( isRunning() ) && mExitType == ExitType::EXIT_NONE ) mDataStack.push_f64( f ); }
    core_forceinline void push_f64v3( crunch_f64 *v ) { if ( !( isRunning() ) && mExitType == ExitType::EXIT_NONE ) mDataStack.push_f64v3( v ); }
    core_forceinline void push_f64v4( crunch_f64 *v ) { if ( !( isRunning() ) && mExitType == ExitType::EXIT_NONE ) mDataStack.push_f64v4( v ); }
    core_forceinline void push_ptr( void *ptr ) { if ( !( isRunning() ) && mExitType == ExitType::EXIT_NONE ) mDataStack.push_ptr( ptr ); }

    bool read_f64( crunch_f64 *f );

  public:
    core_forceinline const ScriptVM* getVM() const { return mVM; }

    bool isRunning();

    // if waitComplete is true, then this will block until this thread is finished,
    //   otherwise it will return immediately
    // if runInline is true, then no new thread will be created, instead this "thread" will run
    //   on the caller's thread.  in this case, waitComplete is ignored as the function will block
    //   until all processing is complete
    bool run( bool waitComplete = true, bool runInline = false );

    core_forceinline ExitType getExitType() const { return mExitType; }

    // this will not block, this will cause this thread to exit early and set its exit type to EXIT_CANCELED
    //   if the thread is running
    void cancel();

  public:
    static crunch::core::String getExitTypeName( ExitType v );

  private:
    CORE_THREAD_ENTRY_POINT_HEADER( processingEntry );

    void processing();

  private:
    ScriptVM *mVM = nullptr;
    const ScriptExecutable *mExecutable = nullptr;

    ExitType mExitType = ExitType::EXIT_NONE;
    volatile bool mCancelRequested = false;

    ScriptDataStack mDataStack;

    crunch::core::String mRootFunctionSignature;
    unsigned int mRootFunctionOffset = 0;
    ScriptTypes::ScriptTypeDescription mRootFunctionReturnType;

    crunch::core::CoreThread mProcessingThread;

    struct FunctionJumpInfo
    {
      const char *mPC;
      uint32_t mDataStackOffset;

      FunctionJumpInfo( const char *pc = nullptr, uint32_t dataStackOffset = 0 ) :
        mPC( pc ),
        mDataStackOffset( dataStackOffset )
      {
        // empty
      }
    };

};

} }

#endif
