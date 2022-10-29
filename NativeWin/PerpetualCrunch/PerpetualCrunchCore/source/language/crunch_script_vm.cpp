
#include "../crunch_global.h"

#include "crunch_script_executable.h"
#include "crunch_script_thread.h"

#include "crunch_script_vm.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

ScriptVM::ScriptVM( ScriptExecutable *executable ) :
  mExecutable( executable )
{
  mThreadsMutex.create();
}

ScriptVM::~ScriptVM()
{
  destroy();
}

void ScriptVM::destroy()
{
  mProperties.clear();

  cancel();
  assert( mThreads.empty() );
}

bool ScriptVM::setup()
{
  if ( mExecutable == nullptr )
    return false;

  if ( mSetup )
    return true;

  if ( !( mExecutable->link( this ) ) )
    return false;

  mSetup = true;
  return true;
}

void ScriptVM::cancel()
{
  // request all threads shut down via cancel
  {
    scoped_lock( &mThreadsMutex );

    ThreadListType::iterator it = mThreads.begin();
    ThreadListType::iterator itEnd = mThreads.end();
    for ( ; it != itEnd; ++it )
      ( *it )->cancel();
  }

  // block until all threads have shut down
  {
    while ( true )
    {
      {
        scoped_lock( &mThreadsMutex );

        if ( mThreads.empty() )
          break;

        ThreadListType::iterator it = mThreads.begin();
        while ( it != mThreads.end() )
        {
          ScriptThread *thread = *it;

          if ( thread->isRunning() )
            ++it;
          else
          {
            delete thread;
            it = mThreads.erase( it );
          }
        }
      }

      coreSleep( 100 );
    }
  }
}

bool ScriptVM::addGlobalLibraryClassProperty( const crunch::core::String& msClassName, const crunch::core::String& msInstanceName, void *classInstance )
{
  if ( mSetup || msClassName == "" || msInstanceName == "" )
    return false;

  PropertyInfo pi;
  pi.mName = crunch::core::String::fmt( "%s.%s", msClassName.c_str(), msInstanceName.c_str() );
  pi.mClassInstance = classInstance;

  std::pair< PropertyInfoSetType::iterator, bool > ret = mProperties.insert( pi );

  return ret.second;
}

bool ScriptVM::findGlobalLibraryClassProperty( const crunch::core::String& msClassName, const crunch::core::String& msInstanceName, void **classInstance ) const
{
  if ( classInstance == nullptr )
    return false;

  PropertyInfo findMe;
  findMe.mName = crunch::core::String::fmt( "%s.%s", msClassName.c_str(), msInstanceName.c_str() );

  PropertyInfoSetType::iterator findIt = mProperties.find( findMe );

  if ( findIt != mProperties.end() )
  {
    *classInstance = findIt->mClassInstance;
    return true;
  }

  return false;
}

ScriptThread* ScriptVM::createThread()
{
  if ( !mSetup )
    return nullptr;

  scoped_lock( &mThreadsMutex );
  ScriptThread *t = new ScriptThread( this, mExecutable );
  mThreads.push_back( t );
  return t;
}

bool ScriptVM::destroyThread( ScriptThread *thread )
{
  if ( thread == nullptr || thread->isRunning() || thread->getVM() != this )
    return false;

  scoped_lock( &mThreadsMutex );

  ThreadListType::iterator it = mThreads.begin();
  ThreadListType::iterator itEnd = mThreads.end();

  for ( ; it != itEnd; ++it )
  {
    if ( *it == thread )
    {
      mThreads.erase( it );
      delete thread;
      return true;
    }
  }

  return false;
}

} }
