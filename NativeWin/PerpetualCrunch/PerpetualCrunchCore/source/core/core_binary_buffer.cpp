
#include "core_global.h"

#include "core_mutex.h"
#include "core_endian_tools.h"
#include "core_file_tools.h"

#include "core_binary_buffer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// defines

// #define ENABLE_BINARY_BUFFER_PRINT

#if defined ENABLE_BINARY_BUFFER_PRINT
  #define bbPrint corePrint
#else
	#if defined( __GNUC__ )
		#define bbPrint(...) (void)(0)
	#elif defined( _MSC_VER )
		#define bbPrint __noop
	#else
		#define bbPrint
	#endif
#endif

#define BINARY_BUFFER_MAX_STRING_LENGTH 32895

#define BB_WCHAR_T unsigned short

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace core {

static unsigned int bb_wcslen( BB_WCHAR_T *wstr )
{
  unsigned int totalChars = 0;
  
  BB_WCHAR_T *search = wstr;
  while ( *search != 0 )
  {
    ++totalChars;
    ++search;
  }

  return totalChars;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// BinaryBuffer

BinaryBuffer::BinaryBuffer() :
	mBuffer( nullptr ),
	mBufferSize( 0 ),
	mBufferExpansionSize( 0 ),
	mBufferOffset( 0 ),
	mFlags( 0 )
{
	// empty
}

BinaryBuffer::~BinaryBuffer()
{
	destroy();
}

BinaryBuffer::BinaryBuffer( const BinaryBuffer& other )
{
  makeCopy( other );
}

BinaryBuffer& BinaryBuffer::operator = ( const BinaryBuffer& other )
{
  if ( this == &other )
    return *this;

  destroy();
  makeCopy( other );

  return *this;
}

void BinaryBuffer::destroy()
{
	if ( mBuffer )
	{
		if ( ( mFlags & FLAG_SHARED_IMAGE ) == 0 )
    {
      // cruchLog( "deleting the buffer | mBufferSize: %u", mBufferSize );
			delete[] mBuffer;
    }

		mBuffer = nullptr;
	}
	
	mBufferSize = 0;
	mBufferExpansionSize = 0;
	mBufferOffset = 0;
	mFlags = 0;
}

bool BinaryBuffer::setup( unsigned int bufferSize, unsigned int expansionSize )
{
	destroy();
	
	mBufferSize = bufferSize;
	mBufferExpansionSize = expansionSize;
	
	mBuffer = new char[mBufferSize];
	assert( mBuffer );
	
	assert( mBufferOffset == 0 );
	
	return ( mBuffer != nullptr );
}

bool BinaryBuffer::readFile( const char *filename, unsigned int expansionSize )
{
	destroy();

	FILE *source = core_fopen( filename, "rb" );
	if ( source == nullptr )
  {
    // bbPrint( "failed to open for reading '%s'", filename );
		return false;
  }

  return readFile( source, WideString( String( filename ) ), expansionSize );	
}

bool BinaryBuffer::readFile( const wchar_t *filename, unsigned int expansionSize )
{
	destroy();

	FILE *source = core_fwopen( filename, L"rb" );
	if ( source == nullptr )
  {
    // bbPrint( "failed to open for reading '%s'", stringx::wstos( filename ).c_str() );
	  return false;
  }
	
  return readFile( source, WideString( filename ), expansionSize );
}

bool BinaryBuffer::readFile( const char *filename, std::vector< unsigned char > *dest )
{
  BinaryBuffer b;

  if ( !( b.readFile( filename ) ) )
    return false;

  if ( b.getBufferSize() == 0 )
  {
    dest->clear();
    return true;
  }

  dest->resize( b.getBufferSize() );

  memcpy( &( ( *dest )[0] ), b.c_str(), b.getBufferSize() );

  return true;
}

bool BinaryBuffer::readFile( const wchar_t *filename, std::vector< unsigned char > *dest )
{
  BinaryBuffer b;

  if ( !( b.readFile( filename ) ) )
    return false;

  if ( b.getBufferSize() == 0 )
  {
    dest->clear();
    return true;
  }

  dest->resize( b.getBufferSize() );

  memcpy( &( ( *dest )[0] ), b.c_str(), b.getBufferSize() );

  return true;
}

bool BinaryBuffer::writeFile( const char *filename ) const
{
	FILE *dest = core_fopen( filename, "wb" );
	if ( dest == nullptr )
  {
    bbPrint( "failed to open '%s' for writing", filename );
		return false;
  }
	
	bool retVal = true;
	
	if ( mBufferOffset > 0 )
	{
		if ( fwrite( mBuffer, 1, mBufferOffset, dest ) != mBufferOffset )
			retVal = false;
	}
	
	fclose( dest );
	
	return true;
}

bool BinaryBuffer::writeFile( const wchar_t *filename ) const
{
	FILE *dest = core_fwopen( filename, L"wb" );
	if ( dest == nullptr )
  {
    // bbPrint( "failed to open file | name: '%s'", stringx::wstos( filename ).c_str() );
	  return false;
  }
	
	bool retVal = true;
	
	if ( mBufferOffset > 0 )
	{
		if ( fwrite( mBuffer, 1, mBufferOffset, dest ) != mBufferOffset )
    {
			retVal = false;
      bbPrint( "failed to write file | size: %u | name: '%s'", mBufferOffset, String::wstos( filename ).c_str() );
    }
	}
	
	fclose( dest );
	
	return retVal;
}

bool BinaryBuffer::writeFile( const char *filename, const std::vector< unsigned char >& source )
{
  unsigned int bufferSize = (unsigned int)( source.size() );

  BinaryBuffer b;

  if ( !( b.setup( bufferSize, 0 ) ) )
    return false;

  if ( !( b.append_raw( &( source[0] ), bufferSize ) ) )
    return false;

  return b.writeFile( filename );
}

bool BinaryBuffer::writeFile( const wchar_t *filename, const std::vector< unsigned char >& source )
{
  unsigned int bufferSize = (unsigned int)( source.size() );

  BinaryBuffer b;

  if ( !( b.setup( bufferSize, 0 ) ) )
    return false;

  if ( !( b.append_raw( &( source[0] ), bufferSize ) ) )
    return false;

  return b.writeFile( filename );
}

bool BinaryBuffer::appendFile( const char *filename, bool clearBuffer )
{
	FILE *dest = core_fopen( filename, "ab" );
	if ( dest == nullptr )
		return false;
	
	bool retVal = true;
	
	if ( mBufferOffset > 0 )
	{
		if ( fwrite( mBuffer, 1, mBufferOffset, dest ) != mBufferOffset )
    {
			retVal = false;

      #if defined _CRUNCH_DEBUG
      bbPrint( "failed to write file; size: %u, name: '%s'", mBufferOffset, filename );
      #endif
    }
	}
	
	fclose( dest );

	if ( clearBuffer )
		mBufferOffset = 0;
	
	return true;
}

bool BinaryBuffer::appendFile( const wchar_t *filename, bool clearBuffer )
{
	FILE *dest = core_fwopen( filename, L"ab" );
	if ( dest == nullptr )
	  return false;
	
	bool retVal = true;
	
	if ( mBufferOffset > 0 )
	{
		if ( fwrite( mBuffer, 1, mBufferOffset, dest ) != mBufferOffset )
    {
			retVal = false;

      #if defined _CRUNCH_DEBUG
      bbPrint( "failed to write file; size: %u, name: '%s'", mBufferOffset, String::wstos( filename ).c_str() );
      #endif
    }
	}
	
	fclose( dest );

	if ( clearBuffer )
		mBufferOffset = 0;
	
	return true;
}

bool BinaryBuffer::setImage( char *image, unsigned int imageSize )
{
	destroy();

	if ( image == nullptr )
		return false;

	mFlags |= FLAG_SHARED_IMAGE;

	mBuffer = image;
	mBufferSize = imageSize;

	return true;
}

bool BinaryBuffer::append_bool( bool b )
{
	return append_uint8( b ? 1 : 0 );
}

bool BinaryBuffer::append_int64( int64_t i )
{
	if ( !( updateBufferSize( sizeof( int64_t ) ) ) )
		return false;

	if ( isEndianConvertOnAppend() )
		EndianTools::swapInt64( &i );
	
	memcpy( &( mBuffer[mBufferOffset] ), &i, sizeof( int64_t ) );
	mBufferOffset += sizeof( int64_t );
	
	return true;
}

bool BinaryBuffer::append_uint64( uint64_t u )
{
	if ( !( updateBufferSize( sizeof( uint64_t ) ) ) )
		return false;
	
	if ( isEndianConvertOnAppend() )
		EndianTools::swapUInt64( &u );
	
	memcpy( &( mBuffer[mBufferOffset] ), &u, sizeof( uint64_t ) );
	mBufferOffset += sizeof( uint64_t );
	
	return true;
}

bool BinaryBuffer::append_int32( int32_t i )
{
	if ( !( updateBufferSize( sizeof( int32_t ) ) ) )
		return false;

	if ( isEndianConvertOnAppend() )
		EndianTools::swapInt32( &i );
	
	memcpy( &( mBuffer[mBufferOffset] ), &i, sizeof( int32_t ) );
	mBufferOffset += sizeof( int32_t );
	
	return true;
}

bool BinaryBuffer::append_uint32( uint32_t u )
{
	if ( !( updateBufferSize( sizeof( uint32_t ) ) ) )
		return false;
	
	if ( isEndianConvertOnAppend() )
		EndianTools::swapUInt32( &u );
	
	memcpy( &( mBuffer[mBufferOffset] ), &u, sizeof( uint32_t ) );
	mBufferOffset += sizeof( uint32_t );
	
	return true;
}

bool BinaryBuffer::append_int16( int16_t i )
{
	if ( !( updateBufferSize( sizeof( int16_t ) ) ) )
		return false;

	if ( isEndianConvertOnAppend() )
		EndianTools::swapInt16( &i );
	
	memcpy( &( mBuffer[mBufferOffset] ), &i, sizeof( int16_t ) );
	mBufferOffset += sizeof( int16_t );
	
	return true;
}

bool BinaryBuffer::append_uint16( uint16_t u )
{
	if ( !( updateBufferSize( sizeof( uint16_t ) ) ) )
		return false;
	
	if ( isEndianConvertOnAppend() )
		EndianTools::swapUInt16( &u );
	
	memcpy( &( mBuffer[mBufferOffset] ), &u, sizeof( uint16_t ) );
	mBufferOffset += sizeof( uint16_t );
	
	return true;
}

bool BinaryBuffer::append_int8( int8_t i )
{
	if ( !( updateBufferSize( sizeof( int8_t ) ) ) )
		return false;
	
	memcpy( &( mBuffer[mBufferOffset] ), &i, sizeof( int8_t ) );
	mBufferOffset += sizeof( int8_t );
	
	return true;
}

bool BinaryBuffer::append_uint8( uint8_t u )
{
	if ( !( updateBufferSize( sizeof( uint8_t ) ) ) )
		return false;
	
	memcpy( &( mBuffer[mBufferOffset] ), &u, sizeof( uint8_t ) );
	mBufferOffset += sizeof( uint8_t );
	
	return true;
}

bool BinaryBuffer::append_String( const char *str )
{
	int strBufferSize = computeStringBufferSize( str );
	if ( !( updateBufferSize( strBufferSize ) ) )
		return false;
	
	assert( ( mBufferOffset + strBufferSize ) <= mBufferSize );
	memcpy( &( mBuffer[mBufferOffset] ), str, strBufferSize );
	mBufferOffset += strBufferSize;
	
	return true;
}

bool BinaryBuffer::append_WideString( const wchar_t *str )
{
  // TODO: possibly need to respect endianess here, since sizeof( BB_WCHAR_T ) expected > 1

	int strBufferSize = computeStringBufferSize( str );
	if ( !( updateBufferSize( strBufferSize ) ) )
		return false;
	
	assert( ( mBufferOffset + strBufferSize ) <= mBufferSize );
	
	if ( sizeof( wchar_t ) == sizeof( BB_WCHAR_T ) )
	  memcpy( &( mBuffer[mBufferOffset] ), str, strBufferSize );
	else
	{
    unsigned int totalChars = (unsigned int)( wcslen( str ) );
    
    BB_WCHAR_T *tempString = (BB_WCHAR_T*)( malloc( ( totalChars + 1 ) * sizeof( BB_WCHAR_T ) ) );
    
    for ( unsigned int i = 0; i < totalChars; ++i )
      tempString[i] = str[i];
    
    tempString[totalChars] = 0;
    
    memcpy( &( mBuffer[mBufferOffset] ), tempString, strBufferSize );
    
    free( tempString );
	}
	
	mBufferOffset += strBufferSize;
	
	return true;
}

bool BinaryBuffer::append_FixedString32( const FixedString32& str )
{
	if ( !( updateBufferSize( sizeof( FixedString32 ) ) ) )
		return false;
	
	memcpy( &( mBuffer[mBufferOffset] ), &str, sizeof( FixedString32 ) );
	mBufferOffset += sizeof( FixedString32 );
	
	return true;
}

bool BinaryBuffer::append_FixedString64( const FixedString64& str )
{
	if ( !( updateBufferSize( sizeof( FixedString64 ) ) ) )
		return false;
	
	memcpy( &( mBuffer[mBufferOffset] ), &str, sizeof( FixedString64 ) );
	mBufferOffset += sizeof( FixedString64 );
	
	return true;
}

bool BinaryBuffer::append_FixedString128( const FixedString128& str )
{
	if ( !( updateBufferSize( sizeof( FixedString128 ) ) ) )
		return false;
	
	memcpy( &( mBuffer[mBufferOffset] ), &str, sizeof( FixedString128 ) );
	mBufferOffset += sizeof( FixedString128 );
	
	return true;
}

bool BinaryBuffer::append_float( float f )
{
	if ( !( updateBufferSize( sizeof( float ) ) ) )
		return false;
	
	if ( isEndianConvertOnAppend() )
		EndianTools::swapFloat( &f );
	
	memcpy( &( mBuffer[mBufferOffset] ), &f, sizeof( float ) );
	mBufferOffset += sizeof( float );
	
	return true;
}

bool BinaryBuffer::append_double( double d )
{
	if ( !( updateBufferSize( sizeof( double ) ) ) )
		return false;
	
	if ( isEndianConvertOnAppend() )
		EndianTools::swapDouble( &d );
	
	memcpy( &( mBuffer[mBufferOffset] ), &d, sizeof( double ) );
	mBufferOffset += sizeof( double );
	
	return true;
}

template < typename T >
bool BinaryBuffer::append_number( T v )
{
	if ( !( updateBufferSize( sizeof( T ) ) ) )
		return false;
	
	if ( isEndianConvertOnAppend() )
    EndianTools::swapValue< T >( &v );
	
	memcpy( &( mBuffer[mBufferOffset] ), &v, sizeof( T ) );
	mBufferOffset += sizeof( T );
	
	return true;
}

bool BinaryBuffer::append_buffer( const BinaryBuffer& source )
{
	unsigned int sourceSize = source.getBufferLength();
	
	if ( !( updateBufferSize( sourceSize ) ) )
		return false;
	
	memcpy( &( mBuffer[mBufferOffset] ), source.c_str(), sourceSize );
	mBufferOffset += sourceSize;
	
	return true;
}

bool BinaryBuffer::append_raw( const void *buffer, unsigned int bufferSize )
{
	if ( buffer == nullptr )
  {
    bbPrint( "no buffer" );
		return false;
  }

	if ( !( updateBufferSize( bufferSize ) ) )
  {
    bbPrint( "failed to update the buffer size" );
		return false;
  }
	
	memcpy( &( mBuffer[mBufferOffset] ), buffer, bufferSize );
	mBufferOffset += bufferSize;

	return true;
}

bool BinaryBuffer::read_bool()
{
	int i = read_uint8();
	return ( i != 0 );
}

int64_t BinaryBuffer::read_int64()
{
  if ( mBufferOffset >= mBufferSize )
  {
    setFlag( FLAG_READ_EXCEEDED_END_OF_BUFFER, true );
    return 0;
  }

	int64_t retVal;
	memcpy( &retVal, &( mBuffer[mBufferOffset] ), sizeof( int64_t ) );
	mBufferOffset += sizeof( int64_t );
	assert( mBufferOffset <= mBufferSize );
	return retVal;
}

uint64_t BinaryBuffer::read_uint64()
{
  if ( mBufferOffset >= mBufferSize )
  {
    setFlag( FLAG_READ_EXCEEDED_END_OF_BUFFER, true );
    return 0;
  }

	uint64_t retVal;
	memcpy( &retVal, &( mBuffer[mBufferOffset] ), sizeof( uint64_t ) );
	mBufferOffset += sizeof( uint64_t );
	assert( mBufferOffset <= mBufferSize && "BinaryBuffer::read_uint64" );
	return retVal;
}

int32_t BinaryBuffer::read_int32()
{
  if ( mBufferOffset >= mBufferSize )
  {
    setFlag( FLAG_READ_EXCEEDED_END_OF_BUFFER, true );
    return 0;
  }

	int32_t retVal;
	memcpy( &retVal, &( mBuffer[mBufferOffset] ), sizeof( int32_t ) );
	mBufferOffset += sizeof( int32_t );
	assert( mBufferOffset <= mBufferSize );
	return retVal;
}

uint32_t BinaryBuffer::read_uint32()
{
  if ( mBufferOffset >= mBufferSize )
  {
    setFlag( FLAG_READ_EXCEEDED_END_OF_BUFFER, true );
    return 0;
  }

	uint32_t retVal;
	memcpy( &retVal, &( mBuffer[mBufferOffset] ), sizeof( uint32_t ) );
	mBufferOffset += sizeof( uint32_t );
	assert( mBufferOffset <= mBufferSize && "BinaryBuffer::read_uint32" );
	return retVal;
}

int16_t BinaryBuffer::read_int16()
{
  if ( mBufferOffset >= mBufferSize )
  {
    setFlag( FLAG_READ_EXCEEDED_END_OF_BUFFER, true );
    return 0;
  }

	int16_t retVal;
	memcpy( &retVal, &( mBuffer[mBufferOffset] ), sizeof( int16_t ) );
	mBufferOffset += sizeof( int16_t );
	assert( mBufferOffset <= mBufferSize );
	return retVal;
}

uint16_t BinaryBuffer::read_uint16()
{
  if ( mBufferOffset >= mBufferSize )
  {
    setFlag( FLAG_READ_EXCEEDED_END_OF_BUFFER, true );
    return 0;
  }

	uint16_t retVal;
	memcpy( &retVal, &( mBuffer[mBufferOffset] ), sizeof( uint16_t ) );
	mBufferOffset += sizeof( uint16_t );
	assert( mBufferOffset <= mBufferSize );
	return retVal;
}

int8_t BinaryBuffer::read_int8()
{
  if ( mBufferOffset >= mBufferSize )
  {
    setFlag( FLAG_READ_EXCEEDED_END_OF_BUFFER, true );
    return 0;
  }

	int8_t retVal = *(int8_t*)( &( mBuffer[mBufferOffset] ) );
	mBufferOffset += sizeof( int8_t );
	assert( mBufferOffset <= mBufferSize );
	return retVal;
}

uint8_t BinaryBuffer::read_uint8()
{
  if ( mBufferOffset >= mBufferSize )
  {
    setFlag( FLAG_READ_EXCEEDED_END_OF_BUFFER, true );
    return 0;
  }

	uint8_t retVal = *(uint8_t*)( &( mBuffer[mBufferOffset] ) );
	mBufferOffset += sizeof( uint8_t );
	assert( mBufferOffset <= mBufferSize );
	return retVal;
}

String BinaryBuffer::read_String()
{
  if ( mBufferOffset >= mBufferSize )
  {
    setFlag( FLAG_READ_EXCEEDED_END_OF_BUFFER, true );
    return "";
  }

	char *retVal = (char*)( &( mBuffer[mBufferOffset] ) );
	mBufferOffset += computeStringBufferSize( retVal );
	assert( mBufferOffset <= mBufferSize );
	return retVal;
}

WideString BinaryBuffer::read_WideString()
{
  if ( mBufferOffset >= mBufferSize )
  {
    setFlag( FLAG_READ_EXCEEDED_END_OF_BUFFER, true );
    return L"";
  }

  // NOTE: the size of a wchar_t in the buffer is always 2 bytes, so must convert to the platform as needed
  
  BB_WCHAR_T *fileString = (BB_WCHAR_T*)( &( mBuffer[mBufferOffset] ) );
  
  WideString retVal;
  
  if ( sizeof( wchar_t ) == sizeof( BB_WCHAR_T ) )
    retVal = WideString( (wchar_t*)( fileString ) );
  else
  {
    unsigned int totalChars = bb_wcslen( fileString );
    
    wchar_t *tempString = (wchar_t*)( malloc( ( totalChars + 1 ) * sizeof( wchar_t ) ) );
    
    for ( unsigned int i = 0; i < totalChars; ++i )
      tempString[i] = fileString[i];
    
    tempString[totalChars] = 0;
    
    retVal = WideString( tempString );
    
    free( tempString );
  }
  
	mBufferOffset += computeStringBufferSize( retVal.wc_str() );
	assert( mBufferOffset <= mBufferSize );
	return retVal;
}

FixedString32 BinaryBuffer::read_FixedString32()
{
  if ( mBufferOffset >= mBufferSize )
  {
    setFlag( FLAG_READ_EXCEEDED_END_OF_BUFFER, true );
    return "";
  }

  FixedString32 retVal;
  memcpy( (void*)( retVal.c_str() ), &( mBuffer[mBufferOffset] ), sizeof( FixedString32 ) );
	mBufferOffset += sizeof( FixedString32 );
	assert( mBufferOffset <= mBufferSize );
	return retVal;
}

FixedString64 BinaryBuffer::read_FixedString64()
{
  if ( mBufferOffset >= mBufferSize )
  {
    setFlag( FLAG_READ_EXCEEDED_END_OF_BUFFER, true );
    return "";
  }

  FixedString64 retVal;
  memcpy( (void*)( retVal.c_str() ), &( mBuffer[mBufferOffset] ), sizeof( FixedString64 ) );
	mBufferOffset += sizeof( FixedString64 );
	assert( mBufferOffset <= mBufferSize && "BinaryBuffer::read_fstring64" );
	return retVal;
}

FixedString128 BinaryBuffer::read_FixedString128()
{
  if ( mBufferOffset >= mBufferSize )
  {
    setFlag( FLAG_READ_EXCEEDED_END_OF_BUFFER, true );
    return "";
  }

  FixedString128 retVal;
  memcpy( (void*)( retVal.c_str() ), &( mBuffer[mBufferOffset] ), sizeof( FixedString128 ) );
	mBufferOffset += sizeof( FixedString128 );
	assert( mBufferOffset <= mBufferSize );
	return retVal;
}

float BinaryBuffer::read_float()
{
  if ( mBufferOffset >= mBufferSize )
  {
    setFlag( FLAG_READ_EXCEEDED_END_OF_BUFFER, true );
    return 0.0f;
  }

	float retVal;
	memcpy( &retVal, &( mBuffer[mBufferOffset] ), sizeof( float ) );
	mBufferOffset += sizeof( float );
	assert( mBufferOffset <= mBufferSize );
	return retVal;
}

double BinaryBuffer::read_double()
{
  if ( mBufferOffset >= mBufferSize )
  {
    setFlag( FLAG_READ_EXCEEDED_END_OF_BUFFER, true );
    return 0.0;
  }

	double retVal;
	memcpy( &retVal, &( mBuffer[mBufferOffset] ), sizeof( double ) );
	mBufferOffset += sizeof( double );
	assert( mBufferOffset <= mBufferSize );
	return retVal;
}

template < typename T >
T BinaryBuffer::read_number()
{
  if ( mBufferOffset >= mBufferSize )
  {
    setFlag( FLAG_READ_EXCEEDED_END_OF_BUFFER, true );
    return (T)( 0 );
  }

	T retVal;
	memcpy( &retVal, &( mBuffer[mBufferOffset] ), sizeof( T ) );
	mBufferOffset += sizeof( T );
	assert( mBufferOffset <= mBufferSize );
	return retVal;
}

char* BinaryBuffer::c_str() const
{
	return mBuffer;
}

char* BinaryBuffer::c_str_offset() const
{
	return &( mBuffer[mBufferOffset] );
}

bool BinaryBuffer::setBufferOffset( unsigned int bufferOffset )
{
	if ( bufferOffset > mBufferSize )
		return false;

	mBufferOffset = bufferOffset;

	return true;
}

bool BinaryBuffer::removeFront( unsigned int amount )
{
  if ( amount > mBufferOffset )
    return false;

  BinaryBuffer tempBuffer;
  tempBuffer.setup( mBufferOffset, 0 );
  tempBuffer.append_raw( &( mBuffer[0] ), mBufferOffset );

  memcpy( &( mBuffer[0] ), &( tempBuffer.mBuffer[amount] ), mBufferOffset - amount );

  mBufferOffset -= amount;

  return true;
}

bool BinaryBuffer::seekFromCurrent( unsigned int amount )
{
	if ( ( mBufferOffset + amount ) > mBufferSize )
		return false;

	mBufferOffset += amount;

	return true;
}

bool BinaryBuffer::updateBufferSize( unsigned int delta )
{
	unsigned int desiredBufferSize = mBufferOffset + delta;

	if ( desiredBufferSize <= mBufferSize )
		return true;
		
	if ( mBufferExpansionSize == 0 )
  {
    bbPrint( "cannot expand buffer, buffer expansion size is zero" );
		return false;
  }
		
	char *origBuffer = mBuffer;
	unsigned int origBufferSize = mBufferSize;
		
	unsigned int overflowSize = desiredBufferSize - mBufferSize;
	
	unsigned int expansions = overflowSize / mBufferExpansionSize;
	
  unsigned int foundBufferSize = mBufferSize;
	if ( ( overflowSize % mBufferExpansionSize ) == 0 )
		foundBufferSize += ( expansions * mBufferExpansionSize );
	else
		foundBufferSize += ( ( expansions * mBufferExpansionSize ) + mBufferExpansionSize );
		
  if ( foundBufferSize < mBufferSize )
    return false; // overflow

  mBufferSize = foundBufferSize;

	assert( desiredBufferSize <= mBufferSize );
	
	mBuffer = new char[mBufferSize];
	assert( mBuffer );
	
  if ( origBuffer != nullptr )
	  memcpy( mBuffer, origBuffer, origBufferSize );
	
	if ( ( mFlags & FLAG_SHARED_IMAGE ) == 0 && origBuffer != nullptr )
		delete[] origBuffer;
	mFlags &= ~FLAG_SHARED_IMAGE;
	
	return true;
}

int BinaryBuffer::computeStringBufferSize( const char *str )
{
	return ( int( strlen( str ) ) + 1 );
}

int BinaryBuffer::computeStringBufferSize( const wchar_t *str )
{
	return ( ( int( wcslen( str ) ) * sizeof( BB_WCHAR_T ) ) + sizeof( BB_WCHAR_T ) );
}

bool BinaryBuffer::readFile( FILE *source, const WideString& filename, unsigned int expansionSize )
{
	fseek( source, 0, SEEK_END );
  long rawSize = ftell( source );
	fseek( source, 0, SEEK_SET );

  if ( rawSize == -1L )
  {
    // bbPrint( "failed to retrieve size of file | sizeof( long ): %u | filename: '%s'", sizeof( long ), filename.strx().c_str() );

    const int LOCAL_BUFFER_SIZE = 1024 * 1024 * 128;
    char *localBuffer = (char*)( malloc( LOCAL_BUFFER_SIZE ) );
    assert( localBuffer && "failed to allocate local buffer" );

    mBufferExpansionSize = expansionSize;
    if ( mBufferExpansionSize == 0 )
      mBufferExpansionSize = LOCAL_BUFFER_SIZE * 4;

    unsigned int bytesRead;
    bool retVal = true;

    while ( true )
    {
      bytesRead = (unsigned int)( fread( localBuffer, 1, LOCAL_BUFFER_SIZE, source ) );
      if ( bytesRead > 0 )
      {
        if ( !( append_raw( localBuffer, bytesRead ) ) )
        {
          retVal = false;
          break;
        }
      }

      if ( bytesRead < LOCAL_BUFFER_SIZE )
        break;

      // bbPrint( "reading large file | mBufferSize: %u", mBufferSize );
    }

    fclose( source );
    free( localBuffer );

    setBufferOffset( 0 );

    /*
    bbPrint( "read large file | result: %s | mBufferSize: %u | filename: '%s'", 
               retVal ? "true" : "false", 
               mBufferSize, 
               filename.strx().c_str() 
               );
    //*/

    return retVal;
  }
	
  mBufferSize = (unsigned int)( rawSize );

	mBufferExpansionSize = expansionSize;
	
  bool retVal = true;

	mBuffer = new char[mBufferSize];
	if ( mBuffer == nullptr )
  {
    bbPrint( "[ERROR] failed to allocate buffer | buffer size: %u bytes (%0.1f MB)", mBufferSize, float( mBufferSize ) / 1024.0f / 1024.0f );
    retVal = false;  
  }
	else
	if ( fread( mBuffer, 1, mBufferSize, source ) != mBufferSize )
  {
    bbPrint( "[ERROR] failed to read file | mBufferSize: %u | filename: '%s'", mBufferSize, filename.toString().c_str() );
		retVal = false;
  }
	
	fclose( source );
	
	return retVal;
}

void BinaryBuffer::makeCopy( const BinaryBuffer& other )
{
  if ( ( other.mFlags & FLAG_SHARED_IMAGE ) != 0 || other.mBufferSize == 0 )
  {
	  mBuffer = other.mBuffer;
	  mBufferSize = other.mBufferSize;
    mBufferExpansionSize = other.mBufferExpansionSize;
    mBufferOffset = other.mBufferOffset;
	  mFlags = other.mFlags;
  }
  else
  {
	  mBufferSize = other.mBufferSize;
	  mBufferExpansionSize = other.mBufferExpansionSize;
    mBufferOffset = other.mBufferOffset;
	
	  mBuffer = new char[mBufferSize];
	  assert( mBuffer );
    memcpy( &mBuffer[0], &( other.mBuffer[0] ), mBufferSize );

    mFlags = other.mFlags;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

template bool BinaryBuffer::append_number< float >( float v );
template bool BinaryBuffer::append_number< double >( double v );

template float BinaryBuffer::read_number< float >();
template double BinaryBuffer::read_number< double >();

} }
