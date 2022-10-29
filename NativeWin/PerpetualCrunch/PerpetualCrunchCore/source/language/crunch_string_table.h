#if !defined ( CRUNCH_STRING_TABLE_H )
#define CRUNCH_STRING_TABLE_H

#include "../core/core_binary_buffer.h"

namespace crunch { namespace language {

class StringTable
{
  public:
    StringTable();
    ~StringTable();

  public:
    void destroy();

    // in the case of a literalString, if surrounding quotes (double or single) are found, they will be stripped
    crunch::core::String* mergeString( const char *v, bool literalString, unsigned int *index = nullptr );

    unsigned int getIndex( const crunch::core::String *s ) const;
    const crunch::core::String* getString( unsigned int index ) const;

  public:
    bool read( crunch::core::BinaryBuffer *source );
    bool write( crunch::core::BinaryBuffer *dest ) const;

  public:
    void appendAssembler( crunch::core::String *dest ) const;

    void toCorePrint() const;

  private:
    std::vector< crunch::core::String* > mPool;

};

} }

#endif
