
#include "../crunch_global.h"

#include "../core/core_binary_buffer.h"
#include "../core/core_text_file.h"

#include "crunch_script_library.h"
#include "syntax tree/crunch_syntax_nodes.h"
#include "crunch_symbol_stack.h"

#include "crunch_script_compiler.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

extern void crunchParseFile( const crunch::core::WideString& filepath, crunch::language::ScriptCompiler *compiler, crunch::language::SyntaxNodeRoot *root );
extern void crunchParseString( const crunch::core::String& text, crunch::language::ScriptCompiler *compiler, crunch::language::SyntaxNodeRoot *root );

extern void crunchStringPoolCreate();
extern void crunchStringPoolDestroy();

extern void crunchClearCurrentScanLine();

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

///////////////////////////////////////////////////////////////////////////////////////////////////

crunch::core::CoreMutex ScriptCompiler::mBuildMutex;

///////////////////////////////////////////////////////////////////////////////////////////////////

ScriptCompiler::ScriptCompiler( unsigned int flags ) :
  mFlags( flags )
{
  // empty
}

ScriptCompiler::~ScriptCompiler()
{
  destroy();
}

void ScriptCompiler::setup()
{
  mBuildMutex.create();
  ScriptLibrary::setup();
}

void ScriptCompiler::destroy()
{
  mContextStack.clear();
  mExecutable.destroy();
  destroyTrackedNodes();

  if ( mSymbolStack != nullptr )
  {
    delete mSymbolStack;
    mSymbolStack = nullptr;
  }

  mErrorCounter = 0;
  mWarningCounter = 0;

  #if defined CRUNCH_COMPILER_ENABLE_ASSEMBLER
  mAssembler.clear();
  #endif
}

bool ScriptCompiler::identifyRootFunction( const crunch::core::String& functionSignature )
{
  // corePrint( "root function: '%s'", functionSignature.c_str() );

  std::pair< RootFunctionSetType::iterator, bool > ret = mRootFunctions.insert( functionSignature );

  return ret.second;
}

bool ScriptCompiler::build( const crunch::core::WideString& filepath, 
                            crunch::core::BinaryBuffer *destBuffer, 
                            const crunch::core::WideString& intermediateFolder, 
                            const crunch::core::WideString& outputName 
                            )
{
  return build( filepath, true, destBuffer, intermediateFolder, outputName );
}

bool ScriptCompiler::buildText( const crunch::core::String& text, 
                                crunch::core::BinaryBuffer *destBuffer, 
                                const crunch::core::WideString& intermediateFolder, 
                                const crunch::core::WideString& outputName
                                )
{
  return build( text, false, destBuffer, intermediateFolder, outputName );
}

bool ScriptCompiler::parseIncludeItem( const crunch::core::String& itemName, SyntaxNodeRoot *sourceRoot, SyntaxNodeRoot **includeRoot )
{
  if ( includeRoot == nullptr )
  {
    reportInternalError( L"", 0, "missing include root" );
    return false;
  }

  if ( sourceRoot == nullptr )
  {
    reportInternalError( L"", 0, "missing source root for include" );
    return false;
  }

  SyntaxNodeRoot *root = new SyntaxNodeRoot( this );

  crunch::core::WideString parseFilePath;

  if ( int( itemName.find( ":" ) ) < 0 )
  {
    // parseFilePath = mSourcePath.getDir() + itemName;

    crunch::core::WideFileString p( sourceRoot->getSourceFilePath() );
    parseFilePath = p.getDir() + itemName;
  }
  else
    parseFilePath = itemName;

  if ( int( itemName.find( "." ) ) < 0 )
  {
    if ( mSourcePath.getExt() != L"" )
      parseFilePath += mSourcePath.getExt();
    else
      parseFilePath += L".crunch";
  }

  // corePrint( "parse file path for include: '%s'", parseFilePath.strx().c_str() );

  crunchParseFile( parseFilePath, this, root );

  *includeRoot = root;

  return true;
}

void ScriptCompiler::reportInternalError( const crunch::core::WideString& parseFilePath, unsigned int lineNumber, const char *format, ... ) const
{
	va_list args;
	va_start( args, format );
  crunch::core::String message = crunch::core::String::fmtv( format, args );
  va_end( args );

  ++mErrorCounter;

  if ( isPrintErrors() )
  {
    if ( lineNumber > 0 )
    {
      printf( "%s(%u): internal error: %s\n", parseFilePath.toString().c_str(), lineNumber, message.c_str() );
    }
    else
    {
      printf( "%s: internal error: %s\n", parseFilePath.toString().c_str(), message.c_str() );
    }
  }
}

