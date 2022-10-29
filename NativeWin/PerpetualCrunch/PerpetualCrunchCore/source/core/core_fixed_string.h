#if !defined ( CORE_FIXED_STRING_H )
#define CORE_FIXED_STRING_H

namespace crunch { namespace core {

template< int TOTAL_CHARS >
class FixedString
{
	public:
		typedef unsigned long long intType;
	
		enum { MAX_LENGTH = TOTAL_CHARS - 1 };
		enum { INT_SIZE = TOTAL_CHARS / sizeof( intType ) };
		
	public:
		FixedString()
		{
			clear();
		}
		
		FixedString( const char *value )
		{
			clear();
			
			int l = int( strlen( value ) );
			if ( l >= TOTAL_CHARS )
				l = TOTAL_CHARS - 1;
				
			memcpy( &( mData[0] ), value, l );
		}
		
		FixedString( const wchar_t *value )
		{
			clear();
			
			// convert to bytes here
			int l = int( wcslen( value ) ) * sizeof( wchar_t );

			if ( l >= TOTAL_CHARS )
				l = TOTAL_CHARS - 1;
				
			memcpy( &( mData[0] ), value, l );
		}
		
		FixedString( const FixedString& other )
		{
			makeCopy( other );
		}
		
		FixedString& operator = ( const FixedString& other )
		{
			if ( this == &other )
				return *this;
				
			makeCopy( other );
				
			return *this;
		}
		
		~FixedString()
		{
			// empty
		}
		
	public:
		core_forceinline void clear()
		{
			assert( ( TOTAL_CHARS % sizeof( intType ) ) == 0 );
			memset( &( mData[0] ), 0, TOTAL_CHARS );
		}
		
		core_forceinline bool empty() const { return ( mData[0] == 0 ); }
	
		core_forceinline bool lexLess( const FixedString& other ) const
		{
			int index;
			for ( index = 0; index < INT_SIZE; ++index )
			{
				if ( mData[index] != other.mData[index] )
					return ( mData[index] < other.mData[index] );
			}
			
			return false;
		}
		
		core_forceinline bool operator == ( const FixedString& other ) const
		{
			int index;
			for ( index = 0; index < INT_SIZE; ++index )
			{
				if ( mData[index] != other.mData[index] )
					return false;
			}
			
			return true;
		}
		
		core_forceinline bool operator != ( const FixedString& other ) const
		{
			return !( *this == other );
		}
		
		core_forceinline const char *c_str() const
		{
			return (const char*)( &( mData[0] ) );
		}

		core_forceinline const wchar_t *wc_str() const
		{
			return (const wchar_t*)( &( mData[0] ) );
		}

		core_forceinline unsigned int length() const
		{
			return (unsigned int)( strlen( c_str() ) );
		}
		
    // looks for findMe in this string, starting the search at startIndex.
    // returns: -1 if not found, otherwise index of first found occurence
		int find( const FixedString& findMe, int startIndex = 0 ) const
		{
      String myFindMe = findMe.c_str();
      String myText = c_str();

      return int( myText.find( myFindMe, startIndex ) );
		}

  public:
		core_forceinline char operator [] ( int i ) const
		{
      String t = c_str();
      return t[i];
		}
		
	private:
		void makeCopy( const FixedString& other )
		{
			memcpy( &( mData[0] ), &( other.mData[0] ), TOTAL_CHARS );
		}
	
	private:
		intType mData[INT_SIZE];
		
};

template< int TOTAL_CHARS >
class FixedStringLexLess : public binary_function< FixedString< TOTAL_CHARS >, FixedString< TOTAL_CHARS >, bool >
{
	public:
		bool operator ()( const FixedString< TOTAL_CHARS >& a, const FixedString< TOTAL_CHARS >& b ) const
		{
			return a.lexLess( b );
		}
				
};

typedef FixedString< 8 >    FixedString8;
typedef FixedString< 16 >   FixedString16;
typedef FixedString< 32 >   FixedString32;
typedef FixedString< 64 >   FixedString64;
typedef FixedString< 128 >  FixedString128;

typedef std::set< FixedString8,    FixedStringLexLess< 8 > >    FixedString8LexLessSetType;
typedef std::set< FixedString16,   FixedStringLexLess< 16 > >   FixedString16LexLessSetType;
typedef std::set< FixedString32,   FixedStringLexLess< 32 > >   FixedString32LexLessSetType;
typedef std::set< FixedString64,   FixedStringLexLess< 64 > >   FixedString64LexLessSetType;
typedef std::set< FixedString128,  FixedStringLexLess< 128 > >  FixedString128LexLessSetType;

} }

#endif
