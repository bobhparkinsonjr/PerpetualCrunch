#if !defined ( CRUNCH_SCRIPT_LIBRARY_FUNCTION_H )
#define CRUNCH_SCRIPT_LIBRARY_FUNCTION_H

#include "crunch_script_types.h"

namespace crunch { namespace language {

class ScriptLibraryClass;
class ScriptExecutable;

class ScriptLibraryFunction
{
  public:
    typedef enum
    {
      FLAG_NO_CALL_CODE = 1 << 0
    } FlagType;

  public:
    ScriptLibraryFunction();
    virtual ~ScriptLibraryFunction();

  public:
    ScriptLibraryFunction* setup( const ScriptLibraryClass *owner,
                                  const crunch::core::String& name, 
                                  const std::vector< ScriptTypes::ScriptTypeDescription >& parameterTypes, 
                                  ScriptTypes::ScriptTypeDescription returnType
                                  );

  public:
    core_forceinline void setOwner( ScriptLibraryClass *owner ) { mOwner = owner; }
    core_forceinline const ScriptLibraryClass* getOwner() const { return mOwner; }

    core_forceinline crunch::core::String getSignature() const { return mSignature; }

    // only set the signature when using this instance for a search
    // typically, the signature is set internally as needed
    core_forceinline void setSignature( const crunch::core::String& signature ) { mSignature = signature; }

    crunch::core::String findName() const;

    core_forceinline void getParameterTypes( std::vector< ScriptTypes::ScriptTypeDescription > *dest ) const { *dest = mParameterTypes; }

    // this never includes the size of a this pointer
    core_forceinline unsigned int getParametersDataStackSize() const { return mParametersDataStackSize; }

    core_forceinline ScriptTypes::ScriptTypeDescription getReturnType() const { return mReturnType; }

    core_forceinline unsigned int getReturnTypeDataStackSize() const { return mReturnTypeDataStackSize; }

    #if defined CRUNCH_LIBRARY_ENABLE_DOCUMENTATION
    virtual crunch::core::String getDocumentation() const { return ""; }
    #endif

    #if defined CRUNCH_LIBRARY_ENABLE_DOCUMENTATION
    core_forceinline const crunch::core::String& getDisplaySignature() const { return mDisplaySignature; }
    #endif

  public:
    virtual void entry( const ScriptExecutable *executable, char *stackParams, char *stackResult ) {}

    // if true, then the compiler will generate no code to call this function,
    //   it will still generate code to push parameters to the stack
    // no effect on operator functions
    core_forceinline bool nocallcode() const { return isFlagged( FLAG_NO_CALL_CODE ); }

    core_forceinline void set_nocallcode( bool v ) { setFlag( FLAG_NO_CALL_CODE, v ); }

  public:
    core_forceinline bool operator < ( const ScriptLibraryFunction& rhs ) const
    {
      return ( mSignature < rhs.mSignature );
    }

  protected:
    core_forceinline bool isFlagged( FlagType f ) const { return ( ( mFlags & f ) != 0 ); }
    core_forceinline void setFlag( FlagType f, bool v ) { if ( v ) mFlags |= f; else mFlags &= ~f; }

  public:
    ScriptLibraryClass *mOwner = nullptr;

    crunch::core::String mSignature;
    std::vector< ScriptTypes::ScriptTypeDescription > mParameterTypes;
    ScriptTypes::ScriptTypeDescription mReturnType;

    #if defined CRUNCH_LIBRARY_ENABLE_DOCUMENTATION
    crunch::core::String mDisplaySignature;
    #endif

    unsigned int mParametersDataStackSize = UINT_MAX;
    unsigned int mReturnTypeDataStackSize = UINT_MAX;

    unsigned int mFlags = 0;

};

class ScriptLibraryFunctionCompare final
{
  public:
    core_forceinline bool operator()( const ScriptLibraryFunction *lhs, const ScriptLibraryFunction *rhs ) const
    {
      return ( *lhs < *rhs );
    }

};

} }

///////////////////////////////////////////////////////////////////////////////////////////////////

