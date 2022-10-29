
#include "../../crunch_global.h"

#include "../crunch_script_library_class.h"
#include "../crunch_script_library_tools.h"
#include "../crunch_script_compiler.h"

#include "libclass_functions_f64v3.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

///////////////////////////////////////////////////////////////////////////////////////////////////

ScriptLibraryClass_f64v3::ScriptLibraryClass_f64v3() :
  ScriptLibraryClass( "f64v3", sizeof( crunch::math::f64v3 ) )
{
  #if defined CRUNCH_LIBRARY_ENABLE_DOCUMENTATION
  mDocumentation = "A three component vector where each component is a f64.  The members of the vector can"
                   "be accessed with the dot operator (eg. a.x) or the subscript operator (eg. a[0]).  The "
                   "members are x, y, and z."
                   ;
  #endif
}

bool ScriptLibraryClass_f64v3::setup()
{
  static_assert( sizeof( crunch::math::f64v3 ) == ( sizeof( crunch_f64 ) * 3 ) && "unexpected f64v3 size" );

  setMemberInfo( "x", ScriptTypes::ScriptType::TYPE_F64, "", 0,                        sizeof( crunch_f64 ) );
  setMemberInfo( "y", ScriptTypes::ScriptType::TYPE_F64, "", sizeof( crunch_f64 ),     sizeof( crunch_f64 ) );
  setMemberInfo( "z", ScriptTypes::ScriptType::TYPE_F64, "", sizeof( crunch_f64 ) * 2, sizeof( crunch_f64 ) );

  #define _class_name f64v3
  #include "../crunch_libclass_setup_start.mac"
  #include "libclass_functions_f64v3.table"
  #include "../crunch_libclass_setup_end.mac"
  #undef _class_name

  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

crunch_declare_libfunction_doc1( f64v3, dot, f64v3, "The dot product of this vector with the given vector." )
crunch_declare_libfunction_p1( f64v3, dot, f64v3, b, f64 )
{
  crunch_libfunction_entry_p2( crunch::math::f64v3, a, crunch::math::f64v3, b );
  crunch_libfunction_return( crunch_f64, (crunch_f64)( crunch::math::DotProd( params->a, params->b ) ) );
}

crunch_declare_libfunction_doc1( f64v3, cross, f64v3, "The cross product of this vector with the given vector." )
crunch_declare_libfunction_p1( f64v3, cross, f64v3, b, f64v3 )
{
  crunch_libfunction_entry_p2( crunch::math::f64v3, a, crunch::math::f64v3, b );
  crunch_libfunction_return( crunch::math::f64v3, crunch::math::CrossProd( params->a, params->b ) );
}

crunch_declare_libfunction_doc( f64v3, normalize, "The normalized vector of this vector." )
crunch_declare_libfunction_p0( f64v3, normalize, f64v3 )
{
  crunch_libfunction_entry_p1( crunch::math::f64v3, a );
  crunch_libfunction_return( crunch::math::f64v3, params->a.Normalize() );
}

crunch_declare_libfunction_doc( f64v3, length, "The length of this vector." )
crunch_declare_libfunction_p0( f64v3, length, f64 )
{
  crunch_libfunction_entry_p1( crunch::math::f64v3, a );
  crunch_libfunction_return( crunch_f64, params->a.Length() );
}

crunch_declare_libfunction_doc( f64v3, length2, "The length squared of this vector." )
crunch_declare_libfunction_p0( f64v3, length2, f64 )
{
  crunch_libfunction_entry_p1( crunch::math::f64v3, a );
  crunch_libfunction_return( crunch_f64, params->a.LengthSquared() );
}

crunch_declare_libfunction_doc1( f64v3, consolePrint, string, "Prints this vector preceded with the given tag." )
crunch_declare_libfunction_p1( f64v3, consolePrint, string, tag, void )
{
  crunch_libfunction_entry_p2( crunch::math::f64v3, a, crunch_string*, tag );
  printf( "%s: < %0.6f, %0.6f, %0.6f >", params->tag->c_str(), params->a.x, params->a.y, params->a.z );
}

} }
