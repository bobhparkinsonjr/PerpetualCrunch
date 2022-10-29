#if !defined ( CRUNCH_SCRIPT_VM_H )
#define CRUNCH_SCRIPT_VM_H

#include "../core/core_mutex.h"

#include "crunch_script_types.h"

namespace crunch { namespace language {

class ScriptExecutable;
class ScriptThread;

class ScriptVM
{
  public:
    typedef std::list< ScriptThread* > ThreadListType;

  public:
    class PropertyInfo
    {
      public:
        core_forceinline bool operator < ( const PropertyInfo& rhs ) const
        {
          return ( mName < rhs.mName );
        }

      public:
        crunch::core::String mName;
        void *mClassInstance = nullptr;

    };

    typedef std::set< PropertyInfo > PropertyInfoSetType;

  public:
    // NOTE: this executable should not have been linked up yet
    ScriptVM( ScriptExecutable *executable = nullptr );

    virtual ~ScriptVM();

  public:
    virtual void destroy();

    // NOTE: must call this after done adding properties, and before creating any threads
    bool setup();

    // this will cancel all current threads and block until they shut down
    void cancel();

    core_forceinline const ScriptExecutable* getExecutable() const { return mExecutable; }

  public:
    bool addGlobalLibraryClassProperty( const crunch::core::String& msClassName, const crunch::core::String& msInstanceName, void *classInstance );
    bool findGlobalLibraryClassProperty( const crunch::core::String& msClassName, const crunch::core::String& msInstanceName, void **classInstance ) const;

  public:
    core_forceinline bool hasThreads() const { return !( mThreads.empty() ); }

    ScriptThread* createThread();
    bool destroyThread( ScriptThread *thread );

  protected:
    bool mSetup = false;

    ScriptExecutable *mExecutable = nullptr;

    crunch::core::CoreMutex mThreadsMutex;
    ThreadListType mThreads;

    PropertyInfoSetType mProperties;

};

} }

#endif
