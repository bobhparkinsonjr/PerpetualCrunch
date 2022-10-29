
#include "../../crunch_global.h"

#include "../crunch_script_library_class.h"
#include "../crunch_script_library_tools.h"
#include "../crunch_script_compiler.h"

#include "libclass_functions_f64v4.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

///////////////////////////////////////////////////////////////////////////////////////////////////

ScriptLibraryClass_f64v4::ScriptLibraryClass_f64v4() :
  ScriptLibraryClass( "f64v4", sizeof( crunch::math::f64v4 ) )
{
  #if defined CRUNCH_LIBRARY_ENABLE_DOCUMENTATION
  mDocumentation = "A four component vector where each component is a f64.  The members of the vector can "
                   "be accessed with the dot operator (eg. a.x) or the subscript operator (eg. a[0]).  The "
                   "members are x, y, z, and w."
                   ;
  #endif
}

bool ScriptLibraryClass_f64v4::setup()
{
  static_assert( sizeof( crunch::math::f64v4 ) == ( sizeof( crunch_f64 ) * 4 ) && "unexpected f64v4 size" );

  setMemberInfo( "x", ScriptTypes::ScriptType::TYPE_F64, "", 0,                        sizeof( crunch_f64 ) );
  setMemberInfo( "y", ScriptTypes::ScriptType::TYPE_F64, "", sizeof( crunch_f64 ),     sizeof( crunch_f64 ) );
  setMemberInfo( "z", ScriptTypes::ScriptType::TYPE_F64, "", sizeof( crunch_f64 ) * 2, sizeof( crunch_f64 ) );
  setMemberInfo( "w", ScriptTypes::ScriptType::TYPE_F64, "", sizeof( crunch_f64 ) * 3, sizeof( crunch_f64 ) );

  #define _class_name f64v4
  #include "../crunch_libclass_setup_start.mac"
  #include "libclass_functions_f64v4.table"
  #include "../crunch_libclass_setup_end.mac"
  #undef _class_name

  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

crunch_declare_libfunction_doc1( f64v4, dot, f64v4, "The dot product of this vector with the given vector." )
crunch_declare_libfunction_p1( f64v4, dot, f64v4, b, f64 )
{
  crunch_libfunction_entry_p2( crunch::math::f64v4, a, crunch::math::f64v4, b );
  crunch_libfunction_return( crunch_f64, (crunch_f64)( crunch::math::DotProd( params->a, params->b ) ) );
}

crunch_declare_libfunction_doc( f64v4, normalize, "The normalized vector of this vector." )
crunch_declare_libfunction_p0( f64v4, normalize, f64v4 )
{
  crunch_libfunction_entry_p1( crunch::math::f64v4, a );
  crunch_libfunction_return( crunch::math::f64v4, params->a.Normalize() );
}

crunch_declare_libfunction_doc( f64v4, length, "The length of this vector." )
crunch_declare_libfunction_p0( f64v4, length, f64 )
{
  crunch_libfunction_entry_p1( crunch::math::f64v4, a );
  crunch_libfunction_return( crunch_f64, params->a.Length() );
}

crunch_declare_libfunction_doc( f64v4, length2, "The length squared of this vector." )
crunch_declare_libfunction_p0( f64v4, length2, f64 )
{
  crunch_libfunction_entry_p1( crunch::math::f64v4, a );
  crunch_libfunction_return( crunch_f64, params->a.LengthSquared() );
}

} }
