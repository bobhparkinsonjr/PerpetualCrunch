#if !defined ( CRUNCH_SCRIPT_COMPILER_H )
#define CRUNCH_SCRIPT_COMPILER_H

#include "../core/core_mutex.h"

#include "syntax tree/crunch_syntax_nodes.h"
#include "crunch_script_types.h"
#include "crunch_symbol_stack.h"
#include "crunch_script_executable.h"
#include "crunch_script_context_stack.h"

#define CRUNCH_COMPILER_ENABLE_ASSEMBLER

namespace crunch { namespace language {

class ScriptCompiler final
{
  public:
    enum { VERSION_MAJOR = 2 };
    enum { VERSION_MINOR = 0 };

  public:
    enum class PassType
    {
      PASS_SYMBOL_GENERATION,
      PASS_CODE_GENERATION
    };

  public:
    typedef enum
    {
      FLAG_PRINT_ERRORS = 1 << 0,
      FLAG_PRINT_WARNINGS = 1 << 1,

      FLAG_CORE_PRINT_SYNTAX_TREE = 1 << 2,
      FLAG_CORE_PRINT_ASSEMBLER = 1 << 3,

      FLAG_ASSEMBLER_FILE = 1 << 4,
      FLAG_SYNTAX_TREE_FILE = 1 << 5,

      #if defined _CRUNCH_DEBUG
      FLAG_DEFAULT_SETTINGS = FLAG_PRINT_ERRORS | FLAG_PRINT_WARNINGS | FLAG_ASSEMBLER_FILE
      #else
      FLAG_DEFAULT_SETTINGS = FLAG_PRINT_ERRORS | FLAG_PRINT_WARNINGS
      #endif
    } FlagType;

  public:
    ScriptCompiler( unsigned int flags = FLAG_DEFAULT_SETTINGS );
    ~ScriptCompiler();

  public:
    static void setup();

  public:
    void destroy();

    // if any root functions are identified, then dead stripping will be performed
    // this function should only be called before build
    bool identifyRootFunction( const crunch::core::String& functionSignature );

    // if destBuffer is not the nullptr, it will write the script executable into the destBuffer,
    //   otherwise the script executable will be written to a file based on filepath, with the extension ".crunchy"
    // outputName can be used to override all file names written, it should only give the name, no folder or extension info
    bool build( const crunch::core::WideString& filepath, 
                crunch::core::BinaryBuffer *destBuffer = nullptr, 
                const crunch::core::WideString& intermediateFolder = L"", 
                const crunch::core::WideString& outputName = L"" 
                );

    // same as build, but instead of a file for input, text contains all the input text to parse
    bool buildText( const crunch::core::String& text, 
                    crunch::core::BinaryBuffer *destBuffer = nullptr, 
                    const crunch::core::WideString& intermediateFolder = L"", 
                    const crunch::core::WideString& outputName = L"" 
                    );

  private:
    // caller responsible for deleting *includeRoot
    bool parseIncludeItem( const crunch::core::String& itemName, SyntaxNodeRoot *sourceRoot, SyntaxNodeRoot **includeRoot );

  public:
    core_forceinline bool isPrintErrors() const { return isFlagged( FLAG_PRINT_ERRORS ); }
    core_forceinline bool isPrintWarnings() const { return isFlagged( FLAG_PRINT_WARNINGS ); }

    core_forceinline unsigned int getTotalErrors() const { return mErrorCounter; }
    core_forceinline unsigned int getTotalWarnings() const { return mWarningCounter; }

    void reportInternalError( const crunch::core::WideString& parseFilePath, unsigned int lineNumber, const char *format, ... ) const;

    void reportError( const crunch::core::WideString& parseFilePath, unsigned int lineNumber, const char *format, ... ) const;
    void reportWarning( const crunch::core::WideString& parseFilePath, unsigned int lineNumber, const char *format, ... ) const;

