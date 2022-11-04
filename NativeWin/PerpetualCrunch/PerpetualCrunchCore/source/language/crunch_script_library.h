#if !defined ( CRUNCH_SCRIPT_LIBRARY_H )
#define CRUNCH_SCRIPT_LIBRARY_H

#include "crunch_script_library_class.h"

namespace crunch { namespace language {

class ScriptCompiler;
class SyntaxNode;

class ScriptLibrary final
{
  private:
    typedef std::set< ScriptLibraryClass*, ScriptLibraryClassCompare > ScriptLibraryClassSetType;

  public:
    // only needs to be called once, safe to call multiple times
    static void setup();

    // call once per compiler
    static void setup( ScriptCompiler *compiler );

    static void destroy();

    static core_forceinline const ScriptLibraryClass* getGlobalLibraryClass() { return mGlobalLibraryClass; }

    static bool hasLibraryClass( const crunch::core::FixedString64& name );
    static const ScriptLibraryClass* findLibraryClass( const crunch::core::FixedString64& name );

  public:
    static void consolePrintAllDocumentation();
    static void consolePrintClassDocumentation( const crunch::core::String& className );
    static void consolePrintFunctionDocumentation( const crunch::core::String& className, const crunch::core::String& functionName );
    static void consolePrintFunctionSignatureDocumentation( const crunch::core::String& className, const crunch::core::String& functionSignature );

  private:
    static void setupGlobalLibraryClass();

  private:
    static bool mSetup;
    static ScriptLibraryClass *mGlobalLibraryClass;
    static ScriptLibraryClassSetType mLibraryClasses;

};

} }

#endif
