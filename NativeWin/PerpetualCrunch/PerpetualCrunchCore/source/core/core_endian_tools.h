#if !defined ( CORE_ENDIAN_TOOLS_H )
#define CORE_ENDIAN_TOOLS_H

#if !defined _CRUNCH_LITTLE_ENDIAN
	#define _CRUNCH_LITTLE_ENDIAN 1
#endif

#if !defined _CRUNCH_BIG_ENDIAN
	#define _CRUNCH_BIG_ENDIAN 0
#endif

namespace crunch { namespace core {

namespace EndianTools
{
	template < typename value_t >
	[[nodiscard]]
	core_forceinline value_t swap32( value_t _t )
	{
    unsigned int t = *(unsigned int*)( &_t );

		return ( ( ( t & 0xFF000000 ) >> 24 ) | ( ( t & 0x000000FF ) << 24 ) | ( ( t & 0x00FF0000 ) >> 8 ) | ( ( t & 0x0000FF00 ) << 8 ) );
	}

	core_forceinline void swapUInt32( unsigned int *value )
	{
		*value = swap32< unsigned int >( *value );
	}

	template < typename value_t >
	[[nodiscard]]
	core_forceinline value_t swap64( value_t _t )
	{
    core_u64 t = *(core_u64*)( &_t );

		return (   ( ( t & 0xFF00000000000000 ) >> 56 )
		         | ( ( t & 0x00000000000000FF ) << 56 )
		         | ( ( t & 0x00FF000000000000 ) >> 40 )
		         | ( ( t & 0x000000000000FF00 ) << 40 )
		         | ( ( t & 0x0000FF0000000000 ) >> 24 )
		         | ( ( t & 0x0000000000FF0000 ) << 24 )
		         | ( ( t & 0x000000FF00000000 ) >> 8 )
		         | ( ( t & 0x00000000FF000000 ) << 8 )
		         );
	}

	core_forceinline void swapUInt64( core_u64 *value ) // __int64 is ms-specific, no port way to declare this type until C++11
	{
		*value = swap64< core_u64 >( *value );
	}

	template < typename value_t >
	[[nodiscard]]
	core_forceinline value_t swap16( value_t _t )
	{
    unsigned short t = *(unsigned short*)( &_t );

		return ( ( ( t & 0xFF00 ) >> 8 ) | ( ( t & 0xFF ) << 8 ) );
	}
	
	core_forceinline void swapUInt16( unsigned short *value )
	{	
		*value = swap16< unsigned short >( *value );
	}

  core_forceinline void swapInt64( core_i64 *value )
  {
    swapUInt64( (core_u64*)( value ) );
  }
	
	core_forceinline void swapInt32( int *value )
	{
		swapUInt32( (unsigned int*)( value ) );
	}
	
	core_forceinline void swapInt16( short *value )
	{
		swapUInt16( (unsigned short*)( value ) );
	}
	
	core_forceinline void swapFloat( float *value )
	{
		assert( value );
		swapUInt32( (unsigned int*)( value ) );
	}

	core_forceinline void swapDouble( double *value )
	{
		assert( value );
		swapUInt64( (core_u64*)( value ) );
	}

  // not to be used with a struct/class
  // intended for numeric types (eg. int, float, double)
  template < typename T >
  core_forceinline void swapValue( T *v )
  {
    unsigned int totalBytes = sizeof( T );
    if ( totalBytes <= 1 || ( totalBytes % 2 ) != 0 )
      return;

    unsigned char tv[sizeof( T )];
    memcpy( tv, v, totalBytes );

    int totalSwaps = totalBytes / 2;

    for ( int i = 0; i < totalSwaps; ++i )
    {
      unsigned char temp = tv[i];
      int mi = totalBytes - i - 1;
      tv[i] = tv[mi];
      tv[mi] = temp;
    }

    memcpy( v, tv, totalBytes );
  }
}

} }

#endif
