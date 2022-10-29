
#include "../crunch_global.h"

#include "crunch_script_library.h"

#include "crunch_script_types.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

crunch::core::String ScriptTypes::getScriptTypeName( ScriptType theType )
{
  switch ( theType )
  {
    case ScriptType::TYPE_STRING: return "_string";
    case ScriptType::TYPE_CHAR : return "_char";
    case ScriptType::TYPE_I64: return "_i64";
    case ScriptType::TYPE_U64: return "_u64";
    case ScriptType::TYPE_F64: return "_f64";
    case ScriptType::TYPE_VOID: return "_void";
    case ScriptType::TYPE_LIBRARY_CLASS: return "_library_class";
    case ScriptType::TYPE_SCRIPT_OBJECT: return "_script_object";

    default:
      // empty
    break;
  }

  return "_none";
}

crunch::core::String ScriptTypes::getScriptTypeKeyword( ScriptType theType )
{
  switch ( theType )
  {
    case ScriptType::TYPE_STRING: return "string";
    case ScriptType::TYPE_CHAR: return "char";
    case ScriptType::TYPE_I64: return "i64";
    case ScriptType::TYPE_U64: return "u64";
    case ScriptType::TYPE_F64: return "f64";
    case ScriptType::TYPE_VOID: return "void";
    case ScriptType::TYPE_LIBRARY_CLASS: return "";
    case ScriptType::TYPE_SCRIPT_OBJECT: return "";

    default:
      // empty
    break;
  }

  return "";
}

ScriptTypes::ScriptType ScriptTypes::getScriptType( const crunch::core::String& name )
{
  crunch::core::String findMe = name;
  findMe.toLower();

  if ( findMe == "_string" ) return ScriptType::TYPE_STRING;
  if ( findMe == "_char" ) return ScriptType::TYPE_CHAR;
  if ( findMe == "_i64" ) return ScriptType::TYPE_I64;
  if ( findMe == "_u64" ) return ScriptType::TYPE_U64;
  if ( findMe == "_f64" ) return ScriptType::TYPE_F64;
  if ( findMe == "_void" ) return ScriptType::TYPE_VOID;
  if ( findMe == "_library_class" ) return ScriptType::TYPE_LIBRARY_CLASS;
  if ( findMe == "_script_object" ) return ScriptType::TYPE_SCRIPT_OBJECT;

  return ScriptType::TYPE_NONE;
}

bool ScriptTypes::getScriptTypeDescriptions( const std::vector< crunch::core::String >& typeNames, std::vector< ScriptTypeDescription > *descriptions )
{
  if ( descriptions == nullptr )
    return false;

  descriptions->clear();

  unsigned int total = (unsigned int)( typeNames.size() );

  for ( unsigned int i = 0; i < total; ++i )
    descriptions->push_back( ScriptTypeDescription( getScriptType( typeNames[i] ) ) );

  return true;
}

bool ScriptTypes::getLibraryScriptTypeDescriptions( const std::vector< crunch::core::String >& typeNames, std::vector< ScriptTypeDescription > *descriptions )
{
  if ( descriptions == nullptr )
    return false;

  descriptions->clear();

  unsigned int total = (unsigned int)( typeNames.size() );

  for ( unsigned int i = 0; i < total; ++i )
    descriptions->push_back( getLibraryScriptTypeDescription( typeNames[i] ) );

  return true;
}

ScriptTypes::ScriptTypeDescription ScriptTypes::getLibraryScriptTypeDescription( const crunch::core::String& name )
{
  if ( name == "string" ) return ScriptTypeDescription( ScriptType::TYPE_STRING );
  if ( name == "char" ) return ScriptTypeDescription( ScriptType::TYPE_CHAR );
  if ( name == "i64" || name == "bool" ) return ScriptTypeDescription( ScriptType::TYPE_I64 );
  if ( name == "u64" ) return ScriptTypeDescription( ScriptType::TYPE_U64 );
  if ( name == "f64" ) return ScriptTypeDescription( ScriptType::TYPE_F64 );
  if ( name == "void" ) return ScriptTypeDescription( ScriptType::TYPE_VOID );

  return ScriptTypeDescription( ScriptType::TYPE_LIBRARY_CLASS, name );
}

unsigned int ScriptTypes::getDataStackSize( ScriptTypeDescription v )
{
  switch ( v.mScriptType )
  {
    case ScriptType::TYPE_STRING: return sizeof( crunch_string* );
    case ScriptType::TYPE_CHAR: return sizeof( crunch_char );
    case ScriptType::TYPE_I64: return sizeof( crunch_i64 );
    case ScriptType::TYPE_U64: return sizeof( crunch_u64 );
    case ScriptType::TYPE_F64: return sizeof( crunch_f64 );
    case ScriptType::TYPE_VOID: return 0;

    case ScriptType::TYPE_LIBRARY_CLASS:
    {
      if ( v.mIdentifier == "" )
        return 0;
      
      const ScriptLibraryClass *slc = ScriptLibrary::findLibraryClass( v.mIdentifier.c_str() );
      if ( slc == nullptr )
        return 0;

      return slc->getDataStackSize();
    }

    case ScriptType::TYPE_SCRIPT_OBJECT:
      static_assert( sizeof( INT_PTR ) == sizeof( crunch_i64 ), "possible padding issues with data stack at run time" );
      return sizeof( INT_PTR );

    default:
      // empty
    break;
  }

  return 0;
}

bool ScriptTypes::getMemberInfo( ScriptType v, const crunch::core::String& memberName, unsigned int *offset, unsigned int *size )
{
  if ( offset == nullptr || size == nullptr )
    return false;

  return false;
}

bool ScriptTypes::hasMembers( ScriptType v )
{
  return false;
}

} }
