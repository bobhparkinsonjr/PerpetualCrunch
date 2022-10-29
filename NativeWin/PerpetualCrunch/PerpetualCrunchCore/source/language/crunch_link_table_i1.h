#if !defined ( CRUNCH_LINK_TABLE_I1_H )
#define CRUNCH_LINK_TABLE_I1_H

#include "../core/core_binary_buffer.h"

#include "crunch_link_table.h"

namespace crunch { namespace language {

class LinkTableI1 : public LinkTable
{
  public:
    class OffsetEntry
    {
      public:
        uint32_t mIndex;
        std::vector< uint32_t > mOffsets;

      public:
        inline bool operator < ( const OffsetEntry& rhs ) const
        {
          return ( mIndex < rhs.mIndex );
        }

    };

    typedef std::set< OffsetEntry > OffsetEntrySetType;

  public:
    LinkTableI1( ScriptInstruction::Opcode linkOpcode = ScriptInstruction::Opcode::OP_NONE );
    virtual ~LinkTableI1();

  public:
    virtual void clear() override { mOffsetEntries.clear(); }

    virtual bool link( ScriptVM *vm, ScriptExecutable *se ) const override;

    virtual void appendAssembler( crunch::core::String *dest ) const override;

    bool append( uint32_t index, uint32_t offset );

  public:
    virtual bool read( crunch::core::BinaryBuffer *source ) override;
    virtual bool write( crunch::core::BinaryBuffer *dest ) const override;

  private:
    bool link_LIB_GFUNC_CALL( ScriptVM *vm, ScriptExecutable *se ) const;

  private:
    OffsetEntrySetType mOffsetEntries;

};

} }

#endif
