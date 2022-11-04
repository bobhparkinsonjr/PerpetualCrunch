
#include "../../crunch_global.h"

#include "../../core/core_environment_variable.h"

#include "../../math/math_tools.h"

#include "../crunch_script_library_class.h"
#include "../crunch_script_library_tools.h"
#include "../crunch_script_compiler.h"

#include "libclass_functions_global.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

ScriptLibraryClass_GlobalLibraryClass::ScriptLibraryClass_GlobalLibraryClass() :
  ScriptLibraryClass( CRUNCH_LIB_GLOBAL_CLASS_NAME )
{
  #if defined CRUNCH_LIBRARY_ENABLE_DOCUMENTATION
  mDocumentation = "All global library functions belong to the global library class."
                   ;
  #endif
}

bool ScriptLibraryClass_GlobalLibraryClass::setup()
{
  #define _class_name GlobalLibraryClass
  #include "../crunch_libclass_setup_start.mac"
  #include "libclass_functions_global.table"
  #include "../crunch_libclass_setup_end.mac"
  #undef _class_name

  crunch_libfunction_nocallcode( "f64v3(_f64,_f64,_f64)" );
  crunch_libfunction_nocallcode( "f64v4(_f64,_f64,_f64,_f64)" );

  return true;
}

void ScriptLibraryClass_GlobalLibraryClass::createGlobalSymbols( ScriptCompiler *compiler )
{
  compiler->createGlobalSymbol( "PI", new SyntaxNodeLiteralType( compiler, nullptr, 0, crunch_f64( MathTools_PI_f64 ) ) );

  compiler->createGlobalSymbol( "__BUILD_DATE__", new SyntaxNodeLiteralType( compiler, nullptr, 0, (crunch_string*)( compiler->stringTableMergeString( __DATE__, false, nullptr ) ) ) );

  #if defined _CRUNCH_WINDOWS
  {
    crunch::core::EnvironmentVariable buildMachine( "COMPUTERNAME" );
    compiler->createGlobalSymbol( "__BUILD_MACHINE__", new SyntaxNodeLiteralType( compiler, nullptr, 0, (crunch_string*)( compiler->stringTableMergeString( buildMachine.c_str(), false, nullptr ) ) ) );
  }
  #else
  compiler->createGlobalSymbol( "__BUILD_MACHINE__", new SyntaxNodeLiteralType( compiler, nullptr, 0, (crunch_string*)( compiler->stringTableMergeString( "", false, nullptr ) ) ) );
  #endif

  compiler->createGlobalSymbol( "__VERSION_MAJOR__", new SyntaxNodeLiteralType( compiler, nullptr, 0, crunch_i64( ScriptCompiler::VERSION_MAJOR ) ) );
  compiler->createGlobalSymbol( "__VERSION_MINOR__", new SyntaxNodeLiteralType( compiler, nullptr, 0, crunch_i64( ScriptCompiler::VERSION_MINOR ) ) );
}

} }