void ScriptCompiler::reportError( const crunch::core::WideString& parseFilePath, unsigned int lineNumber, const char *format, ... ) const
{
	va_list args;
	va_start( args, format );
  crunch::core::String message = crunch::core::String::fmtv( format, args );
  va_end( args );

  ++mErrorCounter;

  if ( isPrintErrors() )
  {
    if ( lineNumber > 0 )
    {
      printf( "%s(%u): error: %s\n", parseFilePath.toString().c_str(), lineNumber, message.c_str() );
    }
    else
    {
      printf( "%s: error: %s\n", parseFilePath.toString().c_str(), message.c_str() );
    }
  }
}

void ScriptCompiler::reportWarning( const crunch::core::WideString& parseFilePath, unsigned int lineNumber, const char *format, ... ) const
{
	va_list args;
	va_start( args, format );
  crunch::core::String message = crunch::core::String::fmtv( format, args );
  va_end( args );

  ++mWarningCounter;

  if ( isPrintWarnings() )
  {
    if ( lineNumber > 0 )
    {
      printf( "%s(%u): warning: %s\n", parseFilePath.toString().c_str(), lineNumber, message.c_str() );
    }
    else
    {
      printf( "%s: warning: %s\n", parseFilePath.toString().c_str(), message.c_str() );
    }
  }
}

#if defined CRUNCH_COMPILER_ENABLE_ASSEMBLER
void ScriptCompiler::appendAssemblerLine( const crunch::core::WideString& parseFilePath, unsigned int lineNumber, const crunch::core::String& line )
{
  if ( !( isGenerateAssembler() ) || mPass != PassType::PASS_CODE_GENERATION )
    return;

  if ( lineNumber > 0 )
  {
    if ( mCurrentScanLineFilePath != parseFilePath )
    {
      mCurrentScanLineFilePath = parseFilePath;
      mAssembler.push_back( "\n  ; ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" );
      mAssembler.push_back( crunch::core::String::fmt( "\n  ; %s", mCurrentScanLineFilePath.toString().c_str() ) );
    }

    ScanLineFileInfo findMe;
    findMe.mParseFilePath = parseFilePath;

    std::set< ScanLineFileInfo >::iterator findIt = mScanLineFiles.find( findMe );

    if ( findIt != mScanLineFiles.end() )
    {
      ScanLineFileInfo *info = (ScanLineFileInfo*)( &( *findIt ) );

      while ( info->mCurrentScanLineIndex < (unsigned int)( info->mScanLines.size() ) && lineNumber >= info->mScanLines[info->mCurrentScanLineIndex].mLineNumber )
      {
        mAssembler.push_back( crunch::core::String::fmt( "\n  ; #%u: %s", info->mScanLines[info->mCurrentScanLineIndex].mLineNumber, info->mScanLines[info->mCurrentScanLineIndex].mLine.c_str() ) );
        ++( info->mCurrentScanLineIndex );
      }
    }
  }

  if ( line != "" && line[0] == '\n' )
    mAssembler.push_back( line );
  else
  {
    crunch::core::String trimLine = line;
    trimLine.trimWS();
    
    if ( trimLine != "" && trimLine[0] == ';' )
      mAssembler.push_back( line );
    else
      mAssembler.push_back( crunch::core::String::fmt( "%08d: %s", int( mAssembler.size() ) + 1, line.c_str() ) );
  }
}
#endif

#if defined CRUNCH_COMPILER_ENABLE_ASSEMBLER
void ScriptCompiler::appendAssemblerOpLine( const crunch::core::WideString& parseFilePath, unsigned int lineNumber, const crunch::core::String& op, const crunch::core::String& args, const crunch::core::String& comment )
{
  if ( !( isGenerateAssembler() ) || mPass != PassType::PASS_CODE_GENERATION )
    return;

  if ( comment == "" )
    appendAssemblerLine( parseFilePath, lineNumber, crunch::core::String::fmt( "%-40s%s", op.c_str(), args.c_str() ) );
  else
    appendAssemblerLine( parseFilePath, lineNumber, crunch::core::String::fmt( "%-40s%-70s ; %s", op.c_str(), args.c_str(), comment.c_str() ) );
}
#endif

