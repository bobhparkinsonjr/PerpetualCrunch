
#include "core_global.h"

#include "core_qsort.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace core {

///////////////////////////////////////////////////////////////////////////////////////////////////

struct sort_r_data
{
  void *arg;
  int (*compar)( const void *a1, const void *a2, void *aarg );
};

int sort_r_arg_swap( void *s, const void *aa, const void *bb )
{
  struct sort_r_data *ss = (struct sort_r_data*)s;
  return (ss->compar)( aa, bb, ss->arg );
}

///////////////////////////////////////////////////////////////////////////////////////////////////

struct reverse_compare
{
  void *context;
  int (*compare)( void *context, const void *a1, const void *a2 );
};

int reverse_compare_callback( const void *a1, const void *a2, void *aarg )
{
  struct reverse_compare *s = (struct reverse_compare*)( aarg );
  return ( (s->compare)( s->context, a1, a2 ) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void sort_r( void *base, size_t nel, size_t width,
             int (*compar)( const void *a1, const void *a2, void *aarg ), void *arg )
{
  #if defined _CRUNCH_ANDROID
  
  assert( false );
  
  #elif ( defined _GNU_SOURCE || defined __GNU__ || defined __linux__ )

    qsort_r(base, nel, width, compar, arg);

  #elif ( defined _WIN32 || defined _WIN64 || defined __WINDOWS__ )

  // struct sort_r_data tmp = {arg, compar};
  // qsort_s(*base, nel, width, &sort_r_arg_swap, &tmp);
	assert( false );

  #else
    #error unknown platform
  #endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////

#if defined _CRUNCH_ANDROID || defined _CRUNCH_LINUX

#define _element( _base, _width, _index ) (void*)( (INT_PTR)(_base) + (INT_PTR)( (int)(_index) * (int)(_width) ) ) 

int crunch_generic_qsort_choose_pivot( void *base, int lo, int hi, size_t width, int (*compare)( void *, const void *, const void * ), void *context )
{
  return ( lo + (int)( (float)( hi - lo ) * 0.5f ) );
}

void crunch_generic_qsort_swap( void *base, size_t width, int i0, int i1 )
{
  char temp[width];
  memcpy( temp, _element( base, width, i0 ), width );
  memcpy( _element( base, width, i0 ), _element( base, width, i1 ), width );
  memcpy( _element( base, width, i1 ), temp, width );
}

int crunch_generic_qsort_partition( void *base, int lo, int hi, size_t width, int (*compare)( void *, const void *, const void * ), void *context )
{
  int pivotIndex = crunch_generic_qsort_choose_pivot( base, lo, hi, width, compare, context );
  
  char pivotValue[width]; 
  memcpy( pivotValue, _element( base, width, pivotIndex ), width );
  
  crunch_generic_qsort_swap( base, width, pivotIndex, hi );
  
  int storeIndex = lo;
  
  for ( int i = lo; i < hi; ++i )
  {
    if ( compare( context, _element( base, width, i ), &( pivotValue[0] ) ) < 0 )
    {
      crunch_generic_qsort_swap( base, width, i, storeIndex );
      ++storeIndex;
    }
  }
  
  crunch_generic_qsort_swap( base, width, storeIndex, hi );
  
  return storeIndex;
}

void crunch_generic_qsort( void *base, size_t width, int (*compare)( void *, const void *, const void * ), void *context, int lo, int hi )
{
  if ( lo < hi )
  {
    int p = crunch_generic_qsort_partition( base, lo, hi, width, compare, context );
    crunch_generic_qsort( base, width, compare, context, lo, p - 1 );
    crunch_generic_qsort( base, width, compare, context, p + 1, hi );
  }
}

void crunch_generic_qsort( void *base, size_t num, size_t width, int (*compare)( void *, const void *, const void * ), void *context )
{
  crunch_generic_qsort( base, width, compare, context, 0, (int)( num ) - 1 );
}

#undef _element

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////

#if defined _CRUNCH_WINDOWS
void core_qsort( void *base, size_t num, size_t width, int (__cdecl *compare)(void *, const void *, const void *), void * context )
#else
void core_qsort( void *base, size_t num, size_t width, int (*compare)(void *, const void *, const void *), void * context )
#endif
{
  #if defined _CRUNCH_WINDOWS
  
  qsort_s( base, num, width, compare, context );
  
  #elif defined _CRUNCH_ANDROID
  
  crunch_generic_qsort( base, num, width, compare, context );
  
  #elif defined _CRUNCH_LINUX
  
  crunch_generic_qsort( base, num, width, compare, context );
  
  #else
  
  #error unknown platform
  
  #endif
}

} }
