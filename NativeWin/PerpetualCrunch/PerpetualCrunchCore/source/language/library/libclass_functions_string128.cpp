
#include "../../crunch_global.h"

#include "../crunch_script_library_class.h"
#include "../crunch_script_library_tools.h"
#include "../crunch_script_compiler.h"

#include "libclass_functions_string128.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

///////////////////////////////////////////////////////////////////////////////////////////////////

ScriptLibraryClass_string128::ScriptLibraryClass_string128() :
  ScriptLibraryClass( "string128", sizeof( crunch::core::FixedString128 ) )
{
  #if defined CRUNCH_LIBRARY_ENABLE_DOCUMENTATION
  mDocumentation = "A fixed string with a capacity of 128 characters.  The max length of this fixed string is 127 characters.  "
                   "Fixed strings always take up the same amount of space regardless of how long the string is."
                   ;
  #endif
}

bool ScriptLibraryClass_string128::setup()
{
  static_assert( sizeof( crunch::core::FixedString128 ) == ( sizeof( crunch_i64 ) * 16 ) && "unexpected string128 size" );

  #define _class_name string128
  #include "../crunch_libclass_setup_start.mac"
  #include "libclass_functions_string128.table"
  #include "../crunch_libclass_setup_end.mac"
  #undef _class_name

  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

crunch_declare_libfunction_doc( string128, length, "The total number of characters in this fixed string." )
crunch_declare_libfunction_p0( string128, length, i64 )
{
  crunch_libfunction_entry_p1( crunch::core::FixedString128, a );
  crunch_libfunction_return( crunch_i64, crunch_i64( params->a.length() ) );
}

crunch_declare_libfunction_doc( string128, to_f64, "Convert this fixed string into a f64." )
crunch_declare_libfunction_p0( string128, to_f64, f64 )
{
  crunch_libfunction_entry_p1( crunch::core::FixedString128, a );
  crunch_libfunction_return( crunch_f64, crunch_f64( crunch::core::String::to_f64( params->a.c_str() ) ) );
}

crunch_declare_libfunction_doc( string128, to_i64, "Convert this fixed string into an i64." )
crunch_declare_libfunction_p0( string128, to_i64, i64 )
{
  crunch_libfunction_entry_p1( crunch::core::FixedString128, a );
  crunch_libfunction_return( crunch_i64, crunch_i64( crunch::core::String::to_i64( params->a.c_str() ) ) );
}

crunch_declare_libfunction_doc( string128, to_u64, "Convert this fixed string into an u64." )
crunch_declare_libfunction_p0( string128, to_u64, u64 )
{
  crunch_libfunction_entry_p1( crunch::core::FixedString128, a );
  crunch_libfunction_return( crunch_u64, crunch_u64( crunch::core::String::to_u64( params->a.c_str() ) ) );
}

} }
