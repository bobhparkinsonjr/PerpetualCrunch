#if !defined ( CORE_WIDE_STRING_H )
#define CORE_WIDE_STRING_H

namespace crunch { namespace core {

class WideString
{
	public:
		WideString() :
			mBufferSize( 0 ),
			mWideString( nullptr )
		{
			// set us up to be the empty string

			mBufferSize = sizeof( wchar_t );
			assert( mWideString == nullptr );
			mWideString = wchar_alloc( mBufferSize );
			assert( mWideString != nullptr );
			mWideString[0] = 0;
		}

    // Make sure utf8String is terminated with a null terminator.
    //   utf8String - start of utf-8 encoded string
    //   utf8StringSize - size in bytes of the given utf8String
		WideString( const char *utf8String, unsigned int utf8StringSize ) :
			mBufferSize( 0 ),
			mWideString( nullptr )
		{
			if ( utf8String == nullptr || utf8StringSize == 0 || utf8String[utf8StringSize - 1] != 0 )
				return;

			#if defined _CRUNCH_WINDOWS

			int totalCharactersNeeded = MultiByteToWideChar( CP_UTF8, 0, (LPCSTR)( utf8String ), utf8StringSize, mWideString, 0 );

			if ( totalCharactersNeeded == 0 )
			{
				*this = L"";
				return;
			}

			mBufferSize = ( totalCharactersNeeded * sizeof( wchar_t ) );
			assert( mWideString == nullptr );
			mWideString = wchar_alloc( mBufferSize );
			assert( mWideString != nullptr );

			if ( mWideString != nullptr )
			{
				if ( MultiByteToWideChar( CP_UTF8, 0, (LPCSTR)( utf8String ), utf8StringSize, mWideString, totalCharactersNeeded ) == 0 )
					destroy();
			}

			#else

			size_t totalCharactersNeeded = mbstowcs( nullptr, utf8String, 0 );
			if ( totalCharactersNeeded == ( (size_t)( -1 ) ) )
			{
				*this = L"";
				return;
			}

			mBufferSize = ( totalCharactersNeeded * sizeof( wchar_t ) ) + sizeof( wchar_t ); // NOTE: should need the extra wchar_t here for nullptr-terminator
			assert( mWideString == nullptr );
			mWideString = wchar_alloc( mBufferSize );
			assert( mWideString != nullptr );

			if ( mWideString != nullptr )
			{
				if ( mbstowcs( mWideString, utf8String, totalCharactersNeeded + 1 ) == (size_t)( -1 ) )
					destroy();
				else
					mWideString[totalCharactersNeeded] = 0;
			}

			#endif
		}

		WideString& operator = ( const WideString& other )
		{
			if ( &other == this )
				return *this;
			
			makeCopy( other );
				
			return *this;
		}

		WideString( const WideString& other ) :
			mBufferSize( 0 ),
			mWideString( nullptr )
		{
			makeCopy( other );
		}

		WideString( const wchar_t* a ) :
			mBufferSize( 0 ),
			mWideString( nullptr )
		{
			if ( a == nullptr )
				return;

			int l = (int)( wcslen( a ) );
			mBufferSize = ( l * sizeof( wchar_t ) ) + sizeof( wchar_t );

			assert( mWideString == nullptr );
			mWideString = wchar_alloc( mBufferSize );
			assert( mWideString != nullptr );
			memcpy( mWideString, a, mBufferSize );
		}

		WideString( const String& other ) :
			mBufferSize( 0 ),
			mWideString( nullptr )
		{
			mBufferSize = ( (unsigned int)( other.size() ) * sizeof( wchar_t ) ) + sizeof( wchar_t );

			assert( mWideString == nullptr );
			mWideString = wchar_alloc( mBufferSize );
			assert( mWideString != nullptr );
			String::stows( other.c_str(), mWideString, ( mBufferSize / sizeof( wchar_t ) ) );
		}

		WideString( const wchar_t* a, const wchar_t* b ) :
			mBufferSize( 0 ),
			mWideString( nullptr )
		{
			if ( a == nullptr || b == nullptr )
				return;

			int la = (int)( wcslen( a ) );
			int lb = (int)( wcslen( b ) );

			int laSize = la * sizeof( wchar_t );
			int lbSize = lb * sizeof( wchar_t );

			mBufferSize = laSize + lbSize + sizeof( wchar_t );

			assert( mWideString == nullptr );
			mWideString = wchar_alloc( mBufferSize );
			assert( mWideString != nullptr );

			memcpy( mWideString, a, laSize );
			memcpy( &( ( (char*)( mWideString ) )[laSize] ), b, lbSize );
			
			mWideString[la + lb] = 0;
		}