#if defined CRUNCH_LIBRARY_ENABLE_DOCUMENTATION
#define crunch_declare_libfunction_doc( _class, _function, _doc ) crunch::core::String _class##_##_function ::getDocumentation() const { return ( _doc ); }
#define crunch_declare_libfunction_doc1( _class, _function, _t0, _doc ) crunch::core::String _class##_##_function##_##_t0 ::getDocumentation() const { return ( _doc ); }
#define crunch_declare_libfunction_doc2( _class, _function, _t0, _t1, _doc ) crunch::core::String _class##_##_function##_##_t0##_##_t1 ::getDocumentation() const { return ( _doc ); }
#define crunch_declare_libfunction_doc3( _class, _function, _t0, _t1, _t2, _doc ) crunch::core::String _class##_##_function##_##_t0##_##_t1##_##_t2 ::getDocumentation() const { return ( _doc ); }
#define crunch_declare_libfunction_doc4( _class, _function, _t0, _t1, _t2, _t3, _doc ) crunch::core::String _class##_##_function##_##_t0##_##_t1##_##_t2##_##_t3 ::getDocumentation() const { return ( _doc ); }
#define crunch_declare_libfunction_doc5( _class, _function, _t0, _t1, _t2, _t3, _t4, _doc ) crunch::core::String _class##_##_function##_##_t0##_##_t1##_##_t2##_##_t3##_##_t4 ::getDocumentation() const { return ( _doc ); }
#define crunch_declare_libfunction_doc6( _class, _function, _t0, _t1, _t2, _t3, _t4, _t5, _doc ) crunch::core::String _class##_##_function##_##_t0##_##_t1##_##_t2##_##_t3##_##_t4##_##_t5 ::getDocumentation() const { return ( _doc ); }
#define crunch_declare_libfunction_doc7( _class, _function, _t0, _t1, _t2, _t3, _t4, _t5, _t6, _doc ) crunch::core::String _class##_##_function##_##_t0##_##_t1##_##_t2##_##_t3##_##_t4##_##_t5##_##_t6 ::getDocumentation() const { return ( _doc ); }
#define crunch_declare_libfunction_doc8( _class, _function, _t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _doc ) crunch::core::String _class##_##_function##_##_t0##_##_t1##_##_t2##_##_t3##_##_t4##_##_t5##_##_t6##_##_t7 ::getDocumentation() const { return ( _doc ); }
#define crunch_declare_libfunction_doc9( _class, _function, _t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _doc ) crunch::core::String _class##_##_function##_##_t0##_##_t1##_##_t2##_##_t3##_##_t4##_##_t5##_##_t6##_##_t7##_##_t8 ::getDocumentation() const { return ( _doc ); }
#else
#define crunch_declare_libfunction_doc( _class, _function, _doc )
#define crunch_declare_libfunction_doc1( _class, _function, _t0, _doc )
#define crunch_declare_libfunction_doc2( _class, _function, _t0, _t1, _doc )
#define crunch_declare_libfunction_doc3( _class, _function, _t0, _t1, _t2, _doc )
#define crunch_declare_libfunction_doc4( _class, _function, _t0, _t1, _t2, _t3, _doc )
#define crunch_declare_libfunction_doc5( _class, _function, _t0, _t1, _t2, _t3, _t4, _doc )
#define crunch_declare_libfunction_doc6( _class, _function, _t0, _t1, _t2, _t3, _t4, _t5, _doc )
#define crunch_declare_libfunction_doc7( _class, _function, _t0, _t1, _t2, _t3, _t4, _t5, _t6, _doc )
#define crunch_declare_libfunction_doc8( _class, _function, _t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _doc )
#define crunch_declare_libfunction_doc9( _class, _function, _t0, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _doc )
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////

// to declare a library function use crunch_declare_libfunction
// the _class_function_signature is formed by: <library-class-name>_<library-function-name>_<parameter-type-0>_<parameter-type-1>_..._<parameter-type-n>
// #define crunch_declare_libfunction( _class_function_signature ) void _class_function_signature ::entry( char *stackParams, char *stackResult )

