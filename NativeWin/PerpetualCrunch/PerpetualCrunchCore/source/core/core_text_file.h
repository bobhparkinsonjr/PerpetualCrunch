#if !defined ( CORE_TEXT_FILE_H )
#define CORE_TEXT_FILE_H

namespace crunch { namespace core {

class TextFile
{
	public:
		enum
		{
			MODE_READ,
			MODE_WRITE,
			MODE_APPEND
		};

    enum
    {
      FLAG_FLUSH_ON_WRITE = 1 << 0
    };

	public:
		TextFile();
		virtual ~TextFile();
		
	public:
		bool open( const WideString& filename, int mode, bool allowBuffering = true, unsigned int flags = FLAG_FLUSH_ON_WRITE );
		
		core_forceinline bool isOpen() const { return ( mFile != nullptr ); }
		
		bool atEof() const;
		
		void close();
		
		bool readLine( String *line );
		
		bool write( const std::string& text );
		bool writeFormatted( const char *fmt, ... );
		
		void flush();
		
    core_forceinline bool isFlushOnWrite() const { return ( ( mFlags & FLAG_FLUSH_ON_WRITE ) != 0 ); }

  public:
    static bool writeAllText( const WideString& filename, const String& text );
    static bool readAllText( const WideString& filename, String *text );
		
	private:
		FILE *mFile;
		char *mBuffer;
		unsigned int mBufferOffset;
		unsigned int mBufferSize;
    unsigned int mFlags;
		
};

} }

#endif