		~WideString()
		{
			destroy();
		}

	public:
		void destroy()
		{
			if ( mWideString != nullptr )
			{
				wchar_free( mWideString );
				mWideString = nullptr;
			}

			mBufferSize = 0;
		}

    // If this function returns true, the caller is responsible for freeing the given
    // *utf8String with a call to free.
    // utf8String - pointer to where the memory shall be allocated
    // utf8StringSize - upon return, the size in bytes of the allocated string
    // returned string always includes a nullptr terminator.
		bool getUtf8String( char **utf8String, int *utf8StringSize ) const
		{
			if ( utf8String == nullptr || utf8StringSize == nullptr )
				return false;

			*utf8String = nullptr;
			*utf8StringSize = 0;

			if ( mWideString == nullptr || mBufferSize == 0 )
				return false;

			#if defined _CRUNCH_WINDOWS

			int bytesRequired = WideCharToMultiByte( CP_UTF8, 0, mWideString, -1, nullptr, 0, nullptr, nullptr );

			if ( bytesRequired <= 0 )
				return false;

			*utf8String = (char*)( malloc( bytesRequired ) );
			assert( *utf8String != nullptr );
			if ( *utf8String == nullptr )
				return false;

			*utf8StringSize = bytesRequired;

			WideCharToMultiByte( CP_UTF8, 0, mWideString, -1, (LPSTR)( *utf8String ), *utf8StringSize, nullptr, nullptr );

			#else

			size_t bytesRequired = wcstombs( nullptr, mWideString, 0 ); // NOTE: doesn't include space for nullptr-terminator
			if ( bytesRequired == (size_t)( -1 ) )
				return false;

			++bytesRequired; // NOTE: adding space for nullptr-terminator
			*utf8String = (char*)( malloc( bytesRequired ) );
			assert( *utf8String != nullptr );
			if ( *utf8String == nullptr )
				return false;

			*utf8StringSize = (int)( bytesRequired );

			wcstombs( *utf8String, mWideString, bytesRequired );
			(*utf8String)[bytesRequired - 1] = 0;

			#endif

			return true;
		}

		core_forceinline const wchar_t* wc_str() const { return mWideString; } // NOTE: can return nullptr

    core_forceinline String toString() const { return String::wstos( wc_str() ); }

		core_forceinline WideString bsubstr( int lowerCharacterIndex, int upperCharacterIndex ) const
		{
			return substr( lowerCharacterIndex, upperCharacterIndex - lowerCharacterIndex + 1 );
		}

		WideString substr( int startCharacterIndex, int totalCharacters = -1 ) const
		{
			assert( startCharacterIndex >= 0 );
			assert( startCharacterIndex <= int( size() ) );

			if ( totalCharacters == 0 )
				return WideString();

			if ( totalCharacters < 0 )
				return WideString( &( mWideString[startCharacterIndex] ) );

			assert( totalCharacters > 0 );
			assert( ( startCharacterIndex + totalCharacters ) <= int( size() ) );

			unsigned int l = ( totalCharacters * sizeof( wchar_t ) ) + sizeof( wchar_t );

			wchar_t *buffer = wchar_alloc( l );
			assert( buffer != nullptr );
			memcpy( buffer, &( mWideString[startCharacterIndex] ), totalCharacters * sizeof( wchar_t ) );
			buffer[totalCharacters] = 0;

			WideString retVal( buffer );

			wchar_free( buffer );

			return retVal;
		}

		int find( wchar_t ch, int startCharacterIndex = -1 ) const
		{
			int totalCharacters = int( size() );
			int chIndex;

			for ( chIndex = ( startCharacterIndex >= 0 ) ? startCharacterIndex : 0; chIndex < totalCharacters; ++chIndex )
			{
				if ( mWideString[chIndex] == ch )
					return chIndex;
			}

			return -1;
		}

		int rfind( wchar_t ch, int startCharacterIndex = -1 ) const
		{
			int totalCharacters = int( size() );
			int chIndex;

			for ( chIndex = ( startCharacterIndex >= 0 ) ? startCharacterIndex : ( totalCharacters - 1 ); chIndex >= 0; --chIndex )
			{
				if ( mWideString[chIndex] == ch )
					return chIndex;
			}

			return -1;
		}

		int find( const WideString& str, int startCharacterIndex = -1 ) const
		{
			return find( str.wc_str(), startCharacterIndex );
		}

