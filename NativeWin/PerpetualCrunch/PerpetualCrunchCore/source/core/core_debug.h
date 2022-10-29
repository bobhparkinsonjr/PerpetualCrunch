#if !defined ( CORE_DEBUG_H )
#define CORE_DEBUG_H

#include <string>

#include "core_platform.h"

#define LINESTR3( _line ) #_line
#define LINESTR2( _line ) LINESTR3( _line )
#define __LINESTR__ LINESTR2( __LINE__ )

#if !defined NDEBUG
  #if !defined _DEBUG
    #define _DEBUG
  #endif
    
  #if !defined _CRUNCH_DEBUG
    #define _CRUNCH_DEBUG
  #endif
#endif

class CoreDebugPrint
{
	private:
		CoreDebugPrint();
	
	public:
		CoreDebugPrint( const char *fmt, ... )
		{
			#define WORK_SIZE 8192
			char work[WORK_SIZE];
				
			va_list args;
			va_start( args, fmt );
				
			#if defined _CRUNCH_WINDOWS
			vsnprintf_s( work, WORK_SIZE, _TRUNCATE, fmt, args );
			#else
			vsnprintf( work, WORK_SIZE - 1, fmt, args );
			work[WORK_SIZE - 1] = 0;
			#endif
				
			#if defined _CRUNCH_WINDOWS
			OutputDebugStringA( work );
			OutputDebugStringA( "\n" );
			#else
			printf( "%s", work );
			printf( "\n" );
			#endif

			#undef WORK_SIZE
		}
			
};

#if ( defined _CRUNCH_DEBUG || defined _DEBUG ) && !( defined _CRUNCH_FASTDEBUG )
	
	#if defined _CRUNCH_WINDOWS
    #if defined _M_IX86
      #define BREAKPOINT() { __asm { int 3 } }
    #else
      #define BREAKPOINT() __debugbreak()
    #endif
	#else
	  #define BREAKPOINT()
	#endif
	
	#if defined assert
		#undef assert
	#endif
	
	#if defined _WINDOWS

		extern bool internalAssertHandler( const char *filename, unsigned int line, const char *msg, bool *ignoreAlways );
		
		#if defined _M_IX86 && !( defined _M_CEE )

			#define assert( _cond ) \
				if ( !( _cond ) ) \
				{ \
					static bool ignoreAlways = false; \
					if ( !ignoreAlways && internalAssertHandler( __FILE__, __LINE__, #_cond, &ignoreAlways ) ) \
						__asm { int 3 } \
				}
				
		#else

			#define assert( _cond ) \
				if ( !( _cond ) ) \
				{ \
					static bool ignoreAlways = false; \
					if ( !ignoreAlways && internalAssertHandler( __FILE__, __LINE__, #_cond, &ignoreAlways ) ) \
						__debugbreak(); \
				}
				
		#endif

	#else
		#define assert( _cond ) if ( !( _cond ) ) { corePrint( __FILE__ "(" __LINESTR__ "): assert: " #_cond ); }
	#endif
	
	#define corePrint CoreDebugPrint
	
	#define _CRUNCH_DEBUG_DECLARE( _type, _var ) _type _var =
	
#else
	
	#define BREAKPOINT()
	
	#undef assert
	#define assert( _cond )
	
	#if defined( __GNUC__ )
		#define corePrint(...) (void)(0)
	#elif defined _CRUNCH_WINDOWS
	  #define corePrint (__noop)
	#else
	  #define corePrint
	#endif
	
	#define _CRUNCH_DEBUG_DECLARE( _type, _var )
	
#endif

#endif
