#if !defined ( CRUNCH_LINK_TABLE_H )
#define CRUNCH_LINK_TABLE_H

#include "../core/core_binary_buffer.h"

#include "crunch_script_instruction.h"

namespace crunch { namespace language {

class ScriptVM;
class ScriptExecutable;

class LinkTable
{
  public:
    LinkTable( ScriptInstruction::Opcode linkOpcode = ScriptInstruction::Opcode::OP_NONE );
    virtual ~LinkTable();

  public:
    virtual void clear() {}

    virtual bool link( ScriptVM *vm, ScriptExecutable *se ) const { return false; }

    virtual void appendAssembler( crunch::core::String *dest ) const;

  public:
    virtual bool read( crunch::core::BinaryBuffer *source );
    virtual bool write( crunch::core::BinaryBuffer *dest ) const;

  protected:
    ScriptInstruction::Opcode mLinkOpcode = ScriptInstruction::Opcode::OP_NONE;

};

} }

#endif
