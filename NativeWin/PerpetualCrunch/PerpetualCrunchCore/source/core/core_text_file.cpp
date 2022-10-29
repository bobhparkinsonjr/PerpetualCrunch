
#include "core_global.h"

#include "core_file_tools.h"

#include "core_text_file.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace core {

TextFile::TextFile() :
	mFile( nullptr ),
	mBuffer( nullptr ),
	mBufferOffset( 0 ),
	mBufferSize( 0 ),
  mFlags( 0 )
{
	// empty
}

TextFile::~TextFile()
{
	close();
}

bool TextFile::open( const WideString& filename, int mode, bool allowBuffering, unsigned int flags )
{
	close();

  mFlags = flags;

	const wchar_t *modeString;

	switch ( mode )
	{
		case MODE_READ:
			modeString = L"rt";
		break;
		
		case MODE_WRITE:
			modeString = L"wt";
		break;
		
		case MODE_APPEND:
			modeString = L"at";
		break;
	
		default:
			return false;
		break;
	}
	
	mFile = core_fwopen( filename.wc_str(), modeString );
	if ( mFile == nullptr )
	  return false;
		
	if ( mode == MODE_READ && allowBuffering )
	{
		assert( mBufferSize == 0 );
		assert( mBufferOffset == 0 );
		assert( mBuffer == nullptr );
		
		fseek( mFile, 0, SEEK_END );
		mBufferSize = ftell( mFile );
		fseek( mFile, 0, SEEK_SET );
		
		if ( mBufferSize > 0 )
		{
			mBuffer = (char*)( malloc( mBufferSize + 1 ) );
			if ( mBuffer == nullptr )
				mBufferSize = 0;
			else
			{
				size_t freadRetVal = fread( mBuffer, 1, mBufferSize, mFile );
			
				if ( freadRetVal < mBufferSize && feof( mFile ) == 0 )
					return false;
				
				mBufferSize = (unsigned int)( freadRetVal );
			}
		}
	}
	
	return true;
}

bool TextFile::atEof() const
{
	if ( mFile == nullptr && mBuffer == nullptr )
		return true;
		
	if ( mBuffer )
		return ( mBufferOffset >= mBufferSize );
		
	return ( feof( mFile ) != 0 );
}

void TextFile::close()
{
	if ( mFile )
	{
		fclose( mFile );
		mFile = nullptr;
	}
	
	if ( mBuffer )
	{
		free( mBuffer );
		mBuffer = nullptr;
		mBufferOffset = 0;
		mBufferSize = 0;
	}

  mFlags = 0;
}

bool TextFile::readLine( String *line )
{
	assert( line );
	
	if ( atEof() )
		return false;

	if ( mBuffer != nullptr )
	{
		char foundCh = 0;
		const char *str = &( mBuffer[mBufferOffset] );
		
		while ( true )
		{
			if ( mBufferOffset >= mBufferSize )
			{
				assert( mBufferOffset == mBufferSize );
				mBuffer[mBufferSize] = 0;
				break;
			}

			++mBufferOffset;
			
			if ( mBuffer[mBufferOffset] == '\n' || mBuffer[mBufferOffset] == '\r' )
			{
				foundCh = mBuffer[mBufferOffset];
				mBuffer[mBufferOffset] = 0;
				break;
			}
		}
	
		*line = str;

		if ( foundCh != 0 )
			mBuffer[mBufferOffset] = foundCh;
	}
	else
	{
		const int BUFFER_SIZE = 8192;
		char buffer[BUFFER_SIZE];
	
		int bufferOffset = 0;
		
		while ( true )
		{
			char *retVal = fgets( &( buffer[bufferOffset] ), 2, mFile );
			if ( retVal == nullptr )
				break;
		
			if ( buffer[bufferOffset] == '\n' || buffer[bufferOffset] == '\r' )
				break;
		
			++bufferOffset;
			if ( bufferOffset >= ( BUFFER_SIZE - 1 ) )
				break;
		}
	
		buffer[bufferOffset] = 0;
		
		*line = buffer;
	}
	
	return true;
}

bool TextFile::write( const std::string& text )
{
	if ( mFile )
	{
		core_fprintf( mFile, "%s", text.c_str() );

    if ( isFlushOnWrite() )
		  flush();

		return true;
	}
	
	return false;
}

bool TextFile::writeFormatted( const char *fmt, ... )
{
	if ( mFile )
	{
		const int WORK_SIZE = 8192;
		char work[WORK_SIZE];
		
		va_list args;
		va_start( args, fmt );
		
		int total = _vscprintf( fmt, args );
		if ( total < 0 )
			return false;
		
		char *bufferPtr = &( work[0] );
		
		if ( total >= WORK_SIZE )
			bufferPtr = (char*)( malloc( total + 1 ) );
			
		assert( bufferPtr );
		
		#if defined _CRUNCH_WINDOWS
		vsnprintf_s( bufferPtr, total + 1, _TRUNCATE, fmt, args );
		#else
		vsnprintf( bufferPtr, total + 1, fmt, args );
		#endif
		
		bufferPtr[total] = 0;
		
		core_fprintf( mFile, "%s", bufferPtr );

    if ( isFlushOnWrite() )
		  flush();
		
		if ( bufferPtr != &( work[0] ) )
			free( bufferPtr );
		
		return true;
	}

	return false;	
}

void TextFile::flush()
{
	if ( mFile )
		fflush( mFile );
}

bool TextFile::writeAllText( const WideString& filename, const String& text )
{
  TextFile dest;

  if ( !( dest.open( filename, MODE_WRITE ) ) )
    return false;

  dest.write( text );
  dest.flush();
  dest.close();

  return true;
}

bool TextFile::readAllText( const WideString& filename, String *text )
{
  if ( text == nullptr )
    return false;

  *text = "";

  TextFile source;

  if ( !( source.open( filename, MODE_READ ) ) )
    return false;

  if ( source.mBuffer != nullptr )
  {
    std::vector< char > localBuffer;
    localBuffer.resize( source.mBufferSize + 2 );
    memcpy( &( localBuffer[0] ), source.mBuffer, source.mBufferSize + 1 );
    localBuffer[localBuffer.size() - 1] = 0;
    *text = &( localBuffer[0] );
  }
  else
  {
    String line;

    while ( !( source.atEof() ) )
    {
      if ( source.readLine( &line ) )
        *text += String::fmt( "%s\n", line.c_str() );
    }
  }

  return true;
}

} }
