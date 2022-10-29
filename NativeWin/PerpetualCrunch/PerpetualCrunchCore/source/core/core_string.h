#if !defined ( CORE_STRING_H )
#define CORE_STRING_H

#include <cinttypes>

namespace crunch { namespace core {

class String : public std::string
{
	public:
		String() : std::string()
		{
			// empty
		}
		
		String( const String& other ) :
			std::string( other )
		{
			// empty
		}
	
		String( const std::string& other ) :
			std::string( other )
		{
			// empty
		}
		
		String( const char *other ) :
			std::string( other )
		{
			// empty
		}
		
		String& operator = ( const String& other )
		{
			if ( &other == this )
				return *this;
				
			*( (std::string*)( this ) ) = other;
				
			return *this;
		}
		
		String& operator = ( const std::string& other )
		{
			if ( &other == this )
				return *this;
				
			*( (std::string*)( this ) ) = other;
				
			return *this;
		}
		
		String& operator = ( const char *other )
		{
			*( (std::string*)( this ) ) = other;
				
			return *this;
		}

	public:
		// Convert string to all lowercase.
		void toLower()
		{
			size_type total = size();
			size_type index;
			
			for ( index = 0; index < total; ++index )
			{
				char ch = at( index );
				if ( ch >= 'A' && ch <= 'Z' )
				{
					ch = ( ch - 'A' ) + 'a';
					at( index ) = ch;
				}
			}
		}

    String getLower() const
    {
      String retVal = *this;
      retVal.toLower();
      return retVal;
    }
		
		// Convert string to all uppercase.
		void toUpper()
		{
			size_type total = size();
			size_type index;
			
			for ( index = 0; index < total; ++index )
			{
				char ch = at( index );
				if ( ch >= 'a' && ch <= 'z' )
				{
					ch = ( ch - 'a' ) + 'A';
					at( index ) = ch;
				}
			}
		}
		
    String getUpper() const
    {
      String retVal = *this;
      retVal.toUpper();
      return retVal;
    }

	  // a stringx in [lower,upper]
		inline String bsubstr( size_type lower, size_type upper ) const
		{
			return substr( lower, upper - lower + 1 );
		}
		
		void trimFrontWS()
		{
			size_type index = 0;
			size_type total = size();
			while ( index < total )
			{
				char ch = at( index );

				if ( ch > 0 && !( isspace( ch ) ) )
					break;
				++index;
			}
			
			if ( index < total )
				*this = substr( index );
			else
				*this = "";
		}
		
		void trimBackWS()
		{
			int mySize = int( size() );
			if ( mySize == 0 )
				return;
		
			int index = mySize - 1;
			while ( index >= 0 )
			{
				char ch = at( index );

				if ( ch > 0 && !( isspace( ch ) ) )
					break;
				--index;
			}
			
			if ( index >= 0 )
				*this = substr( 0, index + 1 );
			else
				*this = "";
		}
		
		void trimWS()
		{
			trimFrontWS();
			trimBackWS();
		}

    // true if string is empty or contains only WS
    bool isBlank() const
    {
      int t = int( size() );

      if ( t <= 0 )
        return true;

      for ( int i = 0; i < t; ++i )
      {
        char ch = at( i );

        if ( !( isspace( ch ) ) )
          return false;
      }

      return true;
    }
		
		// Searches this string for all occurrences of findMe, and replaces
		// those occurrences with insertMe.
		void findAndReplace( const String& findMe, const String& insertMe )
		{
			size_type findMeLength = findMe.size();
			if ( findMeLength == 0 )
				return;
				
			size_type insertMeLength = insertMe.size();
			
			int index = int( find( findMe ) );
			while ( index >= 0 )
			{
				// example:
				// abfindmecd
				// 01234567890
				// index = 2
				// index + findMeLength = 8
				
				if ( index > 0 && index < ( int( size() ) - 1 ) )
					*this = substr( 0, index ) + insertMe + substr( index + findMeLength );
				else
				if ( index > 0 )
					*this = substr( 0, index ) + insertMe;
				else
				{
					assert( index == 0 );
					*this = insertMe + substr( findMeLength );
				}
				
				index = int( find( findMe, index + insertMeLength ) );
			}
		}
		
