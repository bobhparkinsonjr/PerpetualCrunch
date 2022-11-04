#if !defined ( CRUNCH_SCRIPT_EXECUTABLE_H )
#define CRUNCH_SCRIPT_EXECUTABLE_H

#include "crunch_script_types.h"
#include "crunch_string_table.h"
#include "crunch_script_object.h"

#include "crunch_link_table_i1.h"
#include "crunch_link_table_i2.h"

namespace crunch { namespace language {

class ScriptVM;

class ScriptExecutable final
{
  public:
    ScriptExecutable();
    ~ScriptExecutable();

  private:
    ScriptExecutable( const ScriptExecutable& other );
    ScriptExecutable& operator = ( const ScriptExecutable& other );

  public:
    void destroy( bool allowDestroyStringTable = true );

    core_forceinline ScriptVM* getVM() const { return mVM; }

  public:
    crunch::core::String* stringTableMergeString( const char *v, bool literalString, unsigned int *index = nullptr ) const;

    unsigned int stringTableGetIndex( const crunch::core::String *s ) const;
    const crunch::core::String* stringTableGetString( unsigned int index ) const;

    void stringTableAppendAssembler( crunch::core::String *dest ) const;

  public:
    core_forceinline const LinkTableI1* getLinkTable_LIB_GFUNC_CALL() const { return &mLinkTable_LIB_GFUNC_CALL; }
    core_forceinline const LinkTableI2* getLinkTable_PUSH_FETCH_GLIB_CLS() const { return &mLinkTable_PUSH_FETCH_GLIB_CLS; }
    core_forceinline const LinkTableI2* getLinkTable_LIB_FUNC_CALL() const { return &mLinkTable_LIB_FUNC_CALL; }

    core_forceinline bool isLinked() const { return ( mVM != nullptr ); }
    bool link( ScriptVM *vm );

    void appendLinkTablesAssembler( crunch::core::String *dest ) const;
    
  public:
    static bool isAllowedObjectName( const crunch::core::String& name );

    core_forceinline const ScriptObject* getGlobalObject() const { return &mGlobalObject; }

    const ScriptObject* findObject( const crunch::core::String& name ) const;

    ScriptObject* addObject( const crunch::core::String& name );
    bool removeObject( const crunch::core::String& name );

    void appendObjectTableAssembler( crunch::core::String *dest ) const;

  public:
    const crunch::core::BinaryBuffer* getByteCodeBuffer() const { return &mByteCodeBuffer; }

  public:
    bool read( crunch::core::BinaryBuffer *source );
    bool write( crunch::core::BinaryBuffer *dest ) const;

    bool readFile( const crunch::core::WideString& sourceFilePath );
    bool writeFile( const crunch::core::WideString& destFilePath ) const;

  public:
    static bool getFunctionSignature( const crunch::core::String& name, 
                                      const std::vector< ScriptTypes::ScriptTypeDescription >& parameterTypes, 
                                      crunch::core::String *signature 
                                      );

    static bool getFunctionDisplaySignature( const crunch::core::String& name, 
                                             const std::vector< ScriptTypes::ScriptTypeDescription >& parameterTypes, 
                                             crunch::core::String *displaySignature
                                             );

    // in bytes
    static unsigned int getParametersDataStackSize( const std::vector< ScriptTypes::ScriptTypeDescription >& parameterTypes );

    // utility to load up a single script executable, and run a single thread with the given root function
    // this will block until complete
    static bool run( const crunch::core::String& filepath, 
                     const crunch::core::String& rootFunctionName, 
                     const std::vector< crunch::core::String >& rootFunctionParameterTypes, 
                     const std::vector< crunch::core::String >& rootFunctionParameterValues 
                     );

  private:
    mutable StringTable mStringTable;

    ScriptVM *mVM = nullptr;

    LinkTableI1 mLinkTable_LIB_GFUNC_CALL;
    LinkTableI2 mLinkTable_PUSH_FETCH_GLIB_CLS;
    LinkTableI2 mLinkTable_LIB_FUNC_CALL;

    ScriptObject mGlobalObject;
    ScriptObjectSetType mObjects;

    crunch::core::BinaryBuffer mByteCodeBuffer;

};

} }

#endif