void ScriptCompiler::reportCurrentScanLine( const crunch::core::WideString& parseFilePath, unsigned int lineNumber, const crunch::core::String& currentScanLine )
{
  #if defined CRUNCH_COMPILER_ENABLE_ASSEMBLER

  crunch::core::String s = currentScanLine;
  s.trimWS();

  if ( s != "" )
  {
    ScanLineFileInfo *info;

    ScanLineFileInfo findMe;
    findMe.mParseFilePath = parseFilePath;

    std::set< ScanLineFileInfo >::iterator findIt = mScanLineFiles.find( findMe );

    if ( findIt != mScanLineFiles.end() )
      info = (ScanLineFileInfo*)( &( *findIt ) );
    else
    {
      std::pair< std::set< ScanLineFileInfo >::iterator, bool > ret = mScanLineFiles.insert( findMe );
      if ( !( ret.second ) )
        return;
      info = (ScanLineFileInfo*)( &( *( ret.first ) ) );
    }

    // corePrint( "current scan line %u: '%s'", lineNumber, s.c_str() );
    info->mScanLines.push_back( ScanLineInfo( lineNumber, s ) );
  }

  #endif
}

crunch::core::String* ScriptCompiler::stringTableMergeString( const char *v, bool literalString, unsigned int *index )
{
  return mExecutable.stringTableMergeString( v, literalString, index );
}

unsigned int ScriptCompiler::stringTableGetIndex( const crunch::core::String *s )
{
  return mExecutable.stringTableGetIndex( s );
}

const ScriptObject* ScriptCompiler::getGlobalObject() const
{
  return mExecutable.getGlobalObject();
}

const ScriptObject* ScriptCompiler::findObject( const crunch::core::String& name )
{
  return mExecutable.findObject( name );
}

ScriptObject* ScriptCompiler::addObject( const crunch::core::String& name )
{
  return mExecutable.addObject( name );
}

bool ScriptCompiler::findFunctionInfo( SyntaxNode *parentContextNode, 
                                       const crunch::core::String& functionSignature, 
                                       ScriptTypes::ScriptType *objectType, 
                                       unsigned int *offset, 
                                       ScriptTypes::ScriptTypeDescription *returnType,
                                       crunch::core::String *parentContextName,
                                       unsigned int *paramsStackSize,
                                       SyntaxNodeFunctionDeclaration **declaration
                                       ) const
{
  const ScriptLibraryFunction *libraryFunction = nullptr;
  ScriptTypes::ScriptTypeDescription parentTypeDescription;

  if ( declaration != nullptr )
    *declaration = nullptr;

  if ( parentContextNode != nullptr )
  {
    // check only the library class

    parentTypeDescription = parentContextNode->getScriptTypeDescription();

    const ScriptLibraryClass *libraryClass = ScriptLibrary::findLibraryClass( parentTypeDescription.mIdentifier.c_str() );

    if ( libraryClass != nullptr )
    {
      libraryFunction = libraryClass->findFunction( functionSignature );

      if ( libraryFunction == nullptr )
        parentContextNode->reportError( "member library function '%s' of class '%s' not found", functionSignature.c_str(), parentTypeDescription.mIdentifier.c_str() );
    }
    else
      parentContextNode->reportError( "library class '%s' not found", parentTypeDescription.mIdentifier.c_str() );
  }
  else
  {
    // check global object

    if ( getGlobalObject()->getFunctionInfo( functionSignature, offset, returnType, paramsStackSize, declaration ) )
    {
      if ( objectType != nullptr )
        *objectType = ScriptTypes::ScriptType::TYPE_SCRIPT_OBJECT;

      if ( parentContextName != nullptr )
      {
        *parentContextName = getGlobalObject()->getName().c_str();

        if ( *parentContextName == "" )
          reportInternalError( L"", 0, "global script object missing name" );
      }

      return true;
    }

    // check global library class
    
    libraryFunction = ScriptLibrary::getGlobalLibraryClass()->findFunction( functionSignature );

    // if ( libraryFunction == nullptr )
      // reportError( 0, "global object/library function '%s' not found", functionSignature.c_str() );
  }

  if ( libraryFunction != nullptr )
  {
    if ( objectType != nullptr )
      *objectType = ScriptTypes::ScriptType::TYPE_LIBRARY_CLASS;

    if ( parentContextName != nullptr )
    {
      if ( parentContextNode != nullptr )
        *parentContextName = parentTypeDescription.mIdentifier;
      else
        *parentContextName = ScriptLibrary::getGlobalLibraryClass()->getName().c_str();
    }
      
    if ( offset != nullptr )
      *offset = 0;

    if ( returnType != nullptr )
      *returnType = libraryFunction->getReturnType();

    if ( paramsStackSize != nullptr )
      *paramsStackSize = libraryFunction->getParametersDataStackSize();

    return true;  
  }

  return false;
}

