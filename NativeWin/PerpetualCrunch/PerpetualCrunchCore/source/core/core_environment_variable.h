#if !defined ( CORE_ENVIRONMENT_VARIABLE_H )
#define CORE_ENVIRONMENT_VARIABLE_H

namespace crunch { namespace core {

class EnvironmentVariable
{
	public:
		EnvironmentVariable( const char *name = nullptr )
		{
			if ( name )
			{
			  #if defined _CRUNCH_WINDOWS
			  
				char* libvar;
				size_t requiredSize;
				
				getenv_s( &requiredSize, nullptr, 0, name );
				
				if ( requiredSize > 0 )
				{
					libvar = (char*)( malloc( requiredSize * sizeof( char ) ) );
					
					if ( libvar )
					{
						getenv_s( &requiredSize, libvar, requiredSize, name );
						
						mValue = libvar;
						
						free( libvar );
					}
				}
				
				#else
				
				const char *rawValue = getenv( name );
				if ( rawValue != nullptr )
				  mValue = (const char*)( rawValue );
				
				#endif
			}
		}
		
	public:
		core_forceinline const std::string& getValue() const { return mValue; }
		core_forceinline const char* c_str() const { return mValue.c_str(); }
		
	private:
		std::string mValue;
		
};

} }

#endif
