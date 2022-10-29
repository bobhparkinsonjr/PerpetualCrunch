
#include "../../crunch_global.h"

#include "../../math/math_tools.h"

#include "../crunch_script_library_class.h"
#include "../crunch_script_library_tools.h"
#include "../crunch_script_compiler.h"

#include "libclass_functions_global.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

crunch_declare_libfunction_doc3( GlobalLibraryClass, clamp, f64, f64, f64, "Clamp the given value into [min, max]." )
crunch_declare_libfunction_p3( GlobalLibraryClass, clamp, f64, value, f64, minValue, f64, maxValue, f64 )
{
  crunch_libfunction_entry_p3( crunch_f64, value, crunch_f64, minValue, crunch_f64, maxValue );
  crunch_libfunction_return( crunch_f64, crunch_f64( crunch::math::MathTools::clamp_f64( params->value, params->minValue, params->maxValue ) ) );
}

crunch_declare_libfunction_doc1( GlobalLibraryClass, acos, f64, "The acos of the given value." )
crunch_declare_libfunction_p1( GlobalLibraryClass, acos, f64, value, f64 )
{
  crunch_libfunction_entry_p1( crunch_f64, value );
  crunch_libfunction_return( crunch_f64, crunch_f64( acos( params->value ) ) );
}

crunch_declare_libfunction_doc1( GlobalLibraryClass, cos, f64, "The cos of the given value." )
crunch_declare_libfunction_p1( GlobalLibraryClass, cos, f64, value, f64 )
{
  crunch_libfunction_entry_p1( crunch_f64, value );
  crunch_libfunction_return( crunch_f64, crunch_f64( cos( params->value ) ) );
}

crunch_declare_libfunction_doc1( GlobalLibraryClass, to_rad, f64, "Convert the given value in degrees to radians." )
crunch_declare_libfunction_p1( GlobalLibraryClass, to_rad, f64, value, f64 )
{
  crunch_libfunction_entry_p1( crunch_f64, value );
  crunch_libfunction_return( crunch_f64, crunch_f64( MathTools_TO_RAD_f64( params->value ) ) );
}

crunch_declare_libfunction_doc1( GlobalLibraryClass, to_deg, f64, "Convert the given value in radians to degrees." )
crunch_declare_libfunction_p1( GlobalLibraryClass, to_deg, f64, value, f64 )
{
  crunch_libfunction_entry_p1( crunch_f64, value );
  crunch_libfunction_return( crunch_f64, crunch_f64( MathTools_TO_DEG_f64( params->value ) ) );
}

} }
