
#include "core_global.h"

#include "core_mutex.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace core {

CoreMutex::CoreMutex() :
	mReady( false )
{
	// empty
}

CoreMutex::~CoreMutex()
{
	destroy();
}

void CoreMutex::create()
{
	if ( !mReady )
	{
	  #if defined _CRUNCH_WINDOWS
		InitializeCriticalSection( &mCriticalSection );
		#else
		pthread_mutexattr_t attr;
		pthread_mutexattr_init( &attr );
		pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );
		pthread_mutex_init( &mCriticalSection, &attr );
		pthread_mutexattr_destroy( &attr );
		#endif
		
		mReady = true;
	}
}

void CoreMutex::destroy()
{
	if ( mReady )
	{
	  #if defined _CRUNCH_WINDOWS
		DeleteCriticalSection( &mCriticalSection );
		#else
		pthread_mutex_destroy( &mCriticalSection );
		#endif
		
		mReady = false;
	}
}

bool CoreMutex::lock()
{
	assert( mReady );
	
	#if defined _CRUNCH_WINDOWS
	EnterCriticalSection( &mCriticalSection );
	#else
	pthread_mutex_lock( &mCriticalSection );
	#endif
	
	return true;
}

bool CoreMutex::trylock()
{
	assert( mReady );
	
	#if defined _CRUNCH_WINDOWS
	return ( TryEnterCriticalSection( &mCriticalSection ) != 0 );
	#else
	return ( pthread_mutex_trylock( &mCriticalSection ) == 0 ); 
	#endif
}

void CoreMutex::unlock()
{
	assert( mReady );
	
	#if defined _CRUNCH_WINDOWS
	LeaveCriticalSection( &mCriticalSection );
	#else
	pthread_mutex_unlock( &mCriticalSection );
	#endif
}

} }
