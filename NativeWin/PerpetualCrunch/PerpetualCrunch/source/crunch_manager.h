#if !defined ( CRUNCH_MANAGER_H )
#define CRUNCH_MANAGER_H

namespace crunch
{

class CrunchManager final
{
  private:
    CrunchManager();
    CrunchManager( const CrunchManager& other );
    CrunchManager& operator = ( const CrunchManager& other );
    ~CrunchManager();

  public:
    /*

      At least one .crunch or .crunchy file must be specified (not both).

      For a .crunch file, it will be compiled into a .crunchy file.

      For a .crunchy file, it will be ran.

      All flags must precede any given filename (.crunch or .crunchy).

      For additional usage info, use the -h flag (no filename needed), can just use:

        PerpetualCrunch -h

    */
    static int runCommandLine( int argc, char *argv[] );

  private:
    static void reportError( const char *format, ... );
    static void reportWarning( const char *format, ... );

  private:
    static void compileScript( const crunch::core::FileString& sourceFilePath );
    static void runScript( const crunch::core::FileString& sourceFilePath );

  private:
    static void printLibraryDocumentation();

    static void printUsage();

  private:
    static inline bool mPrintUsage = false;
    static inline bool mRunScript = false;
    static inline bool mDisableWarnings = false;

    static inline bool mPrintLibraryDocumentation = false;
    static inline crunch::core::String mDocumentationLibraryClass;
    static inline crunch::core::String mDocumentationLibraryFunctionName;
    static inline crunch::core::String mDocumentationLibraryFunctionSignature;

};

}

#endif
