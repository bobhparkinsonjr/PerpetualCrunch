#if !defined ( CRUNCH_SCRIPT_DATA_STACK_H )
#define CRUNCH_SCRIPT_DATA_STACK_H

#include "crunch_script_types.h"

#define CRUNCH_DATA_STACK_EXTREME_SIZE ( 1024 * 1024 * 512 )

namespace crunch { namespace language {

class ScriptDataStack final
{
  public:
    ScriptDataStack( unsigned int initialSize = 1024 * 1024 * 4, unsigned int expansionSize = 1024 * 1024 * 1 );
    ~ScriptDataStack();

  public:
    core_forceinline uint64_t getOffset() const { return (uint64_t)( mSP - &( mStack[0] ) ); }
    core_forceinline bool empty() const { return ( mSP == &( mStack[0] ) ); }

    // WARNING: do not save this value, should this stack need to expand this address may become invalid
    //          this value is considered safe to use until the next push* call
    core_forceinline char* getSPOffset( uint32_t offset )
    {
      assert( ( mSP - offset ) >= &( mStack[0] ) );
      return ( mSP - offset );
    }

  public:
    core_forceinline void push( const void *source, uint32_t size )
    {
      updateStackSize( size );
      memcpy( mSP, source, size );
      mSP += size;
    }

    core_forceinline void push_string( const crunch_string *s )
    {
      updateStackSize( sizeof( crunch_string* ) );
      *(const crunch_string**)( mSP ) = s;
      mSP += sizeof( crunch_string* );
    }

    core_forceinline void push_i64( crunch_i64 i )
    {
      updateStackSize( sizeof( crunch_i64 ) );
      *(crunch_i64*)( mSP ) = i;
      mSP += sizeof( crunch_i64 );
    }

    core_forceinline void push_u64( crunch_u64 u )
    {
      updateStackSize( sizeof( crunch_u64 ) );
      *(crunch_u64*)( mSP ) = u;
      mSP += sizeof( crunch_u64 );
    }

    core_forceinline void push_f64( crunch_f64 f )
    {
      updateStackSize( sizeof( crunch_f64 ) );
      *(crunch_f64*)( mSP ) = f;
      mSP += sizeof( crunch_f64 );
    }

    core_forceinline void push_f64v3( crunch_f64 *v )
    {
      updateStackSize( sizeof( crunch_f64 ) * 3 );
      memcpy( mSP, v, sizeof( crunch_f64 ) * 3 );
      mSP += ( sizeof( crunch_f64 ) * 3 );
    }

    core_forceinline void push_f64v4( crunch_f64 *v )
    {
      updateStackSize( sizeof( crunch_f64 ) * 4 );
      memcpy( mSP, v, sizeof( crunch_f64 ) * 4 );
      mSP += ( sizeof( crunch_f64 ) * 4 );
    }

    core_forceinline void push_fetch( uint32_t dataOffset, uint64_t dataSize )
    {
      updateStackSize( dataSize );
      memcpy( mSP, &( mStack[dataOffset] ), dataSize );
      mSP += dataSize;
    }

    core_forceinline void push_ptr( void *ptr )
    {
      updateStackSize( sizeof( void* ) );
      *(void**)( mSP ) = ptr;
      mSP += sizeof( void* );
    }

  public:
    core_forceinline void pop( uint32_t size )
    {
      assert( ( mSP - size ) >= &( mStack[0] ) );
      mSP -= size;
    }

    // returnTypeStackSize bytes is on top of the stack, and will be kept on top
    // scopeStackSize bytes are under the return value, they will be removed from the stack
    core_forceinline void collapse( uint32_t scopeStackSize, uint32_t returnTypeStackSize )
    {
      if ( scopeStackSize == 0 )
        return;

      if ( returnTypeStackSize == 0 )
      {
        pop( scopeStackSize );
        return;
      }

      assert( ( mSP - ( scopeStackSize + returnTypeStackSize ) ) >= &( mStack[0] ) );

      mSP -= ( scopeStackSize + returnTypeStackSize );

      if ( scopeStackSize < returnTypeStackSize )
        memmove( mSP, mSP + scopeStackSize, returnTypeStackSize );
      else
        memcpy( mSP, mSP + scopeStackSize, returnTypeStackSize );

      mSP += returnTypeStackSize;
    }

