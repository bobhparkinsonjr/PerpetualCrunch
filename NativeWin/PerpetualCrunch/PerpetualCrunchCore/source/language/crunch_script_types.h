#if !defined ( CRUNCH_SCRIPT_TYPES_H )
#define CRUNCH_SCRIPT_TYPES_H

namespace crunch { namespace language {

typedef crunch::core::String crunch_string;
typedef int64_t crunch_char;
typedef int64_t crunch_i64;
typedef uint64_t crunch_u64;
typedef double crunch_f64;

class ScriptTypes
{
  public:
    // NOTE: do not remove any items here, or re-order, values are recorded into binary files,
    //       it is safe to add new types before TYPE_TOTAL
    enum class ScriptType
    {
      TYPE_NONE = -1,

      TYPE_STRING,
      TYPE_CHAR,
      TYPE_I64,
      TYPE_U64,
      TYPE_F64,
      TYPE_VOID,
      TYPE_LIBRARY_CLASS,
      TYPE_SCRIPT_OBJECT,

      TYPE_TOTAL
    };

    enum { TYPE_MAX_SIZE_BYTES = 128 };

    struct ScriptTypeDescription
    {
      ScriptType mScriptType = ScriptType::TYPE_NONE;
      crunch::core::String mIdentifier;

      ScriptTypeDescription( ScriptType theType = ScriptType::TYPE_NONE, const crunch::core::String& identifier = "" ) :
        mScriptType( theType ),
        mIdentifier( identifier )
      {
        // empty
      }

      core_forceinline bool operator == ( const ScriptTypeDescription& other ) const
      {
        return ( mScriptType == other.mScriptType && mIdentifier == other.mIdentifier );
      }

      core_forceinline bool operator != ( const ScriptTypeDescription& other ) const
      {
        return ( !( *this == other ) );
      }

      // do not modify, this is used to help form function signatures
      crunch::core::String getTypeDescription() const
      {
        if ( mIdentifier == "" )
          return ScriptTypes::getScriptTypeName( mScriptType );
        return crunch::core::String::fmt( "%s %s", ScriptTypes::getScriptTypeName( mScriptType ).c_str(), mIdentifier.c_str() );
      }

      crunch::core::String getDisplayTypeDescription() const
      {
        if ( mIdentifier == "" )
        {
          crunch::core::String k = ScriptTypes::getScriptTypeKeyword( mScriptType );
          if ( k != "" )
            return k;

          return ScriptTypes::getScriptTypeName( mScriptType );
        }

        if ( mScriptType == ScriptType::TYPE_NONE || mScriptType == ScriptType::TYPE_LIBRARY_CLASS || mScriptType == ScriptType::TYPE_SCRIPT_OBJECT )
          return mIdentifier;

        return crunch::core::String::fmt( "%s %s", ScriptTypes::getScriptTypeName( mScriptType ).c_str(), mIdentifier.c_str() );
      }

      core_forceinline bool empty() const { return ( mScriptType == ScriptType::TYPE_NONE && mIdentifier == "" ); }
    };
  
  public:
    static crunch::core::String getScriptTypeName( ScriptType theType );
    static crunch::core::String getScriptTypeKeyword( ScriptType theType );

    static ScriptType getScriptType( const crunch::core::String& name );

    // the mIdentifier property on all the returned ScriptTypeDescriptions is always empty
    static bool getScriptTypeDescriptions( const std::vector< crunch::core::String >& typeNames, std::vector< ScriptTypeDescription > *descriptions );

    // typeNames should contain things like: "int", "TriangleGraph, &c.  Anything not built-in, is assumed to be a library class
    static bool getLibraryScriptTypeDescriptions( const std::vector< crunch::core::String >& typeNames, std::vector< ScriptTypeDescription > *descriptions );

    // this will never return any kind of object type, it is either a library class or a built-in type (eg. int)
    // name is case-sensitive
    // eg. "int" will return ScriptTypeDescription with ScriptType TYPE_I64
    static ScriptTypeDescription getLibraryScriptTypeDescription( const crunch::core::String& name );

    // in bytes
    static unsigned int getDataStackSize( ScriptTypeDescription v );

    // returns false if the given script type doesn't have a member memberName
    // on success, returns true, and offset will contain the offset in bytes to the member from the start of the type (eg. 0 for the first member),
    //   size will contain the size in bytes of the member
    static bool getMemberInfo( ScriptType v, const crunch::core::String& memberName, unsigned int *offset, unsigned int *size );

    // returns true if the script type has member variables
    static bool hasMembers( ScriptType v );

};

} }

#endif
