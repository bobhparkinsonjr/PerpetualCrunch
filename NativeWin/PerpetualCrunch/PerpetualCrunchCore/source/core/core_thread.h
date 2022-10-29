#if !defined ( CORE_THREAD_H )
#define CORE_THREAD_H

/*

  // given thread function:

    CORE_THREAD_ENTRY_POINT( threadEntryPointFunction )
    {
  
      // code

      CORE_THREAD_EXIT( 0 );
    }

  // simple use case:

    void foo()
    {
      CoreThread::create( threadEntryPointFunction );
    }

  // wait on thread case:

    void foo2()
    {
      CoreThread t;

      if ( t.start( threadEntryPointFunction ) )
      {
        t.wait(); // blocks until thread exits
      }
    }

*/

#if defined _CRUNCH_WINDOWS

  // for stand-alone use
  #define CORE_THREAD_ENTRY_POINT( _function_name ) static unsigned __stdcall _function_name ( void *lpParam )

  // for class method use
  #define CORE_THREAD_ENTRY_POINT_HEADER( _function_name ) static unsigned __stdcall _function_name ( void *lpParam )
  #define CORE_THREAD_ENTRY_POINT_BODY( _class_name, _function_name ) unsigned __stdcall _class_name :: _function_name ( void *lpParam )

  #define CORE_THREAD_EXIT( _exit_code ) return _exit_code
  
  #define coreSleep( _ms ) Sleep( _ms )
  
#else

  #include <pthread.h>
  
  // for stand-alone use
  #define CORE_THREAD_ENTRY_POINT( _function_name ) static void* _function_name ( void *lpParam )

  // for class method use
  #define CORE_THREAD_ENTRY_POINT_HEADER( _function_name ) static void* _function_name ( void *lpParam )
  #define CORE_THREAD_ENTRY_POINT_BODY( _class_name, _function_name ) void* _class_name :: _function_name ( void *lpParam )
  
  #if defined _CRUNCH_LINUX
  #define CORE_THREAD_EXIT( _exit_code ) pthread_exit( nullptr ); return _exit_code
  #else
  #define CORE_THREAD_EXIT( _exit_code ) return _exit_code
  #endif
  
  #define coreSleep( _ms ) usleep( _ms * 1000 )

#endif

namespace crunch { namespace core {

class CoreThread
{
	public:
	  #if defined _CRUNCH_WINDOWS
		typedef unsigned (__stdcall* threadEntryPointCallbackType)( void *userData );
		#else
		typedef void* (*threadEntryPointCallbackType)( void *userData );
		#endif

		#if defined _CRUNCH_WINDOWS
		typedef HANDLE threadHandleType;
		#else
		typedef pthread_t threadHandleType;
		#endif

	public:
		CoreThread();
		~CoreThread();

  private:
    CoreThread( const CoreThread& other );
    CoreThread& operator = ( const CoreThread& other );
		
	public:
    // must call wait afterward at some point, until wait returns true not all
    //   resources will have been released
		bool start( threadEntryPointCallbackType entryPoint, void *userData = nullptr );
		
    // if timeoutMS is 0, it will check the thread only once and then return, otherwise
    //   it will wait only until the interval has elapsed
    // to wait forever, set timeoutMS to 0xFFFFFFFF
    // returns true if the thread has closed (if closed, isCreated() will then return false if called)
		bool wait( DWORD timeoutMS = 0xFFFFFFFF );

    core_forceinline bool isCreated() const { return mCreated; }

  public:
    // this will create a thread, cannot call complete
    // true on success
    static bool create( threadEntryPointCallbackType entryPoint, void *userData = nullptr );

    // if true returned, must call complete on theThread
    static bool create( threadEntryPointCallbackType entryPoint, void *entryPointUserData, threadHandleType *theThread );

    // if the given thread has completed running, it will be closed and true is returned, otherwise false is returned
    // if timeoutMS is 0, it will check the thread only once and then return, otherwise waits for interval to elapse
    // to wait forever, set timeoutMS to 0xFFFFFFFF
    static bool complete( threadHandleType theThread, DWORD timeoutMS = 0xFFFFFFFF );

	private:
    threadHandleType mThreadHandle;
    bool mCreated;
	
};

} }

#endif
