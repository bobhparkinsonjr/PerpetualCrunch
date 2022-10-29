#if !defined ( CRUNCH_SCRIPT_CONTEXT_STACK_H )
#define CRUNCH_SCRIPT_CONTEXT_STACK_H

namespace crunch { namespace language {

class ScriptContextStack
{
  public:
    enum class Context
    {
      NONE,

      FETCH,
      STORE,

      BEGIN_SCOPE,
      END_SCOPE
    };

  public:
    void clear() { mStack.clear(); }

    core_forceinline void pushContext( Context v ) { mStack.push_back( v ); }

    core_forceinline void popContext()
    {
      if ( mStack.empty() )
      {
        corePrint( "internal error: corrupt generate code context" );
      }
      else
        mStack.pop_back();
    }

    core_forceinline Context getContext()
    {
      if ( mStack.empty() )
      {
        corePrint( "internal error: corrupt generate code context" );
        return Context::NONE;
      }
  
      return mStack.back();
    }

  private:
    std::list< Context > mStack;

  friend class ScopedScriptContextStack;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class ScopedScriptContextStack
{
  public:
    ScopedScriptContextStack( ScriptContextStack *context, ScriptContextStack::Context c ) :
      mContext( context )
    {
      mContext->pushContext( c );
    }

    ~ScopedScriptContextStack()
    {
      mContext->popContext();
    }

  private:
    ScriptContextStack *mContext = nullptr;

};

/*
 
  sample:

    {
      SCOPED_SCRIPT_CONTEXT( FETCH );

      // generate code

      // context will pop automatically at end of scope
    }

*/
#define SCOPED_SCRIPT_CONTEXT( _c ) ScopedScriptContextStack _ScopedScriptContextStack( mCompiler->getContextStack(), ScriptContextStack::Context::##_c )

} }

#endif