  public:
    core_forceinline bool isGenerateAssembler() const { return ( isFlagged( FLAG_CORE_PRINT_ASSEMBLER ) || isFlagged( FLAG_ASSEMBLER_FILE ) ); }

  private:
    #if defined CRUNCH_COMPILER_ENABLE_ASSEMBLER
    void appendAssemblerLine( const crunch::core::WideString& parseFilePath, unsigned int lineNumber, const crunch::core::String& line );
    void appendAssemblerOpLine( const crunch::core::WideString& parseFilePath, unsigned int lineNumber, const crunch::core::String& op, const crunch::core::String& args, const crunch::core::String& comment = "" );
    #endif

  public:
    void reportCurrentScanLine( const crunch::core::WideString& parseFilePath, unsigned int lineNumber, const crunch::core::String& currentScanLine );

  public:
    crunch::core::String* stringTableMergeString( const char *v, bool literalString, unsigned int *index = nullptr );

    unsigned int stringTableGetIndex( const crunch::core::String *s );

  public:
    const ScriptObject* getGlobalObject() const;

    const ScriptObject* findObject( const crunch::core::String& name );

    ScriptObject* addObject( const crunch::core::String& name );

    void createGlobalSymbol( const crunch::core::String& name, SyntaxNodeLiteralType *literalNode );

  public:
    core_forceinline ScriptContextStack* getContextStack() { return &mContextStack; }

  public:
    // returns true if the given function signature was found
    // on success, parentContextName is the name of the class/object where the function lives,
    //   for the global library class that is: _GLOBAL_CLASS (use define BML_MS_LIB_GLOBAL_CLASS_NAME)
    //   for the global script object that is: _GLOBAL_OBJECT (use define BML_MS_GLOBAL_SCRIPT_OBJECT_NAME)
    bool findFunctionInfo( SyntaxNode *parentContextNode, 
                           const crunch::core::String& functionSignature, 
                           ScriptTypes::ScriptType *objectType, 
                           unsigned int *offset, 
                           ScriptTypes::ScriptTypeDescription *returnType,
                           crunch::core::String *parentContextName,
                           unsigned int *paramsStackSize, // this never includes the size of a this pointer
                           SyntaxNodeFunctionDeclaration **declaration
                           ) const;

  public:
    core_forceinline SymbolStack* getSymbolStack() const { return mSymbolStack; }
    core_forceinline const ScriptExecutable* getExecutable() const { return &mExecutable; }

  public:
    bool isDeadStrippingEnabled() const;
    bool isRootFunction( const crunch::core::String& functionSignature ) const;

  public:
    void reportDependencies( ScriptObject *so, const crunch::core::String& functionSignature );

  private:
    void destroyTrackedNodes();

    core_forceinline bool isFlagged( FlagType f ) const { return ( ( mFlags & f ) != 0 ); }

    void generateAssembler( crunch::core::String *dest ) const;

    void setupGlobalSymbols();

    void exportSyntaxTree( SyntaxNodeRoot *root, const crunch::core::WideString& fileSuffix );

    void deadStrip( SyntaxNodeRoot *root );

    bool build( const crunch::core::WideString& inputSource,
                bool filePathInputSource,
                crunch::core::BinaryBuffer *destBuffer,
                const crunch::core::WideString& intermediateFolder,
                const crunch::core::WideString& outputName
                );

  private:
    static crunch::core::CoreMutex mBuildMutex;

  private:
    crunch::core::WideFileString mSourcePath;
    crunch::core::WideString mIntermediateFolder;
    crunch::core::WideString mOutputName;

    mutable unsigned int mErrorCounter = 0;
    mutable unsigned int mWarningCounter = 0;

    unsigned int mFlags = 0;

    ScriptExecutable mExecutable;

    SymbolStack *mSymbolStack = nullptr;

    std::vector< SyntaxNode* > mTrackedNodes;

    typedef std::set< crunch::core::String > RootFunctionSetType;
    RootFunctionSetType mRootFunctions;

    ScriptContextStack mContextStack;