		int find( const wchar_t* str, int startCharacterIndex = -1 ) const
		{
			if ( str == nullptr )
				return -1;

			int totalCharacters = int( size() );
			int chIndex;

			int strLength = int( wcslen( str ) );

			int lastIndex = totalCharacters - strLength;

			WideString findMe = str;

			for ( chIndex = ( startCharacterIndex >= 0 ) ? startCharacterIndex : 0; chIndex <= lastIndex; ++chIndex )
			{
				if ( substr( chIndex, strLength ) == findMe )
					return chIndex;
			}

			return -1;
		}

		int rfind( const WideString& str, int startCharacterIndex = -1 ) const
		{
			return rfind( str.wc_str(), startCharacterIndex );
		}

		int rfind( const wchar_t* str, int startCharacterIndex = -1 ) const
		{
			if ( str == nullptr )
				return -1;

			int totalCharacters = int( size() );
			int chIndex;

			int strLength = int( wcslen( str ) );

			int lastIndex = totalCharacters - strLength;

			WideString findMe = str;

			for ( chIndex = ( startCharacterIndex >= 0 ) ? startCharacterIndex : lastIndex; chIndex >= 0; --chIndex )
			{
				if ( substr( chIndex, strLength ) == findMe )
					return chIndex;
			}

			return -1;
		}

		core_forceinline unsigned int size() const // size in characters
		{
			if ( mBufferSize > 0 )
			{
				assert( ( ( int( mBufferSize ) / sizeof( wchar_t ) ) - 1 ) >= 0 );
				return ( ( mBufferSize / sizeof( wchar_t ) ) - 1 );
			}

			return 0;
		}

		core_forceinline wchar_t& at( int i )
		{
			return (*this)[i];
		}

		core_forceinline const wchar_t& at( int i ) const
		{
			return (*this)[i];
		}

		void append( int count, wchar_t ch )
		{
			wchar_t a[2];
			a[0] = ch;
			a[1] = 0;

			WideString temp( a );

			for ( int i = 0; i < count; ++i )
				*this = *this + temp;
		}

	public:
		core_forceinline WideString operator + ( const WideString& other ) const
		{
			return WideString( wc_str(), other.wc_str() );
		}

		core_forceinline wchar_t& operator [] ( int i ) 
		{ 
			assert( i >= 0 );
			assert( i < int( size() ) );
			return mWideString[i];
		} 
		
		core_forceinline const wchar_t& operator [] ( int i ) const 
		{ 
			assert( i >= 0 );
			assert( i < int( size() ) );
			return mWideString[i];
		}

		core_forceinline bool operator == ( const WideString& other ) const
		{
			if ( size() != other.size() )
				return false;

			int chIndex;
			int totalCharacters = size();
			for ( chIndex = 0; chIndex < totalCharacters; ++chIndex )
			{
				if ( (*this)[chIndex] != other[chIndex] )
					return false;
			}

			return true;
		}

		core_forceinline bool operator != ( const WideString& other ) const
		{
			return !( *this == other );
		}

		core_forceinline bool operator < ( const WideString& other ) const
		{
			if ( mWideString != nullptr && other.mWideString != nullptr )
				return ( wcscmp( mWideString, other.mWideString ) < 0 );
			return false;
		}

		core_forceinline bool operator > ( const WideString& other ) const
		{
			return ( !( ( *this < other ) || ( *this == other ) ) );
		}

		core_forceinline WideString& operator += ( const WideString& rhs )
		{
			*this = WideString( wc_str(), rhs.wc_str() );
			return *this;
		}

	public:
    // needed on non-windows platforms
		static void setup();

	private:
		void makeCopy( const WideString& other )
		{
			destroy();

			if ( other.mWideString != nullptr && other.mBufferSize > 0 )
			{
				mBufferSize = other.mBufferSize;

				assert( mWideString == nullptr );
				mWideString = wchar_alloc( mBufferSize );
				assert( mWideString != nullptr );
				memcpy( mWideString, other.mWideString, mBufferSize );
			}
		}

	private:
		static wchar_t* wchar_alloc( size_t bytesRequired )
		{
			assert( bytesRequired > 0 );

			if ( bytesRequired == sizeof( wchar_t ) )
				return &mEmptyString; // there is only room for the nullptr terminator

			return (wchar_t*)( malloc( bytesRequired ) );
		}

		static void wchar_free( wchar_t *buffer )
		{
			if ( buffer && buffer != &mEmptyString )
				free( buffer );
		}

	private:
		wchar_t *mWideString;
		unsigned int mBufferSize;

	private:
		static wchar_t mEmptyString;

};

} }

#endif
