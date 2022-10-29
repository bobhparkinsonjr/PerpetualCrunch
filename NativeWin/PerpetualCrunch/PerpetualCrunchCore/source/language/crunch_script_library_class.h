#if !defined ( CRUNCH_SCRIPT_LIBRARY_CLASS_H )
#define CRUNCH_SCRIPT_LIBRARY_CLASS_H

#include "crunch_script_types.h"
#include "crunch_script_library_function.h"

namespace crunch { namespace language {

class ScriptCompiler;
class SyntaxNode;

class ScriptLibraryClass
{
  private:
    typedef std::set< ScriptLibraryFunction*, ScriptLibraryFunctionCompare > FunctionSetType;

  private:
    struct MemberInfo
    {
      crunch::core::String mMemberName;
      ScriptTypes::ScriptType mType;
      crunch::core::String mTypeIdentifier;
      unsigned int mOffset;
      unsigned int mSize;

      inline bool operator < ( const MemberInfo& rhs ) const
      {
        return ( mMemberName < rhs.mMemberName );
      }
    };

    typedef std::set< MemberInfo > MemberInfoSetType;

  public:
    ScriptLibraryClass( const crunch::core::FixedString64& name = "", unsigned int dataStackSize = 0 );
    virtual ~ScriptLibraryClass();
    
  public:
    void destroy();

    virtual bool setup() { return false; }

    virtual void createGlobalSymbols( ScriptCompiler *compiler, std::vector< SyntaxNode* > *trackedNodes ) {}

    virtual crunch::core::String generateMetaInfo() const { return ""; }

    core_forceinline const crunch::core::FixedString64& getName() const { return mName; }
    core_forceinline void setName( const crunch::core::FixedString64& name ) { mName = name; }

    core_forceinline unsigned int getTotalFunctions() const { return (unsigned int)( mFunctions.size() ); }

    bool hasFunctionSignature( const crunch::core::String& signature ) const;

    // after calling this function, this library class will own the given function, it will handle deleting the given function when needed,
    //   this will happen regardless of the return value of this function
    // returns true on success
    bool add( ScriptLibraryFunction *function );

    const ScriptLibraryFunction* findFunction( const crunch::core::String& signature ) const;

    core_forceinline unsigned int getDataStackSize() const { return mDataStackSize; }

    // if this class supports the [] operator (get and/or set), and if all instances of this class use the same base type, then this
    // function needs to return the type
    virtual ScriptTypes::ScriptTypeDescription getBaseType() const { return ScriptTypes::ScriptTypeDescription(); }

  public:
    bool setMemberInfo( const crunch::core::String& memberName, ScriptTypes::ScriptType v, const crunch::core::String& typeIdentifier, unsigned int offset, unsigned int size );

    // returns false if memberName doesn't exist
    // on success, returns true, and offset will contain the offset in bytes to the member from the start of the class (eg. 0 for the first member),
    //   size will contain the size in bytes of the member
    bool getMemberInfo( const crunch::core::String& memberName, ScriptTypes::ScriptType *v, crunch::core::String *typeIdentifier, unsigned int *offset, unsigned int *size ) const;

    // returns true if this library class has member variables
    bool hasMembers() const;

    crunch::core::String generateMemberMetaInfo() const;

  public:
    #if defined CRUNCH_LIBRARY_ENABLE_DOCUMENTATION
    core_forceinline crunch::core::String getDocumentation() const { return mDocumentation; }
    void consolePrintAllDocumentation() const;
    void consolePrintFunctionDocumentation( const crunch::core::String& functionName ) const;
    void consolePrintFunctionSignatureDocumentation( const crunch::core::String& functionSignature ) const;
    #endif

  public:
    core_forceinline bool operator < ( const ScriptLibraryClass& rhs ) const
    {
      return ( mName.lexLess( rhs.mName ) );
    }

  private:
    crunch::core::FixedString64 mName;
    FunctionSetType mFunctions;
    unsigned int mDataStackSize = 0;
    MemberInfoSetType mMembers;

  protected:
    #if defined CRUNCH_LIBRARY_ENABLE_DOCUMENTATION
    crunch::core::String mDocumentation;
    #endif

};

class ScriptLibraryClassCompare final
{
  public:
    core_forceinline bool operator()( const ScriptLibraryClass *lhs, const ScriptLibraryClass *rhs ) const
    {
      return ( *lhs < *rhs );
    }

};

} }

#endif