		void removeCharacters( unsigned char lowerInclusiveBound, unsigned char upperInclusiveBound, unsigned char replaceChar = ' ' )
		{
			int myLength = int( size() );
			int index;
			for ( index = 0; index < myLength; ++index )
			{
				unsigned char ch = (unsigned char)( at( index ) );
			
				if ( ch >= lowerInclusiveBound && ch <= upperInclusiveBound )
					at( index ) = replaceChar;
			}
		}

		bool isIntFormat() const
		{
			int myLength = int( size() );
      if ( myLength <= 0 )
        return false;

			int index;
			for ( index = 0; index < myLength; ++index )
			{
				unsigned char ch = (unsigned char)( at( index ) );
			
				if ( !( ch >= '0' && ch <= '9' ) && ( index != 0 || ch != '-' ) )
					return false;
			}

			return true;
		}

		bool isFloatFormat() const
		{
			bool seenDot = false;

			int myLength = int( size() );
      if ( myLength <= 0 )
        return false;

			int index;
			for ( index = 0; index < myLength; ++index )
			{
				unsigned char ch = (unsigned char)( at( index ) );

				if ( ch == '.' )
				{
					if ( seenDot )
						return false;

					seenDot = true;

					continue;
				}
			
				if ( !( ch >= '0' && ch <= '9' ) && ( index != 0 || ch != '-' ) )
					return false;
			}

			return true;
		}

    std::vector< int > splitInt( char ch ) const
    {
      std::vector< String > a = split( ch );

      int total = (int)( a.size() );

      std::vector< int > retVal;

      if ( total > 0 )
      {
        retVal.resize( total );

        for ( int i = 0; i < total; ++i )
          retVal[i] = String::to_i32( a[i] );
      }

      return retVal;
    }

    std::vector< String > split( char ch ) const
    {
      /*
        SAMPLES:

        delim = ,

        ,one,two,three
        012345678901234567890
        i = 0
        ""
        pi = 0
        i = 4
        "" "one"
        pi = 4
        i = 8
        "" "one" "two"
        pi = 8
        i = -1
        "" "one" "two" "three"

        one,two,three
        012345678901234567890
        pi = -1
        i = 3
        "one"
        pi = 3
        i = 7
        "one" "two"
        pi = 7
        i = -1
        "one" "two" "three"

        one
        012345678901234567890
        pi = -1
        i = -1
        "one"

        one,two,three,
        012345678901234567890
        pi = -1
        i = 3
        "one"
        pi = 3
        i = 7
        "one" "two"
        pi = 7
        i = 13
        "one" "two" "three"
        pi = 13
        i = -1
        "one" "two" "three" ""

      */

      std::vector< String > retVal;

      int i = int( find( ch ) );

      if ( i >= 0 )
      {
        int pi = -1;

        while ( i >= 0 )
        {
          if ( i == 0 )
            retVal.push_back( "" );
          else
            retVal.push_back( bsubstr( pi + 1, i - 1 ) );

          pi = i;
          i = int( find( ch, i + 1 ) );
        }

        if ( ( pi + 1 ) < int( size() ) )
          retVal.push_back( bsubstr( pi + 1, int( size() ) - 1 ) );
        else
        if ( ( pi + 1 ) == int( size() ) )
          retVal.push_back( "" );
      }
      else
      {
        retVal.push_back( *this );
      }

      return retVal;
    }
		
	public:
		static String itos( int i )
		{
			#define WORK_SIZE 256
		
			char work[WORK_SIZE];
			
			#if defined _CRUNCH_WINDOWS
			_snprintf_s( work, WORK_SIZE, WORK_SIZE - 1, "%d", i );
			#else
			snprintf( work, WORK_SIZE - 1, "%d", i );
			work[WORK_SIZE - 1] = 0;
			#endif
			
			#undef WORK_SIZE
			
			return String( work );
		}

		static String i64tos( int64_t i )
		{
			#define WORK_SIZE 256
		
			char work[WORK_SIZE];
			
			#if defined _CRUNCH_WINDOWS
			_snprintf_s( work, WORK_SIZE, WORK_SIZE - 1, "%" PRId64, i );
			#else
			snprintf( work, WORK_SIZE - 1, "%" PRId64, i );
			work[WORK_SIZE - 1] = 0;
			#endif
			
			#undef WORK_SIZE
			
			return String( work );
		}
		
