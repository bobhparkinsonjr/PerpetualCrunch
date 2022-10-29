#if !defined ( CORE_GLOBAL_H )
#define CORE_GLOBAL_H

#include "core_platform.h"

#include <string>
#include <vector>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <map>

template < class Arg1, class Arg2, class Result >
struct binary_function
{
  using first_argument_type = Arg1;
  using second_argument_type = Arg2;
  using result_type = Result;
};

#if defined _CRUNCH_WINDOWS

	#include <math.h>
	#include <limits.h>
	#include <float.h>
	#include <windows.h>
	#include "core_qsort.h"

#elif defined _CRUNCH_ANDROID

  #include <ctype.h>
  #include <math.h>
  #include <limits.h>
  #include <float.h>
  #include <unistd.h>
  #include "core_qsort.h"

	typedef unsigned int DWORD;
	typedef unsigned short WORD;
	
	#if defined __x86_64__ || defined AARCH64
	typedef unsigned long long INT_PTR;
	#else
	typedef unsigned int INT_PTR;
	#endif
	
#else
	#error unknown platform
#endif

#if !defined _CRUNCH_WINDOWS
inline int _vscprintf( const char * format, va_list pargs )
{ 
  int retval; 
  va_list argcopy; 
    
  va_copy( argcopy, pargs );
    
  retval = vsnprintf( nullptr, 0, format, argcopy ); 
  va_end( argcopy ); 
  return retval; 
}
#endif

#include "core_debug.h"

#include "core_string.h"
#include "core_wide_string.h"
#include "core_fixed_string.h"
#include "core_file_string.h"
#include "core_wide_file_string.h"

#include "core_file_tools.h"

#include "core_thread.h"

#include "core_binary_buffer.h"

#endif
