#if !defined ( CRUNCH_SCRIPT_OBJECT_H )
#define CRUNCH_SCRIPT_OBJECT_H

#include "crunch_script_types.h"
#include "crunch_script_value.h"

#define CRUNCH_GLOBAL_SCRIPT_OBJECT_NAME "_GLOBAL_OBJECT"

namespace crunch { namespace language {

class ScriptExecutable;
class SyntaxNodeFunctionDeclaration;

class ScriptObject final
{
  private:
    class PropertyInfo
    {
      public:
        crunch::core::FixedString64 mName;
        ScriptValue mValue;

      public:
        PropertyInfo( const crunch::core::FixedString64& name = "" ) :
          mName( name )
        {
          // empty
        }

        PropertyInfo( const crunch::core::FixedString64& name, const ScriptValue& value ) :
          mName( name ),
          mValue( value )
        {
          // empty
        }

      public:
        inline bool operator < ( const PropertyInfo& rhs ) const
        {
          return ( mName.lexLess( rhs.mName ) );
        }

    };

    typedef std::set< PropertyInfo > PropertyInfoSetType;

  private:
    class FunctionInfo final
    {
      public:
        crunch::core::String mSignature;
        crunch::core::String mName;
        unsigned int mOffset = 0xFFFFFFFF;
        std::vector< ScriptTypes::ScriptTypeDescription > mParameterTypes;
        ScriptTypes::ScriptTypeDescription mReturnType;
        SyntaxNodeFunctionDeclaration *mDeclaration = nullptr;

      public:
        FunctionInfo()
        {
          // empty
        }

      public:
        inline bool operator < ( const FunctionInfo& rhs ) const
        {
          return ( mSignature < rhs.mSignature );
        }

    };

    class FunctionInfoCompare final
    {
      public:
        inline bool operator()( const FunctionInfo *lhs, const FunctionInfo *rhs ) const
        {
          return ( *lhs < *rhs );
        }
        
    };

    typedef std::set< FunctionInfo*, FunctionInfoCompare > FunctionInfoSetType;

  public:
    ScriptObject( const crunch::core::FixedString64& name = "" );
    ~ScriptObject();

  private:
    ScriptObject( const ScriptObject& other );
    ScriptObject& operator = ( const ScriptObject& other );

  public:
    void destroy();

    // this will keep the name intact, but clear out all properties and functions
    void clear();

    inline crunch::core::FixedString64 getName() const { return mName; }

  public:
    inline unsigned int getTotalProperties() const { return (unsigned int)( mProperties.size() ); }

    static bool isAllowedPropertyType( ScriptTypes::ScriptType t );

    bool hasProperty( const crunch::core::FixedString64& name ) const;

    bool addProperty( const crunch::core::FixedString64& name, const ScriptValue& v );
    bool removeProperty( const crunch::core::FixedString64& name );

    bool getPropertyValue( const crunch::core::FixedString64& name, ScriptValue *v ) const;
    bool setPropertyValue( const crunch::core::FixedString64& name, const ScriptValue& v );

    void clearProperties();

  public:
    inline unsigned int getTotalFunctions() const { return (unsigned int)( mFunctions.size() ); }

    static bool isAllowedFunctionName( const crunch::core::String& name );

    bool hasFunctionSignature( const crunch::core::String& signature ) const;

    bool addFunction( const crunch::core::String& name, 
                      unsigned int offset, 
                      const std::vector< ScriptTypes::ScriptTypeDescription >& parameterTypes, 
                      ScriptTypes::ScriptTypeDescription returnType,
                      SyntaxNodeFunctionDeclaration *declaration = nullptr
                      );

    bool getFunctionInfo( const crunch::core::String& signature, 
                          unsigned int *offset = nullptr, 
                          ScriptTypes::ScriptTypeDescription *returnType = nullptr, 
                          unsigned int *paramsStackSize = nullptr,
                          SyntaxNodeFunctionDeclaration **declaration = nullptr
                          ) const;

    bool addFunctionDependencies( const crunch::core::String& signature, ScriptObject *sourceObject, const crunch::core::String& sourceFunctionSignature );

    void clearFunctions();

  public:
    bool read( const ScriptExecutable *se, crunch::core::BinaryBuffer *source );
    bool write( const ScriptExecutable *se, crunch::core::BinaryBuffer *dest ) const;

  public:
    void appendAssembler( crunch::core::String *dest ) const;

  public:
    inline bool operator < ( const ScriptObject& rhs ) const
    {
      return ( mName.lexLess( rhs.mName ) );
    }

  private:
    FunctionInfo* add( const crunch::core::String& name, 
                       unsigned int offset, 
                       const std::vector< ScriptTypes::ScriptTypeDescription >& parameterTypes, 
                       ScriptTypes::ScriptTypeDescription returnType,
                       SyntaxNodeFunctionDeclaration *declaration = nullptr
                       );

  private:
    crunch::core::FixedString64 mName;
    PropertyInfoSetType mProperties;
    FunctionInfoSetType mFunctions;

};

class ScriptObjectCompare final
{
  public:
    inline bool operator()( const ScriptObject *lhs, const ScriptObject *rhs ) const
    {
      return ( *lhs < *rhs );
    }

};

typedef std::set< ScriptObject*, ScriptObjectCompare > ScriptObjectSetType;

} }

#endif
