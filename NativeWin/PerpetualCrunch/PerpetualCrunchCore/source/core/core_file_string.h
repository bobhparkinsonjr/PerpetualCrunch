#if !defined ( CORE_FILE_STRING_H )
#define CORE_FILE_STRING_H

namespace crunch { namespace core {

class FileString
{
  public:
    FileString( const String& path = "" ) { split( path ); }
    ~FileString() {}

  public:
    core_forceinline String getDir() const { return mDir; }
    core_forceinline String getName() const { return mName; }
    core_forceinline String getExt() const { return mExt; }

    core_forceinline String getPath() const { return ( mDir + mName + mExt ); }

    core_forceinline void toLower() { mDir.toLower(); mName.toLower(); mExt.toLower(); }

    core_forceinline void setDir( const String& dir )
    {
      mDir = dir;

      if ( mDir.size() > 2 && mDir[mDir.size() - 1] != _CORE_PATH_SEP_CH )
      {
        if ( mDir[mDir.size() - 1] == '\\' || mDir[mDir.size() - 1] == '/' )
          mDir[mDir.size() - 1] = _CORE_PATH_SEP_CH;
        else
          mDir += _CORE_PATH_SEP_STR;
      }
    }

    core_forceinline void setName( const String& name ) { mName = name; }
    core_forceinline void setExt( const String& ext ) { mExt = ext; }

  private:
    void split( const String& filename )
    {
      int bslashIndex = int( filename.rfind( "\\" ) );
      int fslashIndex = int( filename.rfind( "/" ) );

      int slashIndex;
      if ( bslashIndex >= 0 && fslashIndex >= 0 )
      {
        if ( bslashIndex > fslashIndex )
          slashIndex = bslashIndex;
        else
          slashIndex = fslashIndex;
      }
      else
      if ( bslashIndex >= 0 )
        slashIndex = bslashIndex;
      else
        slashIndex = fslashIndex;

      int dotIndex = int( filename.rfind( "." ) );

      if ( slashIndex >= 0 && dotIndex > ( slashIndex + 1 ) )
      {
        mDir = filename.substr( 0, slashIndex + 1 );
        mName = filename.bsubstr( slashIndex + 1, dotIndex - 1 );
        mExt = filename.substr( dotIndex );

        return;
      }

      if ( slashIndex < 0 && dotIndex > 0 )
      {
        mName = filename.substr( 0, dotIndex );
        mExt = filename.substr( dotIndex );

        return;
      }

      if ( slashIndex >= 0 )
      {
        mDir = filename.substr( 0, slashIndex + 1 );
        mName = filename.substr( slashIndex + 1 );

        return;
      }

      mName = filename;
    }

	private:
		String mDir;
		String mName;
		String mExt;

};

} }

#endif
