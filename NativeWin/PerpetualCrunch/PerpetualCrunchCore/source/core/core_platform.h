#if !defined ( CORE_PLATFORM_H )
#define CORE_PLATFORM_H

#if defined _CRUNCH_LINUX

  #if !defined BOOL
    #define BOOL char
  #endif
  
  #if !defined TRUE
    #define TRUE (1==1)
  #endif
  
  #if !defined FALSE
    #define FALSE (!TRUE)
  #endif
  
  typedef wchar_t WCHAR;
	
#elif defined _WINDOWS || defined _CRUNCH_WINDOWS || defined WIN32

	#if !defined _CRUNCH_WINDOWS
		#define _CRUNCH_WINDOWS
	#endif
	
#elif defined _CRUNCH_ANDROID || defined ANDROID || defined __ANDROID__

  #if !defined _CRUNCH_ANDROID
    #define _CRUNCH_ANDROID
  #endif
  
  #if !defined BOOL
    #define BOOL char
  #endif
  
  #if !defined TRUE
    #define TRUE (1==1)
  #endif
  
  #if !defined FALSE
    #define FALSE (!TRUE)
  #endif
  
  typedef wchar_t WCHAR;
  
#else
	#error missing platform
#endif

#if defined _CRUNCH_LINUX

  typedef unsigned long long core_u64;
  typedef long long core_i64;
  typedef long LONG; // 32-bit signed, always
  typedef unsigned char BYTE;
	typedef unsigned int DWORD;
	typedef unsigned short WORD;
	
	#if defined __x86_64__ || defined AARCH64
	typedef unsigned long long INT_PTR;
	#else
	typedef unsigned int INT_PTR;
	#endif

  #define _CORE_PATH_SEP_CH '/'
  #define _CORE_PATH_SEP_STR "/"
  #define _CORE_PATH_SEP_WCH L'/'
  #define _CORE_PATH_SEP_WSTR L"/"

  #define _CORE_INTEROP_CALLBACK

  #define core_forceinline inline

#elif defined _CRUNCH_WINDOWS

  typedef unsigned __int64 core_u64;
  typedef __int64 core_i64;

  #define _CORE_PATH_SEP_CH '\\'
  #define _CORE_PATH_SEP_STR "\\"
  #define _CORE_PATH_SEP_WCH L'\\'
  #define _CORE_PATH_SEP_WSTR L"\\"

  #define _CORE_INTEROP_CALLBACK __stdcall

  #define core_forceinline __forceinline

#elif defined _CRUNCH_ANDROID

  typedef unsigned long long core_u64;
  typedef long long core_i64;
  typedef long LONG; // 32-bit signed, always

  #define _CORE_PATH_SEP_CH '/'
  #define _CORE_PATH_SEP_STR "/"
  #define _CORE_PATH_SEP_WCH L'/'
  #define _CORE_PATH_SEP_WSTR L"/"

  #define _CORE_INTEROP_CALLBACK

  #define core_forceinline inline
  
#else
  #error missing platform
#endif

#endif
