#if !defined ( CORE_BINARY_BUFFER_H )
#define CORE_BINARY_BUFFER_H

namespace crunch { namespace core {

class BinaryBuffer
{
	public:
		typedef enum
		{
			FLAG_ENDIAN_CONVERT_ON_APPEND        = 0x00000001,
			FLAG_SHARED_IMAGE                    = 0x00000002,
      FLAG_READ_EXCEEDED_END_OF_BUFFER     = 0x00000004
		} flagType;

	public:
		BinaryBuffer();
		~BinaryBuffer();
		
		BinaryBuffer( const BinaryBuffer& other );
		BinaryBuffer& operator = ( const BinaryBuffer& other );
		
	public:
		void destroy();

		bool setup( unsigned int bufferSize, unsigned int expansionSize );
		
		// The entire file is read into this buffer.  The buffer offset is left
		// at the beginning of the buffer.
		bool readFile( const char *filename, unsigned int expansionSize = 0 );
		bool readFile( const wchar_t *filename, unsigned int expansionSize = 0 );

    static bool readFile( const char *filename, std::vector< unsigned char > *dest );
    static bool readFile( const wchar_t *filename, std::vector< unsigned char > *dest );

		bool writeFile( const char *filename ) const;
		bool writeFile( const wchar_t *filename ) const;

    static bool writeFile( const char *filename, const std::vector< unsigned char >& source );
    static bool writeFile( const wchar_t *filename, const std::vector< unsigned char >& source );

		// Write buffer to the end of the given file.  Optionally, clear the buffer.
		bool appendFile( const char *filename, bool clearBuffer );
		bool appendFile( const wchar_t *filename, bool clearBuffer );

		// The given image becomes "shared".  This instance will not delete it, but
		// does reference it directly.  The caller is responsible for keeping it live
		// while this instance uses it.
		bool setImage( char *image, unsigned int imageSize );

		bool append_bool( bool b );
		bool append_int64( int64_t i );
		bool append_uint64( uint64_t u );
		bool append_int32( int32_t i );
		bool append_uint32( uint32_t u );
		bool append_int16( int16_t i );
		bool append_uint16( uint16_t u );
		bool append_int8( int8_t i );
		bool append_uint8( uint8_t u );
		bool append_String( const char *str );
		bool append_WideString( const wchar_t *str );
		bool append_FixedString32( const FixedString32& str );
		bool append_FixedString64( const FixedString64& str );
		bool append_FixedString128( const FixedString128& str );
		bool append_float( float f );
		bool append_double( double d );

    template < typename T >
    bool append_number( T v );
		
		bool append_buffer( const BinaryBuffer& source );
		bool append_raw( const void *buffer, unsigned int bufferSize );

		bool read_bool();
    int64_t read_int64();
    uint64_t read_uint64();
		int32_t read_int32();
		uint32_t read_uint32();
		int16_t read_int16();
		uint16_t read_uint16();
		int8_t read_int8();
		uint8_t read_uint8();
		String read_String();
		WideString read_WideString();
		FixedString32 read_FixedString32();
		FixedString64 read_FixedString64();
		FixedString128 read_FixedString128();
		float read_float();
		double read_double();

    template < typename T >
    T read_number();

		char* c_str() const;
		char* c_str_offset() const;

		bool setBufferOffset( unsigned int bufferOffset );

    bool removeFront( unsigned int amount );

		bool seekFromCurrent( unsigned int amount );

		inline bool isEndianConvertOnAppend() const { return isFlagged( FLAG_ENDIAN_CONVERT_ON_APPEND ); }
		inline void setEndianConvertOnAppend( bool enable ) { setFlag( FLAG_ENDIAN_CONVERT_ON_APPEND, enable ); }
		
		inline unsigned int getBufferLength() const { return mBufferOffset; }
		inline unsigned int getBufferSize() const { return mBufferSize; }
    inline unsigned int getBufferExpansionSize() const { return mBufferExpansionSize; }

    inline bool hasReadExceededEndOfBuffer() const { return isFlagged( FLAG_READ_EXCEEDED_END_OF_BUFFER ); }

	private:
		inline bool isFlagged( flagType f ) const { return ( ( mFlags & f ) != 0 ); }
		inline void setFlag( flagType f, bool enable ) { if ( enable ) mFlags |= f; else mFlags &= ~f; }
	
		bool updateBufferSize( unsigned int delta );

		int computeStringBufferSize( const char *str );
		int computeStringBufferSize( const wchar_t *str );

    bool readFile( FILE *source, const WideString& filename, unsigned int expansionSize );

    void makeCopy( const BinaryBuffer& other );
	
	private:
		char *mBuffer;
		unsigned int mBufferSize;
		unsigned int mBufferExpansionSize;
		unsigned int mBufferOffset;
		unsigned int mFlags;
	
};

} }

#endif