bool ScriptCompiler::isDeadStrippingEnabled() const
{
  return ( mPass == PassType::PASS_CODE_GENERATION && !( mRootFunctions.empty() ) );
}

bool ScriptCompiler::isRootFunction( const crunch::core::String& functionSignature ) const
{
  if ( mRootFunctions.empty() )
    return true;

  RootFunctionSetType::const_iterator findIt = mRootFunctions.find( functionSignature );
  return ( findIt != mRootFunctions.end() );
}

void ScriptCompiler::reportDependencies( ScriptObject *so, const crunch::core::String& functionSignature )
{
  SyntaxNodeFunctionDeclaration *currentFunction = SyntaxNodeFunctionDeclaration::getCurrentFunctionDeclaration();

  if ( currentFunction == nullptr )
  {
    reportInternalError( L"", 0, "no current function for dependencies" );
    return;
  }

  currentFunction->reportDependencies( so, functionSignature );
}

void ScriptCompiler::destroyTrackedNodes()
{
  unsigned int total = (unsigned int)( mTrackedNodes.size() );

  for ( unsigned int i = 0; i < total; ++i )
    delete mTrackedNodes[i];

  mTrackedNodes.clear();
}

void ScriptCompiler::generateAssembler( crunch::core::String *dest ) const
{
  *dest = "";

  #if defined CRUNCH_COMPILER_ENABLE_ASSEMBLER

  if ( !( isGenerateAssembler() ) )
    return;

  mExecutable.stringTableAppendAssembler( dest );
  mExecutable.appendLinkTablesAssembler( dest );
  mExecutable.appendObjectTableAssembler( dest );

  int total = int( mAssembler.size() );

  *dest += crunch::core::String::fmt( "BYTE CODE (lines:%d)\n", total );

  for ( int i = 0; i < total; ++i )
    *dest += crunch::core::String::fmt( "  %s\n", mAssembler[i].c_str() );

  #endif
}

void ScriptCompiler::setupGlobalSymbols()
{
  mTrackedNodes.push_back( new SyntaxNodeLiteralType( this, nullptr, 0, crunch_i64( 1 ) ) );
  mSymbolStack->pushConstantSymbol( "true", (SyntaxNodeLiteralType*)( mTrackedNodes.back() ) );

  mTrackedNodes.push_back( new SyntaxNodeLiteralType( this, nullptr, 0, crunch_i64( 0 ) ) );
  mSymbolStack->pushConstantSymbol( "false", (SyntaxNodeLiteralType*)( mTrackedNodes.back() ) );
}

void ScriptCompiler::exportSyntaxTree( SyntaxNodeRoot *root, const crunch::core::WideString& fileSuffix )
{
  if ( !( isFlagged( FlagType::FLAG_CORE_PRINT_SYNTAX_TREE ) ) && !( isFlagged( FlagType::FLAG_SYNTAX_TREE_FILE ) ) )
    return;

  if ( fileSuffix == L"" )
    return;

  crunch::core::String treeDescription = root->getSyntaxTreeDescription();

  if ( isFlagged( FlagType::FLAG_CORE_PRINT_SYNTAX_TREE ) )
  {
    corePrint( "syntax tree:\n%s", treeDescription.c_str() );
  }

  if ( isFlagged( FlagType::FLAG_SYNTAX_TREE_FILE ) )
  {
    crunch::core::WideString destFilePath = mOutputName + crunch::core::WideString( L".crunch" ) + fileSuffix;

    if ( mIntermediateFolder != L"" )
      destFilePath = mIntermediateFolder + destFilePath;
    else
      destFilePath = mSourcePath.getDir() + destFilePath;

    crunch::core::TextFile::writeAllText( destFilePath, treeDescription );
  }
}

void ScriptCompiler::deadStrip( SyntaxNodeRoot *root )
{
  if ( root == nullptr || mRootFunctions.empty() )
    return;

  // corePrint( "total root functions: %d", int( mRootFunctions.size() ) );

  root->queryAliveStatus();
}

