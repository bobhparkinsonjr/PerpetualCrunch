#if !defined ( LIBCLASS_FUNCTIONS_F64V3_H )
#define LIBCLASS_FUNCTIONS_F64V3_H

#include "../crunch_script_library_tools.h"
#include "../crunch_script_library_function.h"
#include "../crunch_script_library_class.h"

namespace crunch { namespace language {

class ScriptLibraryClass_f64v3 : public ScriptLibraryClass
{
  public:
    ScriptLibraryClass_f64v3();

  public:
    virtual bool setup() override;

    // if this class supports the [] operator (get and/or set), and if all instances of this class use the same base type, then this
    // function needs to return the type
    virtual ScriptTypes::ScriptTypeDescription getBaseType() const override { return ScriptTypes::ScriptTypeDescription( ScriptTypes::ScriptType::TYPE_F64 ); }

};

#define _class_name f64v3
#include "../crunch_libclass_declare_start.mac"
#include "libclass_functions_f64v3.table"
#include "../crunch_libclass_declare_end.mac"
#undef _class_name

} }

#endif