		static String utos( unsigned int u )
		{
			#define WORK_SIZE 256
		
			char work[WORK_SIZE];
			
			#if defined _CRUNCH_WINDOWS
			_snprintf_s( work, WORK_SIZE, WORK_SIZE - 1, "%u", u );
			#else
			snprintf( work, WORK_SIZE - 1, "%u", u );
			work[WORK_SIZE - 1] = 0;
			#endif
			
			#undef WORK_SIZE
			
			return String( work );
		}

		static String u64tos( uint64_t u )
		{
			#define WORK_SIZE 256
		
			char work[WORK_SIZE];
			
			#if defined _CRUNCH_WINDOWS
			_snprintf_s( work, WORK_SIZE, WORK_SIZE - 1, "%" PRIu64, u );
			#else
			snprintf( work, WORK_SIZE - 1, "%" PRIu64, u );
			work[WORK_SIZE - 1] = 0;
			#endif
			
			#undef WORK_SIZE
			
			return String( work );
		}
		
		static String ftos( float f, int postDecimalDigits = 6 )
		{
			#define WORK_SIZE 256
		
			char work[WORK_SIZE];

      String fmtString = String::fmt( "%%0.%df", postDecimalDigits );
			
			#if defined _CRUNCH_WINDOWS
			_snprintf_s( work, WORK_SIZE, WORK_SIZE - 1, fmtString.c_str(), f );
			#else
			snprintf( work, WORK_SIZE - 1, fmtString.c_str(), f );
			work[WORK_SIZE - 1] = 0;
			#endif
			
			#undef WORK_SIZE
			
			return String( work );
		}
		
		static float stof( const String& value )
		{
			return (float)( atof( value.c_str() ) );
		}

		static String dtos( double d, int postDecimalDigits = 8 )
		{
			#define WORK_SIZE 256
		
			char work[WORK_SIZE];

      String fmtString = String::fmt( "%%0.%df", postDecimalDigits );
			
			#if defined _CRUNCH_WINDOWS
			_snprintf_s( work, WORK_SIZE, WORK_SIZE - 1, fmtString.c_str(), d );
			#else
			snprintf( work, WORK_SIZE - 1, fmtString.c_str(), d );
			work[WORK_SIZE - 1] = 0;
			#endif
			
			#undef WORK_SIZE
			
			return String( work );
		}
		
		static double to_f64( const String& value )
		{
			return atof( value.c_str() );
		}
		
		static int to_i32( const String& value )
		{
			return atoi( value.c_str() );
		}

    static int64_t to_i64( const String& value )
    {
      return (int64_t)( atoll( value.c_str() ) );
    }
		
		static unsigned int to_u32( const String& value )
		{
			String myValue = value;

			while ( myValue.size() > 1 && myValue[0] == '0' )
				myValue = myValue.substr( 1 );

			return (unsigned int)( strtoul( myValue.c_str(), nullptr, 0 ) );
		}

    static uint64_t to_u64( const String& value )
    {
			String myValue = value;

			while ( myValue.size() > 1 && myValue[0] == '0' )
				myValue = myValue.substr( 1 );

      return (uint64_t)( strtoull( myValue.c_str(), nullptr, 0 ) );
    }

		static unsigned int hextou( const String& value )
		{
			String myValue;
			if ( value.size() > 2 && value[0] == '0' && ( value[1] == 'x' || value[1] == 'X' ) )
				myValue = value.substr( 2 );
			else
				myValue = value;
			
			char *stop;
			unsigned long num = strtoul( myValue.c_str(), &stop, 16 );
			
			return (unsigned int)( num );
		}
		
		static float hextof( const String& value )
		{
			String myValue;
			if ( value.size() > 2 && value[0] == '0' && ( value[1] == 'x' || value[1] == 'X' ) )
				myValue = value.substr( 2 );
			else
				myValue = value;
			
			char *stop;
			unsigned long num = strtoul( myValue.c_str(), &stop, 16 );
			
			float f = *( reinterpret_cast< float* >( &num ) );
			
			return f;
		}

