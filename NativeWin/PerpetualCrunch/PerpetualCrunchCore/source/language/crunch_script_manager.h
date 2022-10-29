#if !defined ( CRUNCH_SCRIPT_MANAGER_H )
#define CRUNCH_SCRIPT_MANAGER_H

#include "crunch_script_compiler.h"

namespace crunch { namespace language {

class ScriptManager final
{
  private:
    ScriptManager();
    ScriptManager( const ScriptManager& other );
    ScriptManager& operator = ( const ScriptManager& other );
    ~ScriptManager();

  public:
    static void setup();
    static void destroy();

  private:
    static ScriptCompiler *mCompiler = nullptr;

};

} }

#endif