bool ScriptCompiler::build( const crunch::core::WideString& inputSource, 
                            bool filePathInputSource,
                            crunch::core::BinaryBuffer *destBuffer, 
                            const crunch::core::WideString& intermediateFolder, 
                            const crunch::core::WideString& outputName 
                            )
{
  scoped_lock( &mBuildMutex );

  if ( filePathInputSource )
    mSourcePath = inputSource;
  else
    mSourcePath.clear();

  mIntermediateFolder = intermediateFolder;

  if ( outputName != L"" )
    mOutputName = outputName;
  else
    mOutputName = mSourcePath.getName();

  if ( mOutputName == L"" )
    mOutputName = L"output";

  if ( mIntermediateFolder != L"" && mIntermediateFolder[mIntermediateFolder.size() - 1] != _CORE_PATH_SEP_WCH )
    mIntermediateFolder += _CORE_PATH_SEP_WSTR;

  if ( mSourcePath.getDir() == L"" )
  {
    crunch::core::WideString dir;
    if ( crunch::core::FileTools::getCurrentWorkingDirectory( &dir ) )
      mSourcePath.setDir( dir );
  }

  ScriptLibrary::setup();
  ScriptLibrary::setup( this, &mTrackedNodes );

  mContextStack.clear();
  mExecutable.destroy();

  if ( mSymbolStack == nullptr )
    mSymbolStack = new SymbolStack( this );

  setupGlobalSymbols();

  SyntaxNodeRoot *root = new SyntaxNodeRoot( this );

  crunchStringPoolCreate();
  crunchClearCurrentScanLine();

  if ( filePathInputSource )
    crunchParseFile( inputSource, this, root );
  else
    crunchParseString( inputSource.toString(), this, root );

  root->parseIncludeNodes();

  exportSyntaxTree( root, L"t" );

  if ( mErrorCounter == 0 )
  {
    root->applyConstantFolding();
    exportSyntaxTree( root, L"tf" );

    // pass symbol generation : generate throw away byte code buffer to allow syntax tree to finish some link operations
    {
      mPass = PassType::PASS_SYMBOL_GENERATION;

      crunch::core::BinaryBuffer tempBuffer;
      tempBuffer.setup( 1024 * 1024 * 16, 1024 * 1024 * 1 );
      root->generateCode( &tempBuffer );

      mExecutable.destroy();
    }

    mPass = PassType::PASS_CODE_GENERATION;

    deadStrip( root );

    crunch::core::BinaryBuffer *byteCodeBuffer = (crunch::core::BinaryBuffer*)( mExecutable.getByteCodeBuffer() );

    if ( byteCodeBuffer->setup( 1024 * 1024 * 16, 1024 * 1024 * 1 ) )
    {
      if ( root->generateCode( byteCodeBuffer ) )
      {
        if ( destBuffer != nullptr )
        {
          if ( destBuffer->getBufferExpansionSize() == 0 )
            destBuffer->setup( 1024 * 1024 * 16, 1024 * 1024 * 1 );

          if ( !( mExecutable.write( destBuffer ) ) )
            reportInternalError( L"", 0, "failed to write executable to destination binary buffer" );
        }
        else
        {
          crunch::core::WideString destFilePath = mSourcePath.getDir() + mOutputName + crunch::core::WideString( L".crunchy" );

          if ( !( mExecutable.writeFile( destFilePath ) ) )
            reportInternalError( L"", 0, "failed to write output file '%s'", destFilePath.toString().c_str() );
        }
      }
      else
      if ( mErrorCounter == 0 )
        reportInternalError( L"", 0, "unknown compile error" );
    }
    else
      reportInternalError( L"", 0, "failed to setup output buffer" );
  }

  // assembler
  {
    crunch::core::String assemblerContent;
    generateAssembler( &assemblerContent );

    if ( assemblerContent != "" )
    {
      if ( isFlagged( FlagType::FLAG_CORE_PRINT_ASSEMBLER ) )
      {
        corePrint( assemblerContent.c_str() );
      }

      if ( isFlagged( FlagType::FLAG_ASSEMBLER_FILE ) )
      {
        crunch::core::WideString asmDestFilePath = mOutputName + crunch::core::WideString( L".crunchasm" );

        if ( mIntermediateFolder != L"" )
          asmDestFilePath = mIntermediateFolder + asmDestFilePath;
        else
          asmDestFilePath = mSourcePath.getDir() + asmDestFilePath;

        crunch::core::TextFile::writeAllText( asmDestFilePath, assemblerContent );
        // corePrint( "wrote assembler file '%s'", asmDestFilePath.strx().c_str() );
      }
    }
  }

  delete root;

  if ( mSymbolStack != nullptr )
  {
    delete mSymbolStack;
    mSymbolStack = nullptr;
  }

  destroyTrackedNodes();

  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

} }