		static double hextod( const String& value )
		{
			String myValue;
			if ( value.size() > 2 && value[0] == '0' && ( value[1] == 'x' || value[1] == 'X' ) )
				myValue = value.substr( 2 );
			else
				myValue = value;
			
			#if defined _CRUNCH_WINDOWS
			char *stop;
			unsigned __int64 num = _strtoui64( myValue.c_str(), &stop, 16 );
			#else
			unsigned long long num = strtoull( myValue.c_str(), nullptr, 16 );
			#endif
			
			double d = *( reinterpret_cast< double* >( &num ) );
			
			return d;
		}
		
		static String fmt( const char *format, ... )
    {
		  va_list args;
		  va_start( args, format );
      String r = fmtv( format, args );
      va_end( args );
      return r;
    }

    static String fmtv( const char *format, va_list args )
		{
		  const int WORK_SIZE = 8192;
		  char work[WORK_SIZE];
		
		  int total = _vscprintf( format, args );
		  if ( total < 0 )
			  return "";
		
		  char *bufferPtr = &( work[0] );
		
		  if ( total >= WORK_SIZE )
			  bufferPtr = (char*)( malloc( total + 1 ) );
			
		  assert( bufferPtr );
		
		  #if defined _CRUNCH_WINDOWS
		  vsnprintf_s( bufferPtr, total + 1, _TRUNCATE, format, args );
		  #else
		  vsnprintf( bufferPtr, total + 1, format, args );
		  #endif
		
		  bufferPtr[total] = 0;
		
		  if ( bufferPtr != &( work[0] ) )
      {
        String retVal( bufferPtr );
			  free( bufferPtr );
        return retVal;
      }

      return String( bufferPtr );
		}
		
		static bool stows( const char *source, WCHAR *dest, int destSize )
		{
			assert( source );
			assert( dest );
			
			int index = 0;
			int lastIndex = destSize - 1;
			while ( source[index] != 0 && index < lastIndex )
			{
				dest[index] = source[index];
				++index;
			}
			
			if ( source[index] == 0 && index < destSize )
			{
				dest[index] = 0;
				return true;
			}
			
			dest[destSize - 1] = 0;
			
			return false;
		}

		static String wstosl( const WCHAR *source, unsigned int sourceLength )
		{
			assert( source );

      unsigned int sourceSize = ( sourceLength * sizeof( WCHAR ) ) + sizeof( WCHAR );

			const int BUFFER_SIZE = 1024;
			char buffer[BUFFER_SIZE];

      char *dest;
      int destSize;

      if ( sourceSize <= BUFFER_SIZE )
      {
        dest = &( buffer[0] );
        destSize = BUFFER_SIZE;
      }
      else
      {
        dest = (char*)( malloc( sourceSize ) );
        destSize = sourceSize;
      }

			if ( !( String::wstos( source, dest, destSize ) ) )
      {
        if ( dest != &( buffer[0] ) )
          free( dest );

				return "";
      }

			String retVal = String( dest );
      
      if ( dest != &( buffer[0] ) )
        free( dest );

      return retVal;
		}

		static String wstos( const WCHAR *source )
		{
			assert( source );

			const int BUFFER_SIZE = 1024;
			char buffer[BUFFER_SIZE];

      unsigned int l = wlen( source );

      char *bufferPtr;
      unsigned int bufferSize;

      if ( l < BUFFER_SIZE )
      {
        bufferPtr = &( buffer[0] );
        bufferSize = BUFFER_SIZE;
      }
      else
      {
        bufferSize = l + 1;
        bufferPtr = (char*)( malloc( bufferSize ) );
      }

			if ( !( String::wstos( source, bufferPtr, bufferSize ) ) )
      {
        if ( bufferPtr != &( buffer[0] ) )
          free( bufferPtr );

				return "";
      }

      String r = String( bufferPtr );

      if ( bufferPtr != &( buffer[0] ) )
        free( bufferPtr );

			return r;
		}

    static unsigned int wlen( const WCHAR *source )
    {
      int index = 0;
      while ( source[index] != 0 ) ++index;
      return index;
    }

		static bool wstos( const WCHAR *source, char *dest, int destSize )
		{
			assert( source );
			assert( dest );
			
			int index = 0;
			int lastIndex = destSize - 1;
			while ( source[index] != 0 && index < lastIndex )
			{
				dest[index] = (char)( source[index] );
				++index;
			}
			
			if ( source[index] == 0 && index < destSize )
			{
				dest[index] = 0;
				return true;
			}
			
			dest[destSize - 1] = 0;
			
			return false;
		}
		
};

} }

#endif
