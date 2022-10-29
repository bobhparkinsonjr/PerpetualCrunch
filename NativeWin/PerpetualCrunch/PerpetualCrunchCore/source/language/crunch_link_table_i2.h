#if !defined ( CRUNCH_LINK_TABLE_I2_H )
#define CRUNCH_LINK_TABLE_I2_H

#include "../core/core_binary_buffer.h"

#include "crunch_link_table.h"

namespace crunch { namespace language {

class LinkTableI2 : public LinkTable
{
  public:
    class OffsetEntry
    {
      public:
        uint32_t mIndex;
        uint64_t mIndex2;
        std::vector< uint32_t > mOffsets;

      public:
        inline bool operator < ( const OffsetEntry& rhs ) const
        {
          if ( mIndex != rhs.mIndex )
            return ( mIndex < rhs.mIndex );
          return ( mIndex2 < rhs.mIndex2 );
        }

    };

    typedef std::set< OffsetEntry > OffsetEntrySetType;

  public:
    LinkTableI2( ScriptInstruction::Opcode linkOpcode = ScriptInstruction::Opcode::OP_NONE );
    virtual ~LinkTableI2();

  public:
    virtual void clear() override { mOffsetEntries.clear(); }

    virtual bool link( ScriptVM *vm, ScriptExecutable *se ) const override;

    virtual void appendAssembler( crunch::core::String *dest ) const override;

    bool append( uint32_t index, uint64_t index2, uint32_t offset );

  public:
    virtual bool read( crunch::core::BinaryBuffer *source ) override;
    virtual bool write( crunch::core::BinaryBuffer *dest ) const override;

  private:
    bool link_PUSH_FETCH_GLIB_CLS( ScriptVM *vm, ScriptExecutable *se ) const;
    bool link_LIB_FUNC_CALL( ScriptVM *vm, ScriptExecutable *se ) const;

  private:
    OffsetEntrySetType mOffsetEntries;

};

} }

#endif
