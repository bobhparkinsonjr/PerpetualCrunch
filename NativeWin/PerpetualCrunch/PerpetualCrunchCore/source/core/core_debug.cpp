
#include "core_global.h"

#include "core_debug.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#if ( defined _CRUNCH_DEBUG || defined _DEBUG ) && !( defined _CRUNCH_FASTDEBUG )

#if defined _WINDOWS

bool internalAssertHandler( const char *filename, unsigned int line, const char *msg, bool *ignoreAlways )
{
	corePrint( "%s(%u): assert: %s", filename, line, msg );
	
	const int BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE];
	
	_snprintf_s( buffer, BUFFER_SIZE - 1, BUFFER_SIZE - 2, "%s(%u): assert: %s\nPress Yes to debug, No to skip, Cancel to ignore always.", filename, line, msg );
	buffer[BUFFER_SIZE - 1] = 0;
	
	int res = ::MessageBoxA( nullptr, buffer, "Assert", MB_YESNOCANCEL );
	
	if ( res == IDYES )
		return true;
		
	if ( res == IDCANCEL )
		*ignoreAlways = true;
	
	return false;
}

#else

bool internalAssertHandler( const char *filename, unsigned int line, const char *msg, bool *ignoreAlways )
{
	return false;
}

#endif

#endif
