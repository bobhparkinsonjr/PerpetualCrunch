#if !defined ( CORE_MUTEX_H )
#define CORE_MUTEX_H

#if !defined _CRUNCH_WINDOWS
  #include <pthread.h>
#endif

namespace crunch { namespace core {

class CoreMutex
{
	public:
		CoreMutex();
		~CoreMutex();
		
	private:
		CoreMutex( const CoreMutex& other );
		CoreMutex& operator = ( const CoreMutex& other );
		
	public:
		void create();
		void destroy();
		
		bool lock();
		bool trylock();

		void unlock();

		core_forceinline bool isReady() const { return mReady; }
		
	private:
	  #if defined _CRUNCH_WINDOWS
		CRITICAL_SECTION mCriticalSection;
		#else
		pthread_mutex_t mCriticalSection;
		#endif
		
		bool mReady;
		
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class ScopedLock
{
	public:
		ScopedLock( CoreMutex *value ) :
			mCoreMutex( value )
		{
			assert( mCoreMutex );
			
			_CRUNCH_DEBUG_DECLARE( bool, lockResult )
			mCoreMutex->lock();
			assert( lockResult );
		}
		
		~ScopedLock()
		{
			mCoreMutex->unlock();
		}
		
	private:
		ScopedLock( const ScopedLock& other );
		ScopedLock& operator = ( const ScopedLock& other );
		
	private:
		CoreMutex *mCoreMutex;
		
};

} }

#define scoped_lock( _mutex_ptr ) crunch::core::ScopedLock _ScopedLock_##__LINE__( (_mutex_ptr) )

#endif
