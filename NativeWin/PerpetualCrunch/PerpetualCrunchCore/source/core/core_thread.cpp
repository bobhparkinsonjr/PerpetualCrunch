
#include "core_global.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#if defined _CRUNCH_WINDOWS
  #include <process.h>
#else
  #include <errno.h>
#endif

#if defined _CRUNCH_LINUX
  #include <signal.h>
#endif

#include "core_environment_variable.h"

#include "core_thread.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace core {

CoreThread::CoreThread() :
  mCreated( false )
{
  // empty
}

CoreThread::~CoreThread()
{
  // empty
}
		
bool CoreThread::start( threadEntryPointCallbackType entryPoint, void *userData )
{
  if ( mCreated )
    return true;

  mCreated = create( entryPoint, userData, &mThreadHandle );

  return mCreated;
}
		
bool CoreThread::wait( DWORD timeoutMS )
{
  if ( !mCreated )
    return true;

  mCreated = !( complete( mThreadHandle, timeoutMS ) );

  return ( !mCreated );
}

bool CoreThread::create( threadEntryPointCallbackType entryPoint, void *userData )
{
  threadHandleType t;
  bool result = create( entryPoint, userData, &t );

  if ( result )
  {
    #if defined _CRUNCH_WINDOWS

    CloseHandle( t );

    #else

    #if defined _CRUNCH_LINUX // might be safe on other platforms
    pthread_detach( t );
    #endif

    #endif
  }

  return result;
}

bool CoreThread::create( threadEntryPointCallbackType entryPoint, void *entryPointUserData, threadHandleType *theThread )
{
  if ( theThread == nullptr )
    return false;

	#if defined _CRUNCH_WINDOWS
		
	*theThread = (threadHandleType)( _beginthreadex( nullptr, 0, entryPoint, entryPointUserData, 0, nullptr ) );
								   
	if ( *theThread == 0 )
    return false;
		
	#else
		
	int createResult = pthread_create( theThread, nullptr, entryPoint, entryPointUserData );

  #if defined _CRUNCH_DEBUG
  if ( createResult !=  0 )
  {
    static int sCreateResultErrorReportCounter = 0;
    if ( sCreateResultErrorReportCounter < 100 )
    {
      ++sCreateResultErrorReportCounter;
      
      switch ( createResult )
      {
        case EAGAIN:
          coreLog( "pthread_create error EAGAIN" );
        break;

        case EINVAL:
          coreLog( "pthread_create error EINVAL" );
        break;

        case EPERM:
          coreLog( "pthread_create error EPERM" );
        break;

        default:
          coreLog( "unknown pthread_create error" );
        break;
      }
    }
  }
  #endif

  if ( createResult != 0 )
    return false; // can't create a thread right now, so try again later

  /*

    docs:

      EAGAIN Insufficient resources to create another thread.
      EAGAIN A system-imposed limit on the number of threads was
        encountered.  There are a number of limits that may trigger
        this error: the RLIMIT_NPROC soft resource limit (set via
        setrlimit(2)), which limits the number of processes and
        threads for a real user ID, was reached; the kernel's system-
        wide limit on the number of processes and threads,
        /proc/sys/kernel/threads-max, was reached (see proc(5)); or
        the maximum number of PIDs, /proc/sys/kernel/pid_max, was
        reached (see proc(5)).

      EINVAL Invalid settings in attr.

      EPERM  No

  */
		
	#endif

  return true;
}

bool CoreThread::complete( threadHandleType theThread, DWORD timeoutMS )
{
  bool waitForever = ( timeoutMS == 0xFFFFFFFF );

	#if defined _CRUNCH_WINDOWS
		
	if ( WaitForSingleObject( theThread, waitForever ? INFINITE : timeoutMS ) == WAIT_OBJECT_0 )
	{
		CloseHandle( theThread );
    return true;
	}
		
	#else

  DWORD startTime = GetTickCount();
		
  while ( true )
  {
	  if ( pthread_kill( theThread, 0 ) == ESRCH )
    {
      #if defined _CRUNCH_LINUX
      // this really shouldn't be needed, and should always just return ESRCH
      // also, this breaks the OSX platform, we may also need to further restrict
      // on POSIX version
      pthread_join( theThread, nullptr );
      #endif

		  return true;
    }

    if ( !waitForever )
    {
      // see comment above regarding problem with pthread_join
      if ( timeoutMS == 0 )
        break;

      if ( ( GetTickCount() - startTime ) > timeoutMS )
        break;
    }

    coreSleep( 1 );
  }
		
	#endif

  return false;
}

} }
