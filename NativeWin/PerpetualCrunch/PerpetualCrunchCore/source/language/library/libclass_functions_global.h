#if !defined ( LIBCLASS_FUNCTIONS_GLOBAL_H )
#define LIBCLASS_FUNCTIONS_GLOBAL_H

#include "../crunch_script_library_tools.h"
#include "../crunch_script_library_function.h"
#include "../crunch_script_library_class.h"

namespace crunch { namespace language {

#define CRUNCH_LIB_GLOBAL_CLASS_NAME "_GLOBAL_CLASS"

class ScriptLibraryClass_GlobalLibraryClass : public ScriptLibraryClass
{
  public:
    ScriptLibraryClass_GlobalLibraryClass();

  public:
    virtual bool setup() override;

};

#define _class_name GlobalLibraryClass
#include "../crunch_libclass_declare_start.mac"
#include "libclass_functions_global.table"
#include "../crunch_libclass_declare_end.mac"
#undef _class_name

} }

#endif