    core_forceinline const crunch_string* pop_string()
    {
      assert( ( mSP - sizeof( const crunch_string* ) ) >= &( mStack[0] ) );
      mSP -= sizeof( const crunch_string* );
      return *(const crunch_string**)( mSP );
    }

    core_forceinline crunch_i64 pop_i64()
    {
      assert( ( mSP - sizeof( crunch_i64 ) ) >= &( mStack[0] ) );
      mSP -= sizeof( crunch_i64 );
      return *(crunch_i64*)( mSP );
    }

    core_forceinline crunch_u64 pop_u64()
    {
      assert( ( mSP - sizeof( crunch_u64 ) ) >= &( mStack[0] ) );
      mSP -= sizeof( crunch_u64 );
      return *(crunch_u64*)( mSP );
    }

    core_forceinline crunch_f64 pop_f64()
    {
      assert( ( mSP - sizeof( crunch_f64 ) ) >= &( mStack[0] ) );
      mSP -= sizeof( crunch_f64 );
      return *(crunch_f64*)( mSP );
    }

    core_forceinline void pop_f64v3( crunch_f64 *v )
    {
      assert( ( mSP - ( sizeof( crunch_f64 ) * 3 ) ) >= &( mStack[0] ) );
      mSP -= ( sizeof( crunch_f64 ) * 3 );
      v[0] = *(crunch_f64*)( mSP );
      v[1] = *(crunch_f64*)( mSP + sizeof( crunch_f64 ) );
      v[2] = *(crunch_f64*)( mSP + ( sizeof( crunch_f64 ) * 2 ) );
    }

    core_forceinline void pop_f64v4( crunch_f64 *v )
    {
      assert( ( mSP - ( sizeof( crunch_f64 ) * 4 ) ) >= &( mStack[0] ) );
      mSP -= ( sizeof( crunch_f64 ) * 4 );
      v[0] = *(crunch_f64*)( mSP );
      v[1] = *(crunch_f64*)( mSP + sizeof( crunch_f64 ) );
      v[2] = *(crunch_f64*)( mSP + ( sizeof( crunch_f64 ) * 2 ) );
      v[3] = *(crunch_f64*)( mSP + ( sizeof( crunch_f64 ) * 3 ) );
    }

    core_forceinline void pop_store( uint32_t dataOffset, uint64_t dataSize )
    {
      assert( ( mSP - dataSize ) >= &( mStack[0] ) );
      mSP -= dataSize;
      memcpy( &( mStack[dataOffset] ), mSP, dataSize );
    }

  public:
    core_forceinline crunch_f64 read_f64()
    {
      assert( ( mSP - sizeof( crunch_f64 ) ) >= &( mStack[0] ) );
      return *(crunch_f64*)( mSP - sizeof( crunch_f64 ) );
    }

  private:
    core_forceinline void updateStackSize( uint64_t size )
    {
      if ( ( mSP + size ) > &( mStack.back() ) )
        expandStack();
    }

    core_forceinline void expandStack()
    {
      uint64_t offset = (uint64_t)( mSP - &( mStack[0] ) );
      mStack.resize( mStack.size() + mExpansionSize );

      #if defined CRUNCH_DATA_STACK_EXTREME_SIZE
      assert( mStack.size() < CRUNCH_DATA_STACK_EXTREME_SIZE );
      #endif

      mSP = &( mStack[offset] );
    }

  private:
    std::vector< char > mStack;
    char *mSP = nullptr;

    unsigned int mExpansionSize = 0;

};

} }

#endif
