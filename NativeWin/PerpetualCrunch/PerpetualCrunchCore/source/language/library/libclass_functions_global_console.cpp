
#include "../../crunch_global.h"

#include <iostream>

#include "../../math/math_tools.h"

#include "../crunch_script_library_class.h"
#include "../crunch_script_library_tools.h"
#include "../crunch_script_compiler.h"

#include "libclass_functions_global.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

static void setupConsoleMessage( crunch::core::String *msg )
{
  msg->findAndReplace( "\\n", "\n" );
  msg->findAndReplace( "\\r", "\r" );
  msg->findAndReplace( "\\t", "\t" );
}

crunch_declare_libfunction_doc1( GlobalLibraryClass, consolePrint, string, "Print a message to standard output." )
crunch_declare_libfunction_p1( GlobalLibraryClass, consolePrint, string, msg, void )
{
  crunch_libfunction_entry_p1( crunch_string*, msg );

  crunch::core::String msg = *( params->msg );
  setupConsoleMessage( &msg );

  printf( msg.c_str() );
}

crunch_declare_libfunction_doc1( GlobalLibraryClass, consolePrint, string128, "Print the value of a fixed string to standard output." )
crunch_declare_libfunction_p1( GlobalLibraryClass, consolePrint, string128, msg, void )
{
  crunch_libfunction_entry_p1( crunch::core::FixedString128, msg );

  crunch::core::String msg = params->msg.c_str();
  setupConsoleMessage( &msg );

  printf( msg.c_str() );
}

crunch_declare_libfunction_doc1( GlobalLibraryClass, consolePrint, i64, "Print the value of an i64 to standard output." )
crunch_declare_libfunction_p1( GlobalLibraryClass, consolePrint, i64, v, void )
{
  crunch_libfunction_entry_p1( crunch_i64, v );
  printf( "%d", int( params->v ) );
}

crunch_declare_libfunction_doc1( GlobalLibraryClass, consolePrint, f64, "Print the value of a f64 to standard output." )
crunch_declare_libfunction_p1( GlobalLibraryClass, consolePrint, f64, v, void )
{
  crunch_libfunction_entry_p1( crunch_f64, v );
  printf( "%0.8f", float( params->v ) );
}

crunch_declare_libfunction_doc1( GlobalLibraryClass, consolePrint, char, "Print the value of a char to standard output." )
crunch_declare_libfunction_p1( GlobalLibraryClass, consolePrint, char, c, void )
{
  crunch_libfunction_entry_p1( crunch_char, c );
  printf( "%c", char( params->c ) );
}

crunch_declare_libfunction_doc( GlobalLibraryClass, consoleRead128, "Read a string128 from standard input." )
crunch_declare_libfunction_p0( GlobalLibraryClass, consoleRead128, string128 )
{
  std::string v;
  std::getline( std::cin, v );

  crunch_libfunction_return( crunch::core::FixedString128, crunch::core::FixedString128( v.c_str() ) );
}

} }