#define crunch_declare_libfunction_p0( _class, _function, _r ) void _class##_##_function ::entry( const ScriptExecutable *executable, char *stackParams, char *stackResult )
#define crunch_declare_libfunction_p1( _class, _function, _t0, _p0, _r ) void _class##_##_function##_##_t0 ::entry( const ScriptExecutable *executable, char *stackParams, char *stackResult )
#define crunch_declare_libfunction_p2( _class, _function, _t0, _p0, _t1, _p1, _r ) void _class##_##_function##_##_t0##_##_t1 ::entry( const ScriptExecutable *executable, char *stackParams, char *stackResult )
#define crunch_declare_libfunction_p3( _class, _function, _t0, _p0, _t1, _p1, _t2, _p2, _r ) void _class##_##_function##_##_t0##_##_t1##_##_t2 ::entry( const ScriptExecutable *executable, char *stackParams, char *stackResult )
#define crunch_declare_libfunction_p4( _class, _function, _t0, _p0, _t1, _p1, _t2, _p2, _t3, _p3, _r ) void _class##_##_function##_##_t0##_##_t1##_##_t2##_##_t3 ::entry( const ScriptExecutable *executable, char *stackParams, char *stackResult )
#define crunch_declare_libfunction_p5( _class, _function, _t0, _p0, _t1, _p1, _t2, _p2, _t3, _p3, _t4, _p4, _r ) void _class##_##_function##_##_t0##_##_t1##_##_t2##_##_t3##_##_t4 ::entry( const ScriptExecutable *executable, char *stackParams, char *stackResult )
#define crunch_declare_libfunction_p6( _class, _function, _t0, _p0, _t1, _p1, _t2, _p2, _t3, _p3, _t4, _p4, _t5, _p5, _r ) void _class##_##_function##_##_t0##_##_t1##_##_t2##_##_t3##_##_t4##_##_t5 ::entry( const ScriptExecutable *executable, char *stackParams, char *stackResult )
#define crunch_declare_libfunction_p7( _class, _function, _t0, _p0, _t1, _p1, _t2, _p2, _t3, _p3, _t4, _p4, _t5, _p5, _t6, _p6, _r ) void _class##_##_function##_##_t0##_##_t1##_##_t2##_##_t3##_##_t4##_##_t5##_##_t6 ::entry( const ScriptExecutable *executable, char *stackParams, char *stackResult )
#define crunch_declare_libfunction_p8( _class, _function, _t0, _p0, _t1, _p1, _t2, _p2, _t3, _p3, _t4, _p4, _t5, _p5, _t6, _p6, _t7, _p7, _r ) void _class##_##_function##_##_t0##_##_t1##_##_t2##_##_t3##_##_t4##_##_t5##_##_t6##_##_t7 ::entry( const ScriptExecutable *executable, char *stackParams, char *stackResult )
#define crunch_declare_libfunction_p9( _class, _function, _t0, _p0, _t1, _p1, _t2, _p2, _t3, _p3, _t4, _p4, _t5, _p5, _t6, _p6, _t7, _p7, _t8, _p8, _r ) void _class##_##_function##_##_t0##_##_t1##_##_t2##_##_t3##_##_t4##_##_t5##_##_t6##_##_t7##_##_t8 ::entry( const ScriptExecutable *executable, char *stackParams, char *stackResult )

///////////////////////////////////////////////////////////////////////////////////////////////////

// place crunch_libfunction_entry_p# at the beginning of function body

#define crunch_libfunction_entry_p0() \
  struct _params { }; \
  _params *params = (_params*)( stackParams )

#define crunch_libfunction_entry_p1( _t0, _n0 ) \
  struct _params { _t0 _n0 ; }; \
  static_assert( sizeof( _params ) == ( sizeof( _t0 ) ), "invalid params" ); \
  _params *params = (_params*)( stackParams )

#define crunch_libfunction_entry_p2( _t0, _n0, _t1, _n1 ) \
  struct _params { _t0 _n0 ; _t1 _n1 ; }; \
  static_assert( sizeof( _params ) == ( sizeof( _t0 ) + sizeof( _t1 ) ), "invalid params" ); \
  _params *params = (_params*)( stackParams )

#define crunch_libfunction_entry_p3( _t0, _n0, _t1, _n1, _t2, _n2 ) \
  struct _params { _t0 _n0 ; _t1 _n1 ; _t2 _n2 ; }; \
  static_assert( sizeof( _params ) == ( sizeof( _t0 ) + sizeof( _t1 ) + sizeof( _t2 ) ), "invalid params" ); \
  _params *params = (_params*)( stackParams )

