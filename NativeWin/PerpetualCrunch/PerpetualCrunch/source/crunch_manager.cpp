
#include "global.h"

#include "../../PerpetualCrunchCore/source/language/crunch_script_compiler.h"
#include "../../PerpetualCrunchCore/source/language/crunch_script_executable.h"
#include "../../PerpetualCrunchCore/source/language/crunch_script_library.h"

#include "crunch_manager.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch
{

int CrunchManager::runCommandLine( int argc, char *argv[] )
{
  crunch::core::WideString workingDirectory;
  crunch::core::FileTools::getCurrentWorkingDirectory( &workingDirectory );

  // corePrint( "total args: %d | working directory: '%s'", argc, workingDirectory.toString().c_str() );

  crunch::language::ScriptCompiler::setup();

  bool processedFile = false;

  for ( int argIndex = 1; argIndex < argc; ++argIndex )
  {
    crunch::core::String arg = argv[argIndex];

    // corePrint( "arg %d: '%s'", argIndex, arg.c_str() );

    if ( arg == "" )
      continue;

    if ( arg[0] == '-' )
    {
      if ( arg.size() < 2 )
      {
        reportError( "invalid flag '%s'", arg.c_str() );
        return 1;
      }

      crunch::core::String flagName = arg.substr( 1 );
      flagName.toLower();

      if ( flagName == "help" || flagName == "h" )
        mPrintUsage = true;
      else
      if ( flagName == "run" || flagName == "r" )
        mRunScript = true;
      else
      if ( flagName == "nowarn" )
        mDisableWarnings = true;
      else
      if ( flagName == "libdoc" )
        mPrintLibraryDocumentation = true;
      else
      if ( int( flagName.find( "libdoc-class:" ) ) == 0 )
      {
        mPrintLibraryDocumentation = true;
        mDocumentationLibraryClass = flagName.substr( crunch::core::String( "libdoc-class:" ).size() );
      }
      else
      if ( int( flagName.find( "libdoc-function:" ) ) == 0 )
      {
        mPrintLibraryDocumentation = true;
        mDocumentationLibraryFunctionName = flagName.substr( crunch::core::String( "libdoc-function:" ).size() );
      }
      else
      if ( int( flagName.find( "libdoc-function-signature:" ) ) == 0 )
      {
        mPrintLibraryDocumentation = true;
        mDocumentationLibraryFunctionSignature = flagName.substr( crunch::core::String( "libdoc-function-signature:" ).size() );
      }
      else
      {
        reportWarning( "ignoring unknown flag '%s'", arg.c_str() );
        mPrintUsage = true;
      }
    }
    else
    {
      processedFile = true;

      if ( mPrintUsage )
        printUsage();

      if ( mPrintLibraryDocumentation )
        printLibraryDocumentation();

      crunch::core::FileString crunchFilePath( arg );

      crunch::core::String ext = crunchFilePath.getExt();
      ext.toLower();

      if ( ext == ".crunch" )
        compileScript( crunchFilePath );
      else
      if ( ext == ".crunchy" )
        runScript( crunchFilePath );
      else
      {
        reportError( "unknown file type '%s' for file '%s'", ext.c_str(), crunchFilePath.getPath().c_str() );
        return 1;
      }
    }
  }

  if ( !processedFile )
  {
    if ( mPrintUsage )
      printUsage();

    if ( mPrintLibraryDocumentation )
      printLibraryDocumentation();
  }

  return 0;
}

void CrunchManager::reportError( const char *format, ... )
{
  va_list args;
  va_start( args, format );
  crunch::core::String message = crunch::core::String::fmtv( format, args );
  va_end( args );

  printf( "error: %s\n", message.c_str() );
}

void CrunchManager::reportWarning( const char *format, ... )
{
  va_list args;
  va_start( args, format );
  crunch::core::String message = crunch::core::String::fmtv( format, args );
  va_end( args );

  printf( "warning: %s\n", message.c_str() );
}

void CrunchManager::compileScript( const crunch::core::FileString& sourceFilePath )
{
  unsigned int compilerFlags = crunch::language::ScriptCompiler::FlagType::FLAG_DEFAULT_SETTINGS;

  if ( mDisableWarnings )
    compilerFlags &= ~( crunch::language::ScriptCompiler::FlagType::FLAG_PRINT_WARNINGS );

  crunch::language::ScriptCompiler compiler( compilerFlags );

  bool buildResult = compiler.build( sourceFilePath.getPath(), nullptr, L"", sourceFilePath.getName() );

  if ( mRunScript && buildResult && compiler.getTotalErrors() == 0 )
  {
    crunch::core::String runSource = sourceFilePath.getDir() + sourceFilePath.getName() + ".crunchy";
    crunch::core::FileString runFilePath( runSource );
    runScript( runFilePath );
  }
}

void CrunchManager::runScript( const crunch::core::FileString& sourceFilePath )
{
  crunch::language::ScriptExecutable::run( sourceFilePath.getPath(), "main", {}, {} );
}

void CrunchManager::printLibraryDocumentation()
{
  if ( mDocumentationLibraryClass == "" )
    crunch::language::ScriptLibrary::consolePrintAllDocumentation();
  else
  {
    if ( mDocumentationLibraryFunctionName != "" )
      crunch::language::ScriptLibrary::consolePrintFunctionDocumentation( mDocumentationLibraryClass, mDocumentationLibraryFunctionName );
    else
    if ( mDocumentationLibraryFunctionSignature != "" )
      crunch::language::ScriptLibrary::consolePrintFunctionSignatureDocumentation( mDocumentationLibraryClass, mDocumentationLibraryFunctionSignature );
    else
      crunch::language::ScriptLibrary::consolePrintClassDocumentation( mDocumentationLibraryClass );
  }
}

void CrunchManager::printUsage()
{
  printf( "\nPerpetual Crunch\n" );
  printf( "version %d.%d\n", VERSION_MAJOR, VERSION_MINOR );
  printf( "build date: " __DATE__ "\n" );

  printf( "\nusage:\n" );

  printf( "\nthe command line takes the form:\n" );
  printf( "PerpetualCrunch <flags> <filename>\n" );

  printf( "\nall flags start with a - character, and precede the filename, the available flags are:\n" );
  printf( "  -help or -h         : print this usage info\n" );
  printf( "  -run or -r          : run the script in addition to compiling it\n" );
  printf( "  -nowarn             : disable warnings\n" );
  printf( "  -libdoc             : print all library documentation\n" );
  printf( "  -libdoc-class:<class name> : print library documentation for the given class\n" );
  printf( "  -libdoc-function:<function name> : print library documentation for the given function\n" );
  printf( "  -libdoc-function-signature:<function signature> : print library documentation for the given function signature\n" );

  printf( "\nexamples:\n" );

  printf( "\nto print documentation for all the functions in the library class string128 and then exit:\n" );
  printf( "PerpetualCrunch -libdoc-class:string128\n" );

  printf( "\nto print documentation for the length function in class f64v3 and then exit:\n" );
  printf( "PerpetualCrunch -libdoc-class:f64v3 -libdoc-function:length\n" );

  printf( "\nto compile the sample script:\n" );
  printf( "PerpetualCrunch ../../samples/sample.crunch\n" );

  printf( "\nto compile and run the sample script:\n" );
  printf( "PerpetualCrunch -r ../../samples/sample.crunch\n" );
}

}