    PassType mPass = PassType::PASS_SYMBOL_GENERATION;

    bool mEnableCreateGlobalSymbol = false;

  private:
    #if defined CRUNCH_COMPILER_ENABLE_ASSEMBLER

    std::vector< crunch::core::String > mAssembler;

    struct ScanLineInfo
    {
      unsigned int mLineNumber = 0;
      crunch::core::String mLine;

      ScanLineInfo( unsigned int lineNumber = 0, const crunch::core::String& line = "" ) :
        mLineNumber( lineNumber ),
        mLine( line )
      {
        // empty
      }

    };

    struct ScanLineFileInfo
    {
      crunch::core::WideString mParseFilePath;
      std::vector< ScanLineInfo > mScanLines;
      unsigned int mCurrentScanLineIndex = 0;

      inline bool operator < ( const ScanLineFileInfo& rhs ) const
      {
        return ( mParseFilePath < rhs.mParseFilePath );
      }
    };

    std::set< ScanLineFileInfo > mScanLineFiles;
    crunch::core::WideString mCurrentScanLineFilePath;

    #endif

  friend class ScriptInstruction;

  friend class SyntaxNodeInclude;
  friend class SyntaxNodeExpression;
  friend class SyntaxNodeFunctionDeclaration;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#if defined CRUNCH_COMPILER_ENABLE_ASSEMBLER
  #define CRUNCH_MCOMPILER_APPEND_ASSEMBLER( _parseFilePath, _lineNumber, _line ) if ( mCompiler->isGenerateAssembler() ) mCompiler->appendAssemblerLine( _parseFilePath, _lineNumber, _line )
  #define CRUNCH_MCOMPILER_APPEND_ASSEMBLER_OP( _parseFilePath, _lineNumber, _op, _args ) if ( mCompiler->isGenerateAssembler() ) mCompiler->appendAssemblerOpLine( _parseFilePath, _lineNumber, _op, _args )
  #define CRUNCH_MCOMPILER_APPEND_ASSEMBLER_OP_COM( _parseFilePath, _lineNumber, _op, _args, _comment ) if ( mCompiler->isGenerateAssembler() ) mCompiler->appendAssemblerOpLine( _parseFilePath, _lineNumber, _op, _args, _comment )
  #define CRUNCH_COMPILER_APPEND_ASSEMBLER( _parseFilePath, _lineNumber, _line ) if ( compiler->isGenerateAssembler() ) compiler->appendAssemblerLine( _parseFilePath, _lineNumber, _line )
  #define CRUNCH_COMPILER_APPEND_ASSEMBLER_OP( _parseFilePath, _lineNumber, _op, _args ) if ( compiler->isGenerateAssembler() ) compiler->appendAssemblerOpLine( _parseFilePath, _lineNumber, _op, _args )
  #define CRUNCH_COMPILER_APPEND_ASSEMBLER_OP_COM( _parseFilePath, _lineNumber, _op, _args, _comment ) if ( compiler->isGenerateAssembler() ) compiler->appendAssemblerOpLine( _parseFilePath, _lineNumber, _op, _args, _comment )
#else
  #define CRUNCH_MCOMPILER_APPEND_ASSEMBLER( _parseFilePath, _lineNumber, _line )
  #define CRUNCH_MCOMPILER_APPEND_ASSEMBLER_OP( _parseFilePath, _lineNumber, _op, _args )
  #define CRUNCH_MCOMPILER_APPEND_ASSEMBLER_OP_COM( _parseFilePath, _lineNumber, _op, _args, _comment )
  #define CRUNCH_COMPILER_APPEND_ASSEMBLER( _parseFilePath, _lineNumber, _line )
  #define CRUNCH_COMPILER_APPEND_ASSEMBLER_OP( _parseFilePath, _lineNumber, _op, _args )
  #define CRUNCH_COMPILER_APPEND_ASSEMBLER_OP_COM( _parseFilePath, _lineNumber, _op, _args, _comment )
#endif

} }

#endif
