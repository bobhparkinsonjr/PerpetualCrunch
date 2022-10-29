#if !defined ( CORE_FILE_TOOLS_H )
#define CORE_FILE_TOOLS_H

namespace crunch { namespace core {

class FileTools
{
	public:
		typedef struct _fileTimeInfo
		{
			WORD mYear;
			WORD mMonth;
			WORD mDayOfWeek;
			WORD mDay;
			WORD mHour;
			WORD mMinute;
			WORD mSecond;
			WORD mMilliseconds;
		} fileTimeInfo;
		
		typedef void (*progressCallbackType)( void *userData, float percentComplete );
	
	public:
    // if a file extension is found, returns a string of the form ".txt",
    // otherwise returns the empty string
		static String getFileExtension( const String& filename );
		
    // filename with all directory and file extension info removed
		static String getName( const String& filename );
		
    // the directory info, with trailing '/' or '\\', name and extension are removed
		static String getFileDirectory( const String& filename );
		
    // path - the complete path to the file to delete
    // verifyDelete - if true, function will not return until file reports that it no longer exists
    // returns: true if file was found and deleted
		static bool deleteFile( const String& path, bool verifyDelete = false );
		static bool deleteFile( const WideString& path, bool verifyDelete = false );

		static bool copyFile( const String& sourcePath, const String& destPath, bool overwriteOk );
		static bool copyFile( const WideString& sourcePath, const WideString& destPath, bool overwriteOk );

		static bool moveFile( const String& sourcePath, const String& destPath, bool overwriteOk );
		static bool moveFile( const WideString& sourcePath, const WideString& destPath, bool overwriteOk );

		static bool fileExists( const WideString& filename );

    // fileSize is in bytes
		static bool getFileSize( const WideString& filename, unsigned int *fileSize );
		
    // Searches from startFolder for findMe, will recurse into sub folders only if recurse is true.
    // returns: the found full path (dir+name+ext) on success, or the empty string if the file wasn't found
		static String findFile( const String& findMe, const String& startFolder, bool recurse );
		
    // supported on windows/linux
		static bool createDirectory( const String& dir );

    // supported on windows
    static bool createDirectoryW( const WideString& dir );

    static bool setCurrentWorkingDirectory( const WideString& dir );
    static bool getCurrentWorkingDirectory( WideString *dir );

		static bool getFileModifiedDateTimeInfo( const WideString& filePath, fileTimeInfo *info );
		
		static bool setFileModifiedDateTimeInfo( const WideString& filePath, const fileTimeInfo& info );

		static int compare( const fileTimeInfo& lhs, const fileTimeInfo& rhs );

	public:
	  static FILE* open_util( const char *filename, const char *mode );
	  static FILE* wopen_util( const wchar_t *filename, const wchar_t *mode );

    #if !defined _CRUNCH_WINDOWS
    static bool copyFile( FILE *source, FILE *dest );
    #endif

};

} }

#if defined _CRUNCH_WINDOWS
  #define core_fprintf fprintf_s
  #define core_ftell _ftelli64
  #define core_fpos_t __int64
  #define core_fseek _fseeki64
#else
  #define core_fprintf fprintf
  #define core_ftell ftell
  #define core_fpos_t long
  #define core_fseek fseek
#endif

#define core_fopen( _filename, _mode ) crunch::core::FileTools::open_util( _filename, _mode )
#define core_fwopen( _filename, _mode ) crunch::core::FileTools::wopen_util( _filename, _mode )

#endif
