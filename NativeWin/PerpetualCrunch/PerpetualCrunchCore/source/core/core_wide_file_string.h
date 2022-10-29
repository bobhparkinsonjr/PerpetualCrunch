#if !defined ( CORE_WIDE_FILE_STRING_H )
#define CORE_WIDE_FILE_STRING_H

namespace crunch { namespace core {

class WideFileString
{
	public:
		WideFileString( const WideString& path = (const wchar_t*)( L"" ) ) { split( path ); }
		~WideFileString() {}

	public:
    core_forceinline void clear() { mDir = L""; mName = L""; mExt = L""; }

		core_forceinline WideString getDir() const { return mDir; }
		core_forceinline WideString getName() const { return mName; }
		core_forceinline WideString getExt() const { return mExt; }

		core_forceinline WideString getPath() const { return ( mDir + mName + mExt ); }

		core_forceinline void setDir( const WideString& dir )
    {
      mDir = dir;

      if ( mDir.size() > 2 && mDir[mDir.size() - 1] != _CORE_PATH_SEP_WCH )
      {
        if ( mDir[mDir.size() - 1] == L'\\' || mDir[mDir.size() - 1] == L'/' )
          mDir[mDir.size() - 1] = _CORE_PATH_SEP_WCH;
        else
          mDir += _CORE_PATH_SEP_WSTR;
      }
    }

		core_forceinline void setName( const WideString& name ) { mName = name; }
		core_forceinline void setExt( const WideString& ext ) { mExt = ext; }

	private:
		void split( const WideString& filename )
		{
			int slashIndex = int( filename.rfind( L'\\' ) );
			if ( slashIndex < 0 )
				slashIndex = int( filename.rfind( L'/' ) );

			int dotIndex = int( filename.rfind( L'.' ) );

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
		WideString mDir;
		WideString mName;
		WideString mExt;

};

} }

#endif