#define crunch_libfunction_entry_p4( _t0, _n0, _t1, _n1, _t2, _n2, _t3, _n3 ) \
  struct _params { _t0 _n0 ; _t1 _n1 ; _t2 _n2 ; _t3 _n3 ; }; \
  static_assert( sizeof( _params ) == ( sizeof( _t0 ) + sizeof( _t1 ) + sizeof( _t2 ) + sizeof( _t3 ) ), "invalid params" ); \
  _params *params = (_params*)( stackParams )

#define crunch_libfunction_entry_p5( _t0, _n0, _t1, _n1, _t2, _n2, _t3, _n3, _t4, _n4 ) \
  struct _params { _t0 _n0 ; _t1 _n1 ; _t2 _n2 ; _t3 _n3 ; _t4 _n4 ; }; \
  static_assert( sizeof( _params ) == ( sizeof( _t0 ) + sizeof( _t1 ) + sizeof( _t2 ) + sizeof( _t3 ) + sizeof( _t4 ) ), "invalid params" ); \
  _params *params = (_params*)( stackParams )

#define crunch_libfunction_entry_p6( _t0, _n0, _t1, _n1, _t2, _n2, _t3, _n3, _t4, _n4, _t5, _n5 ) \
  struct _params { _t0 _n0 ; _t1 _n1 ; _t2 _n2 ; _t3 _n3 ; _t4 _n4 ; _t5 _n5 ; }; \
  static_assert( sizeof( _params ) == ( sizeof( _t0 ) + sizeof( _t1 ) + sizeof( _t2 ) + sizeof( _t3 ) + sizeof( _t4 ) + sizeof( _t5 ) ), "invalid params" ); \
  _params *params = (_params*)( stackParams )

#define crunch_libfunction_entry_p7( _t0, _n0, _t1, _n1, _t2, _n2, _t3, _n3, _t4, _n4, _t5, _n5, _t6, _n6 ) \
  struct _params { _t0 _n0 ; _t1 _n1 ; _t2 _n2 ; _t3 _n3 ; _t4 _n4 ; _t5 _n5 ; _t6 _n6 ; }; \
  static_assert( sizeof( _params ) == ( sizeof( _t0 ) + sizeof( _t1 ) + sizeof( _t2 ) + sizeof( _t3 ) + sizeof( _t4 ) + sizeof( _t5 ) + sizeof( _t6 ) ), "invalid params" ); \
  _params *params = (_params*)( stackParams )

#define crunch_libfunction_entry_p8( _t0, _n0, _t1, _n1, _t2, _n2, _t3, _n3, _t4, _n4, _t5, _n5, _t6, _n6, _t7, _n7 ) \
  struct _params { _t0 _n0 ; _t1 _n1 ; _t2 _n2 ; _t3 _n3 ; _t4 _n4 ; _t5 _n5 ; _t6 _n6 ; _t7 _n7 ; }; \
  static_assert( sizeof( _params ) == ( sizeof( _t0 ) + sizeof( _t1 ) + sizeof( _t2 ) + sizeof( _t3 ) + sizeof( _t4 ) + sizeof( _t5 ) + sizeof( _t6 ) + sizeof( _t7 ) ), "invalid params" ); \
  _params *params = (_params*)( stackParams )

#define crunch_libfunction_entry_p9( _t0, _n0, _t1, _n1, _t2, _n2, _t3, _n3, _t4, _n4, _t5, _n5, _t6, _n6, _t7, _n7, _t8, _n8 ) \
  struct _params { _t0 _n0 ; _t1 _n1 ; _t2 _n2 ; _t3 _n3 ; _t4 _n4 ; _t5 _n5 ; _t6 _n6 ; _t7 _n7 ; _t8 _n8 ; }; \
  static_assert( sizeof( _params ) == ( sizeof( _t0 ) + sizeof( _t1 ) + sizeof( _t2 ) + sizeof( _t3 ) + sizeof( _t4 ) + sizeof( _t5 ) + sizeof( _t6 ) + sizeof( _t7 ) + sizeof( _t8 ) ), "invalid params" ); \
  _params *params = (_params*)( stackParams )

// only if your function returns a value
#define crunch_libfunction_return( _t, _v ) *( (_t *)( stackResult ) ) = _v

#endif
