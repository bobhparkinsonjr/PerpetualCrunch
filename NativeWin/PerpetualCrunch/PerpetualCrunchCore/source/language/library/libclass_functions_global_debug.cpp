
#include "../../crunch_global.h"

#include "../../math/math_tools.h"

#include "../crunch_script_library_class.h"
#include "../crunch_script_library_tools.h"
#include "../crunch_script_compiler.h"

#include "libclass_functions_global.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

///////////////////////////////////////////////////////////////////////////////////////////////////

crunch_declare_libfunction_doc1( GlobalLibraryClass, debugPrint, string, "Print a message to debug output." )
crunch_declare_libfunction_p1( GlobalLibraryClass, debugPrint, string, msg, void )
{
  crunch_libfunction_entry_p1( crunch_string*, msg );
  corePrint( "%s", params->msg->c_str() );
}

crunch_declare_libfunction_doc1( GlobalLibraryClass, debugPrint, i64, "Print an i64 to debug output." )
crunch_declare_libfunction_p1( GlobalLibraryClass, debugPrint, i64, v, void )
{
  crunch_libfunction_entry_p1( crunch_i64, v );
  corePrint( "%d", params->v );
}

crunch_declare_libfunction_doc1( GlobalLibraryClass, debugPrint, f64, "Print a f64 to debug output." )
crunch_declare_libfunction_p1( GlobalLibraryClass, debugPrint, f64, v, void )
{
  crunch_libfunction_entry_p1( crunch_f64, v );
  corePrint( "%0.8f", params->v );
}

crunch_declare_libfunction_doc2( GlobalLibraryClass, debugPrint, string, f64, "Print a f64 with the given tag to debug output." )
crunch_declare_libfunction_p2( GlobalLibraryClass, debugPrint, string, tag, f64, v, void )
{
  crunch_libfunction_entry_p2( crunch_string*, tag, crunch_f64, v );
  corePrint( "%s: %0.8f", params->tag->c_str(), params->v );
}

crunch_declare_libfunction_doc1( GlobalLibraryClass, debugBreak, string, "Establish a location for a breakpoint with the given message." )
crunch_declare_libfunction_p1( GlobalLibraryClass, debugBreak, string, msg, void )
{
  crunch_libfunction_entry_p1( crunch_string*, msg );
  corePrint( "[[ CRUNCH BREAKPOINT ]] %s", params->msg->c_str() );

  #if defined _CRUNCH_DEBUG

  while ( true )
  {
    int i = 0; // place breakpoint here in debugger
  }

  #endif
}

} }
