
#include "../../crunch_global.h"

#include "../../math/math_tools.h"

#include "../crunch_script_library_class.h"
#include "../crunch_script_library_tools.h"
#include "../crunch_script_compiler.h"

#include "libclass_functions_global.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

crunch_declare_libfunction_doc3( GlobalLibraryClass, f64v3, f64, f64, f64, "Three tuple f64 vector constructor, takes parameters x, y, and z." )
crunch_declare_libfunction_p3( GlobalLibraryClass, f64v3, f64, x, f64, y, f64, z, f64v3 )
{
  corePrint( "[WARNING] the compiler shouldn't have generated code to call this constructor (f64v3)" );
  crunch_libfunction_entry_p3( crunch_f64, x, crunch_f64, y, crunch_f64, z );
  crunch_libfunction_return( crunch::math::f64v3, crunch::math::f64v3( params->x, params->y, params->z ) );
}

crunch_declare_libfunction_doc2( GlobalLibraryClass, f64v3_operator_add, f64v3, f64v3, "Add two vectors." )
crunch_declare_libfunction_p2( GlobalLibraryClass, f64v3_operator_add, f64v3, a, f64v3, b, f64v3 )
{
  crunch_libfunction_entry_p2( crunch::math::f64v3, a, crunch::math::f64v3, b );
  crunch_libfunction_return( crunch::math::f64v3, ( params->a + params->b ) );
}

crunch_declare_libfunction_doc2( GlobalLibraryClass, f64v3_operator_subtract, f64v3, f64v3, "Subtract two vectors." )
crunch_declare_libfunction_p2( GlobalLibraryClass, f64v3_operator_subtract, f64v3, a, f64v3, b, f64v3 )
{
  crunch_libfunction_entry_p2( crunch::math::f64v3, a, crunch::math::f64v3, b );
  crunch_libfunction_return( crunch::math::f64v3, ( params->a - params->b ) );
}

crunch_declare_libfunction_doc2( GlobalLibraryClass, f64v3_operator_multiply, f64v3, f64, "Multiply a vector by a scalar." )
crunch_declare_libfunction_p2( GlobalLibraryClass, f64v3_operator_multiply, f64v3, a, f64, b, f64v3 )
{
  crunch_libfunction_entry_p2( crunch::math::f64v3, a, crunch_f64, b );
  crunch_libfunction_return( crunch::math::f64v3, ( params->a * params->b ) );
}

crunch_declare_libfunction_doc2( GlobalLibraryClass, f64v3_operator_multiply, f64, f64v3, "Multiply a vector by a scalar." )
crunch_declare_libfunction_p2( GlobalLibraryClass, f64v3_operator_multiply, f64, a, f64v3, b, f64v3 )
{
  crunch_libfunction_entry_p2( crunch_f64, a, crunch::math::f64v3, b );
  crunch_libfunction_return( crunch::math::f64v3, ( params->a * params->b ) );
}

crunch_declare_libfunction_doc1( GlobalLibraryClass, f64v3_operator_minus, f64v3, "Negate a vector." )
crunch_declare_libfunction_p1( GlobalLibraryClass, f64v3_operator_minus, f64v3, a, f64v3 )
{
  crunch_libfunction_entry_p1( crunch::math::f64v3, a );
  crunch_libfunction_return( crunch::math::f64v3, ( -( params->a ) ) );
}

crunch_declare_libfunction_doc2( GlobalLibraryClass, f64v3_operator_get_index, f64v3, i64, "Get the f64 at the given index.  The index must be 0, 1, or 2.  The indices correspond to the members x, y, and z." )
crunch_declare_libfunction_p2( GlobalLibraryClass, f64v3_operator_get_index, f64v3, a, i64, i, f64 )
{
  crunch_libfunction_entry_p2( crunch::math::f64v3, a, crunch_i64, i );
  crunch_libfunction_return( crunch_f64, ( params->a[int( params->i )] ) );
}

} }
