
// this will make crunch_popen/crunch_pclose just use the provided popen/pclose functions
// #define ENABLE_CRUNCH_STANDARD_POPEN_PCLOSE

#include "core_global.h"

#if defined _CRUNCH_WINDOWS

  #include <Shlwapi.h>
  #include <direct.h>

#else

  #include <sys/stat.h>

  #include <sys/param.h>
  #include <sys/wait.h>
  #include <sys/ioctl.h>
  #include <sys/resource.h>
  
  #include <fcntl.h>
  #include <signal.h>
  #include <errno.h>
  #include <unistd.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <paths.h>
  #include <dirent.h>

#endif

#include "core_file_tools.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#if defined _CRUNCH_LINUX
extern char **environ;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace core {

String FileTools::getFileExtension( const String& filename )
{
	int foundIndex = int( filename.rfind( "." ) );
	if ( foundIndex >= 0 )
		return filename.substr( foundIndex );
	
	return "";
}

String FileTools::getName( const String& filename )
{
	int slashIndex = int( filename.rfind( "\\" ) );
	if ( slashIndex < 0 )
		slashIndex = int( filename.rfind( "/" ) );

	int dotIndex = int( filename.rfind( "." ) );
	
	if ( slashIndex >= 0 && dotIndex > ( slashIndex + 1 ) )
		return filename.bsubstr( slashIndex + 1, dotIndex - 1 );
	
	if ( slashIndex < 0 && dotIndex > 0 )
		return filename.substr( 0, dotIndex );
	
	if ( slashIndex >= 0 )
		return filename.substr( slashIndex + 1 );
	
	return filename;
}

String FileTools::getFileDirectory( const String& filename )
{
	String retVal = filename;
	
	int foundIndex;
	
	foundIndex = int( retVal.rfind( "\\" ) );
	if ( foundIndex < 0 )
		foundIndex = int( retVal.rfind( "/" ) );
	
	if ( foundIndex < 0 )
		return "";
		
	return retVal.substr( 0, foundIndex + 1 );
}

bool FileTools::deleteFile( const String& path, bool verifyDelete )
{
  #if defined _CRUNCH_WINDOWS
  
	if ( DeleteFileA( path.c_str() ) != 0 )
	{
		if ( verifyDelete )
		{
			while ( fileExists( path ) )
			{
				coreSleep( 10 );
			}
		}

    return true;
	}
	
	#else
	
	if ( remove( path.c_str() ) == 0 )
	{
		if ( verifyDelete )
		{
			while ( fileExists( path ) )
			{
				crunchSleep( 10 );
			}
		}

    return true;
	}
	
	#endif
	
	return false;
}

bool FileTools::deleteFile( const WideString& path, bool verifyDelete )
{
	#if defined _CRUNCH_WINDOWS

	if ( DeleteFileW( path.wc_str() ) != 0 )
	{
		if ( verifyDelete )
		{
			while ( fileExists( path ) )
			{
				coreSleep( 10 );
			}
		}

    return true;
	}

	#else
	
  char *utf8String;
  int utf8StringSize;
  if ( path.getUtf8String( &utf8String, &utf8StringSize ) )
  {
    bool retVal = false;

    if ( remove( utf8String ) == 0 )
    {
      retVal = true;

      if ( verifyDelete )
      {
        while ( fileExists( path ) )
        {
          crunchSleep( 10 );
        }
      }
    }
    
    free( utf8String );

    return retVal;
	}

	#endif
	
	return false;
}

#if defined _CRUNCH_WINDOWS

static int deleteFolderSortCallback( const void *a, const void *b )
{
	String *lhs = *(String**)( a );
	String *rhs = *(String**)( b );

	return (int)( rhs->size() - lhs->size() );
}

#endif

bool FileTools::copyFile( const String& sourcePath, const String& destPath, bool overwriteOk )
{
	#if defined _CRUNCH_WINDOWS
	
	return ( CopyFileA( sourcePath.c_str(), destPath.c_str(), !overwriteOk ) != 0 );
	
	#else
	
	if ( !overwriteOk && fileExists( destPath ) )
	  return false;
	
	FILE *source = crunch_fopen( sourcePath.c_str(), "rb" );
	
	if ( source )
	{
		FILE *dest = crunch_fopen( destPath.c_str(), "wb" );
		
		if ( dest )
      return copyFile( source, dest );
		
		fclose( source );
	}
	
	return false;
	
	#endif
}

bool FileTools::copyFile( const WideString& sourcePath, const WideString& destPath, bool overwriteOk )
{
	#if defined _CRUNCH_WINDOWS
	
	return ( CopyFileW( sourcePath.wc_str(), destPath.wc_str(), !overwriteOk ) != 0 );
	
	#else
	
	if ( !overwriteOk && fileExists( destPath ) )
	  return false;
	
	FILE *source = crunch_fwopen( sourcePath.wc_str(), L"rb" );
	
	if ( source )
	{
		FILE *dest = crunch_fwopen( destPath.wc_str(), L"wb" );
		
		if ( dest )
      return copyFile( source, dest );
		
		fclose( source );
	}
	
	return false;
	
	#endif
}

bool FileTools::moveFile( const String& sourcePath, const String& destPath, bool overwriteOk )
{
  if ( copyFile( sourcePath, destPath, overwriteOk ) )
  {
    deleteFile( sourcePath );
    return true;
  }

  return false;
}

bool FileTools::moveFile( const WideString& sourcePath, const WideString& destPath, bool overwriteOk )
{
  if ( copyFile( sourcePath, destPath, overwriteOk ) )
  {
    deleteFile( sourcePath );
    return true;
  }

  return false;
}

bool FileTools::fileExists( const WideString& filename )
{
	if ( filename == L"" )
		return false;
	
	#if defined _CRUNCH_WINDOWS

	return ( PathFileExistsW( (LPCWSTR)( filename.wc_str() ) ) != FALSE );
	
	#else
	
  char *utf8String;
  int utf8StringSize;
  if ( filename.getUtf8String( &utf8String, &utf8StringSize ) )
  {
    struct stat buffer;
    
    bool retVal = ( stat( utf8String, &buffer ) == 0 );
    
    free( utf8String );
    
    return retVal;
	}
	
	return false;
	
	#endif
}

bool FileTools::getFileSize( const WideString& filename, unsigned int *fileSize )
{
	if ( fileSize == nullptr )
		return false;

	*fileSize = 0;
	
	FILE *source = core_fwopen( filename.wc_str(), L"rb" );
	if ( source != nullptr )
	{
		fseek( source, 0, SEEK_END );
		*fileSize = (unsigned int)( ftell( source ) );

		fclose( source );
		return true;
	}

	return false;
}

String FileTools::findFile( const String& findMe, const String& startFolder, bool recurse )
{
  #if defined _CRUNCH_WINDOWS

	std::list< String > folders;
	
	folders.push_back( startFolder );

	while ( !( folders.empty() ) )
	{
		String currentFolder = folders.front();
		folders.pop_front();
		
		if ( fileExists( WideString( currentFolder + findMe ) ) )
			return ( currentFolder + findMe );
			
		if ( !recurse )
			break;
		
		WIN32_FIND_DATAA myInfo;
		
		HANDLE myHandle = FindFirstFileA( (LPCSTR)( ( currentFolder + "*.*" ).c_str() ), &myInfo );
		
		if ( myHandle != INVALID_HANDLE_VALUE )
		{
			while ( true )
			{
				if ( ( myInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 )
				{
					String foundFile = myInfo.cFileName;
					if ( foundFile != "." && foundFile != ".." )
						folders.push_back( currentFolder + foundFile + "\\" );
				}
					
				if ( !( FindNextFileA( myHandle, &myInfo ) ) )
					break;
			}
			
			FindClose( myHandle );
		}
	}
	
	return "";

  #else

  return "";

  #endif
}

bool FileTools::createDirectory( const String& dir )
{
  #if defined _CRUNCH_WINDOWS

	int slashIndex = int( dir.find( "\\" ) );
	while ( slashIndex >= 0 )
	{
		slashIndex = int( dir.find( "\\", slashIndex + 1 ) );
		if ( slashIndex < 0 )
			break;
			
		String subDir = dir.substr( 0, slashIndex );
			
		CreateDirectoryA( (LPCSTR)( subDir.c_str() ), nullptr );
	}
	
	CreateDirectoryA( (LPCSTR)( dir.c_str() ), nullptr );

  return true;

  #elif defined _CRUNCH_LINUX

	int slashIndex = int( dir.find( _CRUNCH_PATH_SEP_STR ) );
	while ( slashIndex >= 0 )
	{
		slashIndex = int( dir.find( _CRUNCH_PATH_SEP_STR, slashIndex + 1 ) );
		if ( slashIndex < 0 )
			break;
			
		String subDir = dir.substr( 0, slashIndex );
			
    mkdir( subDir.c_str(), 0755 );
	}
	
  mkdir( dir.c_str(), 0755 );

  return true;

  #else

  return false;

  #endif
}

bool FileTools::createDirectoryW( const WideString& dir )
{
  #if defined _CRUNCH_WINDOWS

	int slashIndex = int( dir.find( L"\\" ) );
	while ( slashIndex >= 0 )
	{
		slashIndex = int( dir.find( L"\\", slashIndex + 1 ) );
		if ( slashIndex < 0 )
			break;
			
		WideString subDir = dir.substr( 0, slashIndex );
			
		CreateDirectoryW( (LPCWSTR)( subDir.wc_str() ), nullptr );
	}
	
	CreateDirectoryW( (LPCWSTR)( dir.wc_str() ), nullptr );

  return true;

  #elif defined _CRUNCH_LINUX

  return false;

  #else

  return false;

  #endif
}

bool FileTools::setCurrentWorkingDirectory( const WideString& dir )
{
  #if defined _CRUNCH_WINDOWS
  return ( SetCurrentDirectoryW( dir.wc_str() ) != 0 );
  #else
  return ( chdir( dir.c_str() ) == 0 );
  #endif
}

bool FileTools::getCurrentWorkingDirectory( WideString *dir )
{
  #if defined _CRUNCH_WINDOWS

  WCHAR buffer[MAX_PATH];
  if ( GetCurrentDirectoryW( MAX_PATH, buffer ) > 0 )
  {
    *dir = buffer;
    return true;
  }

  #else

  corePrint( "[ERROR] getCurrentWorkingDirectory not implemented for this platform" );

  #endif

  return false;
}

bool FileTools::getFileModifiedDateTimeInfo( const WideString& filePath, fileTimeInfo *info )
{
	assert( info );
	
	bool retVal = false;
	
	#if defined _CRUNCH_WINDOWS

	HANDLE myFileHandle = CreateFileW( (LPCWSTR)( filePath.wc_str() ),
	                                  GENERIC_READ,
                                      FILE_SHARE_READ,
                                      nullptr, // LPSECURITY_ATTRIBUTES lpSecurityAttributes
                                      OPEN_EXISTING,
                                      FILE_ATTRIBUTE_NORMAL,
                                      nullptr // HANDLE hTemplateFile
                                      );

	if ( myFileHandle == INVALID_HANDLE_VALUE )
		return false;

	FILETIME modifiedTime;

	if ( GetFileTime( myFileHandle, // handle to file
                      nullptr, // creation time
                      nullptr, // last access time
                      &modifiedTime // last write time
                      ) != 0 )
	{
		FILETIME localModifiedTime;
		if ( FileTimeToLocalFileTime( &modifiedTime, &localModifiedTime ) != 0 )
		{
			SYSTEMTIME modifiedSystemTime;

			if ( FileTimeToSystemTime( &localModifiedTime, &modifiedSystemTime ) != 0 )
			{
				info->mYear = modifiedSystemTime.wYear;
				info->mMonth = modifiedSystemTime.wMonth;
				info->mDayOfWeek = modifiedSystemTime.wDayOfWeek;
				info->mDay = modifiedSystemTime.wDay;
				info->mHour = modifiedSystemTime.wHour;
				info->mMinute = modifiedSystemTime.wMinute;
				info->mSecond = modifiedSystemTime.wSecond;
				info->mMilliseconds = modifiedSystemTime.wMilliseconds;

				retVal = true;
			}
		}
	}

	CloseHandle( myFileHandle );
	
	#elif defined _CRUNCH_ANDROID

  char *utf8String;
  int utf8StringSize;
  if ( filePath.getUtf8String( &utf8String, &utf8StringSize ) )
  {
    struct stat attrib;
    if ( stat( utf8String, &attrib ) == 0 )
    {
      struct tm myTime;
      
      time_t mtime = (time_t)( attrib.st_mtime );
      localtime_r( &mtime, &myTime );
      
      info->mYear = myTime.tm_year;
      info->mMonth = myTime.tm_mon;
      info->mDayOfWeek = myTime.tm_wday;
      info->mDay = myTime.tm_mday;
      info->mHour = myTime.tm_hour;
      info->mMinute = myTime.tm_min;
      info->mSecond = myTime.tm_sec;
      info->mMilliseconds = 0;
      
      retVal = true;
    }
    
    free( utf8String );
  }
	
	#else
	
  char *utf8String;
  int utf8StringSize;
  if ( filePath.getUtf8String( &utf8String, &utf8StringSize ) )
  {
    struct stat attrib;
    if ( stat( utf8String, &attrib ) == 0 )
    {
      struct tm myTime;
      localtime_r( &( attrib.st_mtime ), &myTime );
      
      info->mYear = myTime.tm_year;
      info->mMonth = myTime.tm_mon;
      info->mDayOfWeek = myTime.tm_wday;
      info->mDay = myTime.tm_mday;
      info->mHour = myTime.tm_hour;
      info->mMinute = myTime.tm_min;
      info->mSecond = myTime.tm_sec;
      info->mMilliseconds = 0;
      
      retVal = true;
    }
    
    free( utf8String );
  }
	
	#endif

	return retVal;
}

bool FileTools::setFileModifiedDateTimeInfo( const WideString& filePath, const fileTimeInfo& info )
{
  #if defined _CRUNCH_WINDOWS

	bool retVal = false;
	
	HANDLE myFileHandle = CreateFileW( (LPCWSTR)( filePath.wc_str() ),
	                                  GENERIC_READ | GENERIC_WRITE,
                                      0, // share mode
                                      nullptr, // LPSECURITY_ATTRIBUTES lpSecurityAttributes
                                      OPEN_EXISTING,
                                      FILE_ATTRIBUTE_NORMAL,
                                      nullptr // HANDLE hTemplateFile
                                      );

	if ( myFileHandle == INVALID_HANDLE_VALUE )
		return false;
	
	SYSTEMTIME st;
	
	st.wYear = info.mYear;
	st.wMonth = info.mMonth;
	st.wDayOfWeek = info.mDayOfWeek;
	st.wDay = info.mDay;
	st.wHour = info.mHour;
	st.wMinute = info.mMinute;
	st.wSecond = info.mSecond;
	st.wMilliseconds = info.mMilliseconds;
	
	FILETIME ft;
	
	if ( SystemTimeToFileTime( &st, &ft ) != 0 )
	{
		FILETIME modifiedFileTime;
		
		if ( LocalFileTimeToFileTime( &ft, &modifiedFileTime ) != 0 )
		{
			if ( SetFileTime( myFileHandle, nullptr, nullptr, &modifiedFileTime ) != 0 )
				retVal = true;
		}
	}
	
	CloseHandle( myFileHandle );
	
	return retVal;

  #else

  return false;

  #endif
}

int FileTools::compare( const fileTimeInfo& lhs, const fileTimeInfo& rhs ) // const SYSTEMTIME& pSr, const SYSTEMTIME& pSl )
{
  #if defined _CRUNCH_WINDOWS
  
	// SYSTEMTIME t_res;
	FILETIME v_ftime;
	ULARGE_INTEGER v_ui;
	__int64 v_right, v_left, v_res;

	SYSTEMTIME s_lhs;
	s_lhs.wYear = lhs.mYear;
	s_lhs.wMonth = lhs.mMonth;
	s_lhs.wDayOfWeek = lhs.mDayOfWeek;
	s_lhs.wDay = lhs.mDay;
	s_lhs.wHour = lhs.mHour;
	s_lhs.wMinute = lhs.mMinute;
	s_lhs.wSecond = lhs.mSecond;
	s_lhs.wMilliseconds = lhs.mMilliseconds;

	SYSTEMTIME s_rhs;
	s_rhs.wYear = rhs.mYear;
	s_rhs.wMonth = rhs.mMonth;
	s_rhs.wDayOfWeek = rhs.mDayOfWeek;
	s_rhs.wDay = rhs.mDay;
	s_rhs.wHour = rhs.mHour;
	s_rhs.wMinute = rhs.mMinute;
	s_rhs.wSecond = rhs.mSecond;
	s_rhs.wMilliseconds = rhs.mMilliseconds;

	SystemTimeToFileTime( &s_rhs, &v_ftime );
	v_ui.LowPart = v_ftime.dwLowDateTime;
	v_ui.HighPart = v_ftime.dwHighDateTime;
	v_right = v_ui.QuadPart;

	SystemTimeToFileTime( &s_lhs, &v_ftime );
	v_ui.LowPart = v_ftime.dwLowDateTime;
	v_ui.HighPart = v_ftime.dwHighDateTime;
	v_left = v_ui.QuadPart;

	v_res = v_right - v_left;

	// v_ui.QuadPart = v_res;
	// v_ftime.dwLowDateTime = v_ui.LowPart;
	// v_ftime.dwHighDateTime = v_ui.HighPart;
	// FileTimeToSystemTime( &v_ftime, &t_res );

	// return t_res;

	if ( v_res == 0 )
		return 0;

	return ( ( v_res > 0 ) ? 1 : -1 );
	
	#else
	
	struct tm tm_lhs;
  tm_lhs.tm_year = lhs.mYear;
  tm_lhs.tm_mon = lhs.mMonth;
  tm_lhs.tm_wday = lhs.mDayOfWeek;
  tm_lhs.tm_mday = lhs.mDay;
  tm_lhs.tm_hour = lhs.mHour;
  tm_lhs.tm_min = lhs.mMinute;
  tm_lhs.tm_sec = lhs.mSecond;
	time_t t0 = mktime( &tm_lhs );
	
	struct tm tm_rhs;
  tm_rhs.tm_year = rhs.mYear;
  tm_rhs.tm_mon = rhs.mMonth;
  tm_rhs.tm_wday = rhs.mDayOfWeek;
  tm_rhs.tm_mday = rhs.mDay;
  tm_rhs.tm_hour = rhs.mHour;
  tm_rhs.tm_min = rhs.mMinute;
  tm_rhs.tm_sec = rhs.mSecond;
	time_t t1 = mktime( &tm_rhs );
	
	return ( ( difftime( t0, t1 ) > 0.0 ) ? -1 : 1 ); // t0-t1>0 => t0 bigger
	
	#endif
}

FILE* FileTools::open_util( const char *filename, const char *mode )
{
	#if defined _CRUNCH_WINDOWS

	FILE *retVal = nullptr;
	if ( fopen_s( &retVal, filename, mode ) == 0 && retVal != nullptr )
	  return retVal;
	
	#else
	
	FILE *retVal = fopen( filename, mode );
	if ( retVal != nullptr )
	  return retVal;
	
	#endif
	
	return nullptr;
}

FILE* FileTools::wopen_util( const wchar_t *filename, const wchar_t *mode )
{
	#if defined _CRUNCH_WINDOWS

	FILE *retVal = nullptr;
	if ( _wfopen_s( &retVal, filename, mode ) == 0 && retVal != nullptr )
	  return retVal;
	
	#else
	
	WideString myFilename( filename );
	
	char *utf8String;
	int utf8StringSize;
	if ( myFilename.getUtf8String( &utf8String, &utf8StringSize ) )
	{
	  char modeBuffer[16];
	  String::wstos( mode, modeBuffer, 15 );
	  modeBuffer[15] = 0;
	  
    FILE *retVal = fopen( utf8String, modeBuffer );
    
    free( utf8String );
    
    if ( retVal != nullptr )
      return retVal;
	}
	
	#endif
	
	return nullptr;
}

#if !defined _CRUNCH_WINDOWS
bool FileTools::copyFile( FILE *source, FILE *dest )
{
  if ( source == nullptr || dest == nullptr )
    return false;

  bool retVal = true;
	
  const int BUFFER_SIZE = 8192;
			
  char buffer[BUFFER_SIZE];
			
  while ( true )
  {
    unsigned int bytesRead = fread( buffer, 1, BUFFER_SIZE, source );
    if ( bytesRead == 0 )
      break;
				
    if ( fwrite( buffer, 1, bytesRead, dest ) != bytesRead )
    {
      retVal = false;
      break;
    }
				
    if ( bytesRead < BUFFER_SIZE )
      break;
  }
			
  fclose( dest );
  fclose( source );
	
  return retVal;
}
#endif

} }
