
#include "../../crunch_global.h"

#include "../../math/math_tools.h"

#include "../crunch_script_library_class.h"
#include "../crunch_script_library_tools.h"
#include "../crunch_script_compiler.h"

#include "libclass_functions_global.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

crunch_declare_libfunction_doc1( GlobalLibraryClass, string128, string, "Construct a fixed string from a string." )
crunch_declare_libfunction_p1( GlobalLibraryClass, string128, string, txt, string128 )
{
  crunch_libfunction_entry_p1( crunch_string*, txt );
  crunch_libfunction_return( crunch::core::FixedString128, crunch::core::FixedString128( params->txt->c_str() ) );
}

crunch_declare_libfunction_doc2( GlobalLibraryClass, string128_operator_get_index, string128, i64, "Retrieve a character from a fixed string at the given index." )
crunch_declare_libfunction_p2( GlobalLibraryClass, string128_operator_get_index, string128, a, i64, i, char )
{
  crunch_libfunction_entry_p2( crunch::core::FixedString128, a, crunch_i64, i );
  crunch_libfunction_return( crunch_char, crunch_char( params->a[int( params->i )] ) );
}

} }
