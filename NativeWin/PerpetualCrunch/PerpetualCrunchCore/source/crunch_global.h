#if !defined ( CRUNCH_GLOBAL_H )
#define CRUNCH_GLOBAL_H

#define CRUNCH_LIBRARY_ENABLE_DOCUMENTATION

#include "core/core_platform.h"

#include <string>
#include <vector>
#include <set>
#include <list>

#if defined _CRUNCH_WINDOWS

	#include <math.h>
	#include <limits.h>
	#include <float.h>
	#include <windows.h>

	#include "core/core_qsort.h"

#elif defined _CRUNCH_ANDROID

  #include <ctype.h>
  #include <math.h>
  #include <limits.h>
  #include <float.h>
  #include <unistd.h>
  #include "core/core_qsort.h"

	typedef unsigned int DWORD;
	typedef unsigned short WORD;
	
	#if defined __x86_64__
	typedef unsigned long long INT_PTR;
	#else
	typedef unsigned int INT_PTR;
	#endif
	
#elif defined _CRUNCH_LINUX

  #include <ctype.h>
  #include <math.h>
  #include <limits.h>
  #include <float.h>
  #include <unistd.h>
  #include <stdlib.h>
  #include <stdio.h>
  #include <cstdarg>
  #include <cstdio>
  #include <cstring>
  #include "core/core_qsort.h"

	typedef unsigned int DWORD;
	typedef unsigned short WORD;
	
	#if defined __x86_64__
	typedef unsigned long long INT_PTR;
	#else
	typedef unsigned int INT_PTR;
	#endif
	
#else
	#error unknown platform
#endif

#include "core/core_headers.h"

#endif
